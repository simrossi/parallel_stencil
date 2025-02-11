#!/usr/bin/env python3
import matplotlib.pyplot as plt
import numpy as np
import os
import re
from matplotlib.animation import FuncAnimation
from matplotlib.widgets import Slider, Button
import sys

"""
a simple function that reads the number of rows and columns and the cells' values
"""


def read_values(file_path):
    with open(file_path, "r") as file:
        rows, cols = map(int, file.readline().split())
        values = list(map(float, file.read().split()))
    return rows, cols, values[: rows * cols]


"""
handler for slider to visit specific iteration
"""


def update_manual(val):
    global playing

    if playing:
        # slider "disabled" during auto play
        return

    index = int(val) - 1
    new_grid = values_grids[index]

    # check scale mode and update legend
    if use_global_scale:
        img.set_clim(vmin_global, vmax_global)
    else:
        vmin_local, vmax_local = new_grid.min(), new_grid.max()
        img.set_clim(vmin_local, vmax_local)

    img.set_array(new_grid)
    cbar.update_normal(img)
    ax.set_title(f"Simulation - iteration {index}")


"""
handler for automatic animation scale
"""


def update_auto(frame):
    img.set_array(values_grids[frame])

    # check scale mode and update legend
    if use_global_scale:
        img.set_clim(vmin_global, vmax_global)
    else:
        vmin_local, vmax_local = values_grids[frame].min(), values_grids[frame].max()
        img.set_clim(vmin_local, vmax_local)

    ax.set_title(f"Simulation - iteration {frame}")
    slider.set_val(frame)  # to sync slider with auto play


"""
handler for button player
"""


def toggle_animation(event):

    global playing

    if playing:
        ani.event_source.stop()
        button_play.label.set_text("Pause")
    else:
        ani.event_source.start()
        button_play.label.set_text("Play")

    playing = not playing


"""
handler for scale mode
"""


def toggle_scale(event):
    global use_global_scale

    button_scale.label.set_text("Local scale" if use_global_scale else "Global scale")
    use_global_scale = not use_global_scale

    index = int(slider.val) - 1
    new_grid = values_grids[index]

    if use_global_scale:
        img.set_clim(vmin_global, vmax_global)
    else:
        vmin_local, vmax_local = new_grid.min(), new_grid.max()
        img.set_clim(vmin_local, vmax_local)

    img.set_array(new_grid)
    cbar.update_normal(img)
    ax.set_title(f"Simulation - iteration {index}")


## ---------------------- MAIN ---------------------- ##

if len(sys.argv) < 3:
    print("Usage: ./isl_simulation.py <input> <iterations folder>")
    exit(0)

input = sys.argv[1]
iterations_dir = sys.argv[2]

# read initial configuration and its evolving through the iterations
rows, cols, initial_values = read_values(input)
initial_grid = np.array(initial_values).reshape((rows, cols))
files = sorted(
    [f for f in os.listdir(iterations_dir) if re.match(r"iteration\d+", f)],
    key=lambda x: int(re.search(r"\d+", x).group()),
)

# populate list of values to be used in heat map
values_grids = [initial_grid]
for file in files:
    _, _, values = read_values(os.path.join(iterations_dir, file))
    values_grids.append(np.array(values).reshape((rows, cols)))
num_frames = len(values_grids)

# global scale with minimum and maimum value read
vmin_global = min(grid.min() for grid in values_grids)
vmax_global = max(grid.max() for grid in values_grids)

# create heat map plot
fig, ax = plt.subplots(figsize=(8, 6))
plt.subplots_adjust(bottom=0.35)

# first frame to be displayed, initial configuration + global scale
current_grid = values_grids[0]
vmin_local, vmax_local = current_grid.min(), current_grid.max()
img = ax.imshow(current_grid, cmap="coolwarm", vmin=vmin_local, vmax=vmax_local)
ax.set_title(f"Simulation - iteration 1", fontsize=16)
cbar = fig.colorbar(img, ax=ax)

# slider for manual scrolling
ax_slider = plt.axes([0.15, 0.1, 0.50, 0.03])
slider = Slider(ax_slider, "Iteration", 1, num_frames, valinit=0, valstep=1)

# play button and scale type
ax_button_play = plt.axes([0.7, 0.1, 0.12, 0.05])
button_play = Button(ax_button_play, "Play")
playing = True

ax_button_scale = plt.axes([0.85, 0.1, 0.12, 0.05])
button_scale = Button(ax_button_scale, "Global scale")
use_global_scale = True

# set handlers
slider.on_changed(update_manual)
button_play.on_clicked(toggle_animation)
button_scale.on_clicked(toggle_scale)

# create animation of iterations
ani = FuncAnimation(fig, update_auto, frames=num_frames, interval=100, repeat=True)
plt.show()
