try:
    import serial  # Python2
except ImportError:
    from serial3 import *  # Python3
import time


# Define the serial port and baud rate
ser = serial.Serial('/dev/cu.usbmodem1423301', 9600) 

LIDAR_POWER_ENABLE_PIN1 = 9
LIDAR_POWER_ENABLE_PIN2 = 10

def enableLidar(lidar_pin):
    ser.write(bytes([lidar_pin, 1]))  # Send command to enable Lidar


def disableLidar(lidar_pin):
    ser.write(bytes([lidar_pin, 0]))  # Send command to disable Lidar

# Setup Lidar
def setup():
    # Set the Lidar Power Enable pins as outputs
    ser.write(bytes([LIDAR_POWER_ENABLE_PIN1, 2]))  # Send command to set pin mode as output
    ser.write(bytes([LIDAR_POWER_ENABLE_PIN2, 2]))  # Send command to set pin mode as output


# Read distance from Lidar
def readDistance():
    ser.write(bytes([0x62, 4]))  # Send command to start distance measurement
    time.sleep(1)  # Add a delay to allow the sensor to measure distance
    response = ser.read(2)  # Read 2 bytes from serial
    distance = int.from_bytes(response, byteorder='big')  # Convert bytes to integer
    return (distance % 100.0)

# Main function
def main():
    setup()
    alternate = False
    while True:
        if alternate:
            disableLidar(LIDAR_POWER_ENABLE_PIN1)
            enableLidar(LIDAR_POWER_ENABLE_PIN2)
            alternate = False
        else:
            enableLidar(LIDAR_POWER_ENABLE_PIN1)
            disableLidar(LIDAR_POWER_ENABLE_PIN2)
            alternate = True
        
        dist = readDistance()
        print(f"{dist} cm")
        time.sleep(0.01)  # Adjust delay as necessary

if __name__ == "__main__":
    main()

