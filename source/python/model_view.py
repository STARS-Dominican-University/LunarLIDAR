import plotly.graph_objects as go
import pandas as pd

data = pd.read_csv('../../data/mock_coordinates.csv')

x_plot = data['x']
y_plot = data['y']
z_plot = data['z']

surface_plot = go.Surface(x=x_plot, y=y_plot, z=z_plot, showscale=False)

# Create scatter plot for data points
scatter_plot = go.Scatter3d(
    x=x_plot,
    y=y_plot,
    z=z_plot,
    mode='markers',
    marker=dict(
        size=2,
        color='white',  # Change marker color to white
        opacity=0.8
    )
)

# Create figure with both surface and scatter plots
fig = go.Figure(data=[surface_plot, scatter_plot])

# Update layout
fig.update_layout(
    title='3D Point Cloud',
    width=600, height=600,
    margin=dict(t=40, r=0, l=20, b=20)
)

camera = dict(
    up=dict(x=0, y=0, z=1),
    center=dict(x=0, y=0, z=0),
    eye=dict(x=1.25, y=1.25, z=1.25)
)

fig.update_layout(scene_camera=camera)

fig.show()