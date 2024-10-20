#include <SPI.h>
#include <RH_RF95.h>

// Broadcasting Radio. Hooked up to RaceCapture and Pi
// Provide the RFM95 module's chip select and interrupt pins
RH_RF95 rf95(12, 6);

int LED = 13;                               // Status LED is on pin 13
float frequency = 921.2;                    // Broadcast frequency
unsigned long lastSendTime = 0;             // To track time for sending messages
unsigned long sendInterval = 10000;         // 10 seconds interval for sending incoming data
unsigned long lastHealthCheckTime = 0;      // To track time for health check
unsigned long healthCheckInterval = 30000;  // 30 seconds interval for health check

void setup() {


  pinMode(LED, OUTPUT);

  // Initialize the Radio
  if (!rf95.init()) {
    digitalWrite(LED, HIGH);
    while (1)
      ;  // Freeze if initialization fails
  }

  // Set frequency
  rf95.setFrequency(frequency);

  // Set transmission power (14 dBm in this case)
  rf95.setTxPower(14, false);

  // Initial LED signal to show the system is ready
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);

  // Start the Serial communication
  SerialUSB.begin(9600);  // Adjust as needed
}

int incomingByte = 0;
int testingMode = 0;


void loop() {
  // Check if there is incoming data
  if (SerialUSB.available() > 0) {
    // read the incoming string until newline:
    String incomingString = SerialUSB.readStringUntil('\n');

    // say what you got:
    SerialUSB.print("I received: ");
    SerialUSB.println(incomingString);

    // Convert the incoming string to a char array
    const char *message = incomingString.c_str();

    // Send the message through the radio
    rf95.send((uint8_t *)message, strlen(message) + 1);  // Send the message

    rf95.waitPacketSent();  // Wait for the packet to be sent

    digitalWrite(LED, HIGH);  // Turn on LED while sending
    delay(100);               // Keep LED on for 100ms
    digitalWrite(LED, LOW);   // Turn off LED

    lastSendTime = millis();  // Update last send time
  } else if (testingMode!=0) {
    const char *testMessage = "I have no mail";
    rf95.send((uint8_t *)testMessage, strlen(testMessage) + 1);  // Send test message
    rf95.waitPacketSent();                                       // Wait for the packet to be sent

    digitalWrite(LED, HIGH);  // Turn on LED while sending
    delay(100);               // Keep LED on for 100ms
    digitalWrite(LED, LOW);   // Turn off LED

    lastSendTime = millis();  // Update last send time
  }

  if (testingMode != 0 && millis() - lastSendTime > sendInterval) {
    const char *testMessage = "Broadcasting Radio is on";
    rf95.send((uint8_t *)testMessage, strlen(testMessage) + 1);  // Send test message
    rf95.waitPacketSent();                                       // Wait for the packet to be sent

    digitalWrite(LED, HIGH);  // Turn on LED while sending
    delay(100);               // Keep LED on for 100ms
    digitalWrite(LED, LOW);   // Turn off LED

    lastSendTime = millis();  // Update last send time


    SerialUSB.println("sent something to  Client!");
  }
}
