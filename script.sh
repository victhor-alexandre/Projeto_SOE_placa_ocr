#!/bin/bash

# Function to read distance from the ultrasonic sensor
read_distance() {
    # Replace with the actual command or code to read the distance from your sensor
    # This is a placeholder command
    python3 read_distance.py
}

# Create the images directory if it doesn't exist
mkdir -p images

# Wait for the ultrasonic sensor signal
while true; do
    distance=$(read_distance)
    echo "Distance: $distance cm"
    if (( $(echo "$distance <= 50" | bc -l) )); then
        echo "Distance is less than or equal to 50 cm. Taking pictures..."
        break
    fi
    sleep 1
done

# Take four pictures with fswebcam and save them under the images folder
for i in {1..4}; do
    fswebcam -r 1280x720 --jpeg 85 -D 1 images/captured_image_$i.jpg
done

# Check if the pictures were taken successfully
for i in {1..4}; do
    if [ -f images/captured_image_$i.jpg ]; then
        echo "Picture $i taken successfully and saved to images/captured_image_$i.jpg"
    else
        echo "Failed to take picture $i"
        exit 1
    fi
done

# Call the main.out executable
./main.out

# Remove the captured images
rm images/captured_image_*.jpg


# End of script.sh