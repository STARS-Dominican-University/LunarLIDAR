from plotly.offline import plot
import plotly.graph_objs as go
import plotly.express as px
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import serial as sr
import regex as re
import random
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

pin_1 = True # Pin 9
pin_2 = False # Pin 10


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
            
def visusalize(x_data, y_data, z_data):
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
            bgcolor='black'  # Set background color to black
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

def get_x_coordinate(data: str) -> np.int64:
    x_coordinate = re.findall(r'\b\d+\b', data)
    if x_coordinate:
        return np.int64(x_coordinate[0])
    else:
        return np.int64(-1)
    
def print_coordinate(cords: np.array, theta, omega):
    print(f'X: {cords[0]}, Y: {cords[1]}, Z: {cords[2]} | Θ: {theta}, ω: {omega}')

def save_coordinate(x: int, theta, omega) -> np.array:
    # Perform math to get accurate coordinates
    x_cord = np.multiply(x, np.sin(theta))
    y_cord = np.multiply(x_cord, np.sin(omega))
    z_cord = np.multiply(x, np.cos(theta))
    # print_coordinate([x_cord, y_cord, z_cord], theta, omega)
    return np.around(np.array([x_cord, y_cord, z_cord, theta, omega]), 3)

def generate_mock_coordiantes(num):
    x = np.around(np.random.uniform(low=0, high=1000, size=(num, )), 3)
    y = np.around(np.random.uniform(low=0, high=1000, size=(num, )), 3)
    z = np.around(np.random.uniform(low=0, high=1000, size=(num, )), 3)
    return np.array([x, y, z])

def generate_mock_measurements(num):
    measurement = []
    for _ in range(num):
        x = np.random.randint(0, 2000)
        measurement.append(f'{x} cm')
    return np.array(measurement)

def save_data(file_name: str, data: pd.DataFrame):
    file_path = f'../../data/{file_name}.csv'
    if os.path.exists(file_path):
        os.remove(file_path)
    data.to_csv("../../data/mock_coordinates.csv")
    
def simulate(yaws,
             pitches,
             increments):
    fake_frame = pd.DataFrame(columns=['x', 'y', 'z', 'angle_1', 'angle_2'])
    increment = np.divide(np.multiply(increments, np.pi), 180)
    omega = 0
    for i in range(yaws):
        omega += increment
        theta = np.divide(np.negative(np.pi), 2)
        fake_cords = generate_mock_coordiantes(1000)
        x_cords, y_cords, z_cords = fake_cords
        for j in range(pitches):
            fake_frame.loc[len(fake_frame)] = save_coordinate(x_cords[j], theta, omega)
            theta += increment
    save_data("mock_coordinates", fake_frame)
    # print(f'Simulation complete ...')
    
simulate(yaws=400, pitches=200, increments=1.8)

status("Visualizing", "loading simulation data", "OK")

data = pd.read_csv('../../data/mock_coordinates.csv')

x_plot = data['x']
y_plot = data['y']
z_plot = data['z']

status("Visualizing", "data loaded", "OK")

visusalize(x_plot, y_plot, z_plot)