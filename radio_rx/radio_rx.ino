#include <SPI.h>
#include <RH_RF95.h>

//Receiving Radio
RH_RF95 rf95(12, 6);

int LED = 13; //Status LED on pin 13
float frequency = 921.2;
int counter = 0; // Counter to keep track of iterations

void setup()
{
  pinMode(LED, OUTPUT);
  SerialUSB.begin(9600);
  
  // Wait for serial before continuing
  while (!SerialUSB);

  SerialUSB.println("Receiver booting up!!");

  // Initialize the Radio. 
  if (rf95.init() == false) {
    SerialUSB.println("Radio Init Failed - Freezing");
    while (1);
  } else {
    // LED indicator for successful initialization
    SerialUSB.println("Receiver up!");
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
  }

  rf95.setFrequency(frequency);
}

void loop()
{
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len)) {
      digitalWrite(LED, HIGH); // Turn on status LED
      SerialUSB.print((char*)buf);
      SerialUSB.println();
      digitalWrite(LED, LOW);  // Turn off status LED
    } else {
      SerialUSB.println("Receive failed");
    }
  }

  // Increment the counter
  counter++;

  // Health check
  if (counter % 3000000 == 0) { // Print every 1000 iterations
    SerialUSB.println("The receiving radio is on");
  }
}
