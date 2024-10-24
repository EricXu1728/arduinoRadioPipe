import serial
import asyncio
import websockets
import csv
import serial.tools.list_ports



# List available serial ports
def list_serial_ports():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        print(f"Port: {port.device}, Description: {port.description}")

# Function to handle WebSocket connections and save to CSV
async def handler(websocket, path):
    ser = serial.Serial('COM15', 9600)  # Replace with your port

    # Open the CSV file for writing as a backup
    with open('data_backup.csv', mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['Rpm1', 'Rpm2', 'Tick'])  # CSV header
        i = 0
        try:
            while True:
                if ser.in_waiting > 0:
                    # Read the incoming data from the serial port
                    data = ser.readline().decode('utf-8').strip()
                    #data = "1, "+ str(i) +",1"
                    #i+=1
                    # Send the data to the WebSocket client (Vue frontend)
                    await websocket.send(data)
                    print(f"Data sent: {data}")

                    # Write the data to the CSV file
                    row = data.split(',')  # Assuming the data is comma-separated
                    writer.writerow(row)
                    print(f"Data saved to CSV: {row}")

                    # Wait for a second between reads
                    #await asyncio.sleep(1)

        except Exception as e:
            print(f"Error: {e}")

        finally:
            ser.close()  # Close the serial port

# Start WebSocket server
async def main():
    async with websockets.serve(handler, "localhost", 8765):  # Serve WebSocket on localhost
        print("WebSocket server started on ws://localhost:8765")
        await asyncio.Future()  # Run forever

if __name__ == '__main__':
    list_serial_ports()
    asyncio.run(main())
