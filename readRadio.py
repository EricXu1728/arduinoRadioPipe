import serial.tools.list_ports

def list_serial_ports():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        print(f"Port: {port.device}, Description: {port.description}")

if __name__ == '__main__':
    list_serial_ports()

import serial
import csv
import time

# Set up the serial connection (make sure to adjust the port to your Arduino's port)
ser = serial.Serial('/dev/ttyUSB0', 9600)  # For Windows, this might be 'COM3' or similar

# Open the CSV file for writing
with open('data.csv', mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['SensorValue', 'Temperature', 'Humidity'])  # Write CSV header

    try:
        while True:
            if ser.in_waiting > 0:
                # Read the incoming data from the serial port
                data = ser.readline().decode('utf-8').strip()
                # Split the data into CSV format
                row = data.split(',')
                # Write the row to the CSV file
                writer.writerow(row)
                print(f"Data written: {row}")
                time.sleep(1)
    except KeyboardInterrupt:
        print("Program interrupted by user")
    finally:
        ser.close()  # Close the serial port
