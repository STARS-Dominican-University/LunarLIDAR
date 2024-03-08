from plotly.offline import plot
import plotly.graph_objs as go
import numpy as np
import pandas as pd
import serial as sr
import regex as re
import sys
import os

sys.path.append("../python")

from computers import *

class colors:
    INFO = '\33[36M'
    OK = '\033[92m'
    FOUND = '\033[32m'
    NOTFOUND = '\033[31m'
    ERROR = '\033[91m'
    END = '\033[0m'

def status(str, element, status):
    text = f"{str} : {element}"
    status = status.lower().replace(" ", "")
    if status == 'info':
        print(colors.INFO + text + colors.END)
    elif status == 'ok':
        print(colors.OK + text + colors.END)
    elif status == 'found':
        print(colors.FOUND + text + colors.END)
    elif status == 'notfound':
        print(colors.NOTFOUND + text + colors.END)
    elif status == 'error':
        print(colors.ERROR + text + colors.END)
    else:
        print(text)
            
def visualize(x_data, y_data, z_data):
    status("Loading", "rendering plot", "OK")
    # Create trace for scatter plot
    trace = go.Scatter3d(
        x = x_data,
        y = y_data,
        z = z_data,
        mode='markers',
        marker=dict(
            size=2,
            color='white',  # Change marker color to white,
            opacity=0.8
        )
    )

    # Create layout with custom z-axis range and black background
    layout = go.Layout(
        scene=dict(
            xaxis=dict(visible=False),  # Hide x-axis
            yaxis=dict(visible=False),  # Hide y-axis
            zaxis=dict(visible=False),  # Hide z-axis
            aspectmode='manual',  # Fix aspect ratio
            aspectratio=dict(x=1, y=1, z=1),  # Set aspect ratio to 1:1:1
            bgcolor='black',  # Set background color to black
        ),
        margin=dict(l=0, r=0, t=0, b=0),
        showlegend=False,  # Hide legend
        annotations=[
            dict(
                text='3D Point Cloud Render',
                x=0.5,
                y=1.05,
                showarrow=True,
                font=dict(
                    family='Times New Roman',
                    size=20,
                    color='white'
                ),
                xref='paper',
                yref='paper'
            )
        ]
    )

    # Create figure
    fig = go.Figure(data=[trace], layout=layout)

    # Show plot in Jupyter notebook
    fig.show()

def get_coordinates(data: str):        
    coords = list(map(int, re.findall(r'\w+:(\d+)', data)))
    if coords:
        return np.array(coords).astype(np.int64)
    else:
        return np.int64(-1)
 
def print_coordinate(cords: np.array):
    x, y, z, t, o = cords
    status("Coordinates", f'X: {x}, Y: {y}, Z: {z} | 0: {t}, w: {o}', "INFO")

def save_coordinate(coords:np.array) -> np.array:
    
    x = coords[0]
    theta = np.divide(np.multiply(coords[1], np.pi), 180)
    omega = np.divide(np.multiply(coords[2], np.pi), 180)    
    z_cord = np.multiply(x, np.cos(theta))
    y_cord = np.multiply(np.multiply(x, np.sin(theta)), np.sin(omega))
    x_cord = np.multiply(np.multiply(x, np.sin(theta)), np.cos(omega))
    return np.around(np.array([x_cord, y_cord, z_cord, theta, omega]), 3)

def save_data(file_name: str, data: pd.DataFrame):
    file_path = f'../../data/{file_name}.csv'
    if os.path.exists(file_path):
        os.remove(file_path)
    data.to_csv(f"../../data/{file_name}.csv")
    
pin_1 = True # Pin 3
pin_2 = False # Pin 2

coordinates_frame = pd.DataFrame(columns=['x', 'y', 'z', 'theta', 'omega'])

ser = sr.Serial(port=PABLOS_COMPUTER_MAC, baudrate=9600)  # Adjust the serial port and baud rate
status("Serial Connection", f"connection to port {ser} is established", "OK")

iter = 0
while iter < 150:
    
    # LIDAR logic
    if pin_1:
        pin_1 = False
        pin_2 = True
    else:
        pin_1 = True
        pin_2 = False
        
    # Read data from the serial port
    data = ser.readline().decode('utf-8').replace(" ", "")
    status("Arduino data", data, "FOUND")
    get_coords = get_coordinates(data)
    status("Get data", get_coords, "FOUND")
    
    if (get_coords == -1).any():
        continue
    else:
        coordinates = save_coordinate(get_coords) # array with cords
        coordinates_frame.loc[len(coordinates_frame.index)] = coordinates
        print_coordinate(coordinates)
        save_data("lunar_point_cloud", coordinates_frame)
        
        iter += 1
        status("Iter", iter, "INFO")

status("Visualizing", "loading simulation data", "OK")

data = pd.read_csv('../../data/lunar_point_cloud.csv')

x_plot = data['x']
y_plot = data['y']
z_plot = data['z']

status("Visualizing", "data loaded", "OK")

visualize(x_plot, y_plot, z_plot)
