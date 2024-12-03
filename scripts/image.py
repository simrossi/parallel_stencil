#!/usr/bin/env python3
import os
import sys
import subprocess
from PIL import Image

binary = input_img = output_dir = None
red_file = green_file = blue_file = None

def main():
    if len(sys.argv) != 4:
        print("Usage: ./image.py <stencil binary> <input image> <output directory>")

    global binary, input_img, output_dir
    binary = sys.argv[1];
    input_img = sys.argv[2];
    output_dir = sys.argv[3];

    imageToFiles()
    computeStencil()
    filesToImage()


def imageToFiles():
    image = Image.open(input_img)
    image = image.convert('RGB')
    
    width, height = image.size
    r_channel = []
    g_channel = []
    b_channel = []
    
    for y in range(height):
        for x in range(width):
            r, g, b = image.getpixel((x, y))
            r_channel.append(r)
            g_channel.append(g)
            b_channel.append(b)
    
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    # Write RGB data to three separate binary files
    def write_channel(file_name, channel_data):
        with open(file_name, 'w') as file:

            file.write(f'{width} {height}\n')
            file.write(' '.join(map(str, channel_data)) + '\n')
    
    global red_file, green_file, blue_file
    red_file = os.path.join(output_dir, input_img + '.red')
    green_file = os.path.join(output_dir, input_img + '.green')
    blue_file = os.path.join(output_dir, input_img + '.blue')

    # Save the RGB channels into separate files
    write_channel(red_file, r_channel)
    write_channel(green_file, g_channel)
    write_channel(blue_file, b_channel)


def computeStencil():
    subprocess.run([binary, '-i', red_file, '-o', red_file])
    subprocess.run([binary, '-i', green_file, '-o',green_file])
    subprocess.run([binary, '-i', blue_file, '-o',blue_file])


def filesToImage():
    def read_channel(file_name):
        with open(file_name, 'r') as file:
            width, height = map(int, file.readline().split())
            channel_data = [int(float(x)) for line in file for x in line.split()]
        return width, height, channel_data

    # Read the RGB channels
    r_width, r_height, r_channel = read_channel(red_file)
    g_width, g_height, g_channel = read_channel(green_file)
    b_width, b_height, b_channel = read_channel(blue_file)

    # Ensure the dimensions match
    if r_width != g_width or g_width != b_width or r_height != g_height or g_height != b_height:
        print("Image dimensions do not match between channels.")

    image = Image.new('RGB', (r_width, r_height))
    pixels = []
    for i in range(len(r_channel)):
        pixels.append((r_channel[i], g_channel[i], b_channel[i]))

    image.putdata(pixels)
    image.save(os.path.join(output_dir, input_img))


if __name__ == "__main__":
    main()
