import numpy as np
import pandas as pd
import serial as sr
import sys
import re
import random

def print_coordinate(cords: np.array):
    print(f'X: {cords[0]}, Y: {cords[1]}, Z: {cords[2]}')

def get_fake_x_coordinate(data: str) -> np.int64:
    x_coordinate = re.findall(r'\b\d+\b', data)
    if x_coordinate:
        return np.int64(x_coordinate[0])
    else:
        return np.int64(-1)
    
def save_fake_coordinate(x: int, y: int, z: int) -> np.array:
    # Perform math to get accurate coordinates
    return np.around(np.array([x, y, z]), 3)

def generate_fake_coordiantes(num):
    x = np.around(np.random.uniform(0, 1000, num))
    y = np.around(np.random.uniform(0, 1000, num))
    z = np.around(np.random.uniform(0, 1000, num))
    return np.array([x, y, z])

def generate_fake_measurements(num):
    measurement = []
    for _ in range(num):
        x = np.random.randint(0, 2000)
        measurement.append(f'{x} cm')
    return np.array(measurement)

fake_serial_data = generate_fake_measurements(50)

fake_coordinates = generate_fake_coordiantes(50)

x_batch, y_batch, z_batch = fake_coordinates


fake_df = pd.DataFrame(columns=["x", "y", "z"])

"""
angle = 0

angle = current_angle + (current_angle*pi)/180
"""

# 1.8 <- increment
# 1.8 + increment
# 1.8 + increment + increment

def save_fake_coordinate(x: int, theta, omega) -> np.array:
    # Perform math to get accurate coordinates
    x_cord = np.multiply(x, np.sin(theta))
    y_cord = np.multiply(x, np.sin(omega))
    z_cord = np.multiply(x, np.cos(theta))
    return np.around(np.array([x_cord, y_cord, z_cord]), 3)

increment = (1.8*np.pi)/180  # 1.8deg -> rad
omega = 0
for _ in range(100):
    omega += increment
    theta = 0
    fake_coordinates = generate_fake_coordiantes(1000)
    x_batch, y_batch, z_batch = fake_coordinates
    for r in range(50):
        print(f'Theta: {theta}, Omega {omega}')
        theta += increment
        fake_df.loc[len(fake_df.index)] = save_fake_coordinate(x_batch[r], theta, omega)
    

import matplotlib.pyplot as plt
import plotly.graph_objs as go

# Create trace for scatter plot
trace = go.Scatter3d(
    x=fake_df['x'],
    y=fake_df['y'],
    z=fake_df['z'],
    mode='markers',
    marker=dict(
        size=2,
        color='white',  # Change marker color to white
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
            text='STARS NASA MIDNS',
            x=0.5,
            y=1.05,
            showarrow=False,
            font=dict(
                family='Arial',
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