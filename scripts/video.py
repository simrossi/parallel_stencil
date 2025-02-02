#!/usr/bin/env python3
import os
import subprocess
from PIL import Image
import sys
import cv2

'''
extract the frames from the video getting as
result a list of images
'''
def extract_frames(video_path, output_dir, max_frames):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    cap = cv2.VideoCapture(video_path)
    frame_count = 0

    while cap.isOpened() and frame_count < max_frames:
        ret, frame = cap.read()
        if not ret:
            break
        frame_path = os.path.join(output_dir, f"frame_{frame_count:03d}.png")
        cv2.imwrite(frame_path, frame)
        frame_count += 1

    cap.release()
    print(f"Extracted {frame_count} frames")
    return frame_count

'''
as each frame is an image, it is possible to apply the stencil on it
'''
def apply_stencil_to_frames(input_dir, output_dir, stencil_binary):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    for frame_file in sorted(os.listdir(input_dir)):
        input_path = os.path.join(input_dir, frame_file)
        output_path = os.path.join(output_dir, frame_file)
        process_frame_with_stencil(input_path, output_path, stencil_binary)

def process_frame_with_stencil(input_img, output_img, stencil_binary):
    #in a similar way done for the images, we have to extract the 3 channels of each frame
    image = Image.open(input_img).convert("RGB")
    width, height = image.size

    r_channel, g_channel, b_channel = [], [], []
    for y in range(height):
        for x in range(width):
            r, g, b = image.getpixel((x, y))
            r_channel.append(r)
            g_channel.append(g)
            b_channel.append(b)

    #cycle in temporary channel files in order to save memory space
    red_file, green_file, blue_file = "temp.red", "temp.green", "temp.blue"
    save_channel(red_file, r_channel, width, height)
    save_channel(green_file, g_channel, width, height)
    save_channel(blue_file, b_channel, width, height)

    subprocess.run([stencil_binary, "-i", red_file, "-o", red_file])
    subprocess.run([stencil_binary, "-i", green_file, "-o", green_file])
    subprocess.run([stencil_binary, "-i", blue_file, "-o", blue_file])

    #reconstruct the image after applying the stencil
    new_image = reconstruct_image(red_file, green_file, blue_file, width, height)
    new_image.save(output_img)

    os.remove(red_file)
    os.remove(green_file)
    os.remove(blue_file)

def save_channel(file_name, channel_data, width, height):
    with open(file_name, "w") as file:
        file.write(f"{width} {height}\n")
        file.write(" ".join(map(str, channel_data)) + "\n")

def reconstruct_image(red_file, green_file, blue_file, width, height):
    def read_channel(file_name):
        with open(file_name, "r") as file:
            file.readline()
            return [int(float(x)) for x in file.read().split()]

    r_channel = read_channel(red_file)
    g_channel = read_channel(green_file)
    b_channel = read_channel(blue_file)

    image = Image.new("RGB", (width, height))
    pixels = [(r_channel[i], g_channel[i], b_channel[i]) for i in range(len(r_channel))]
    image.putdata(pixels)
    return image

'''
do the inverse procedure of extracting the frames so, generate a new video
from the new frames computed in a mp4 format
'''
def create_video_from_frames_ffmpeg(frame_dir, output_video, fps=30):
    frame_pattern = os.path.join(frame_dir, "frame_%03d.png")
    
    #FFmpeg comand to create videos
    command = [
        'ffmpeg',
        '-framerate', str(fps),
        '-i', frame_pattern,
        '-c:v', 'libx264',
        '-pix_fmt', 'yuv420p',
        output_video
    ]
    
    subprocess.run(command)
    print(f"Video generated: {output_video}")

'''
This script takes as input a video and applies to it the defined stencil.
It will generate as output a new video, different from the previous one based on the structure
and properties of the stencil used.

The idea is the same used for images so, it is important to use as input (if possible) square videos.
When proportions are not uniform, the filter's operation could produce artifacts
or noise because the dimension of the stencil doesn't correspond to the spacial structure of the frames.
'''
if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("Use: python3 video.py <stencil_binary> <video_path> <output_dir> <video name output>")
        sys.exit(1)

    stencil_binary = sys.argv[1]
    video_path = sys.argv[2]
    output_frames = os.path.join(sys.argv[3], "frames")
    processed_frames = os.path.join(sys.argv[3], "processed_frames")
    output_video = sys.argv[4]
    max_frames = 1000#if the video has too much frames you can say how many you want to compute
                    #if you set a large number of frames it could be that all the video is going to be
                    #computed

    print("Extraction of frames")
    extract_frames(video_path, output_frames, max_frames)

    print("Applying stencil")
    apply_stencil_to_frames(output_frames, processed_frames, stencil_binary)

    print("Generate video")
    create_video_from_frames_ffmpeg(processed_frames, output_video)

    print("Done")
