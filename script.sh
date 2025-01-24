#!/bin/bash

# Create the images directory if it doesn't exist

# Take a picture with fswebcam and save it under the images folder
fswebcam -r 1280x720 --jpeg 85 -D 1 images/captured_image.jpg

# Check if the picture was taken successfully

# Call the main.out executable
./main.out

rm images/captured_image.jpg