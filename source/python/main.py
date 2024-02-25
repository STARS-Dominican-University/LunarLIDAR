import serial
import numpy as np
import pandas  as pd
from computers import *

# ANGLE DATAFRAME
angle_df = pd.DataFrame(columns=['x', 'y', 'z'])
 
pin1 = True
pin2 = False
ANGLE_Y = -45
ANGLE_Z = -90

def getX(tel: str):
    data = tel.strip(' ')[0]
    print(f"data: {data}")
    if data == ">":
        return 0
    return np.int64(data)


ser = serial.Serial(PABLOS_COMPUTER, 9600)  # Adjust the serial port and baud rate

print("connection established ...")
while True:
    
    if ANGLE_Y == 45:
        break

    # Read data from the serial port
    data = ser.readline().decode('utf-8').strip()
    print(f'type: {type(data)}')

    # get lidar 1 points
    print(data)
    x = getX(data)
    y = np.multiply(x, np.tan(ANGLE_Y))
    z = np.multiply(y, np.tan(ANGLE_Z))
    
    if pin1: 
        angle_df.loc[len(angle_df.index)] = [x,y,z]
        ANGLE_Y = ANGLE_Y + 1
        ANGLE_Z = ANGLE_Z + 2
        pin2 = True
        print(f'x:{x}, y:{x}, z:{z}')

    else:
        ANGLE_Y = ANGLE_Y + 1
        ANGLE_Z = ANGLE_Z + 1
        angle_df.loc[len(angle_df.index)] = [x,y,z]
        pin2 = False
        print(f'x:{x}, y:{x}, z:{z}')

angle_df.to_csv("./angles.xlsx")

