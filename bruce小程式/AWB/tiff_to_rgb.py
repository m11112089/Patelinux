from PIL import Image
import numpy as np
import sys

def tiff_to_argb_raw(tiff_path, output_path):
    # Open the TIFF image
    try:
        img = Image.open(tiff_path)
    except Exception as e:
        print(f"Error opening TIFF file: {e}")
        sys.exit(1)

    # Convert to numpy array
    img_array = np.array(img)

    # Check the number of channels
    if len(img_array.shape) != 3 or img_array.shape[2] not in [3, 4]:
        print("Error: Input TIFF must be RGB or RGBA")
        sys.exit(1)

    height, width = img_array.shape[0], img_array.shape[1]

    # If RGB (3 channels), add an alpha channel
    if img_array.shape[2] == 3:
        # Create a fully opaque alpha channel (255)
        alpha = np.full((height, width), 255, dtype=np.uint8)
        # Stack alpha with RGB to make ARGB
        argb_array = np.dstack((alpha, img_array[:, :, 0], img_array[:, :, 1], img_array[:, :, 2]))
    else:
        # If already RGBA, reorder to ARGB
        argb_array = np.zeros((height, width, 4), dtype=np.uint8)
        argb_array[:, :, 0] = img_array[:, :, 3]  # Alpha
        argb_array[:, :, 1] = img_array[:, :, 0]  # Red
        argb_array[:, :, 2] = img_array[:, :, 1]  # Green
        argb_array[:, :, 3] = img_array[:, :, 2]  # Blue

    # Save as raw file
    with open(output_path, 'wb') as f:
        argb_array.tofile(f)

    print(f"Converted TIFF to ARGB RAW: {output_path}")
    print(f"Width: {width}, Height: {height}")
    print(f"File size: {width * height * 4} bytes")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py <input_tiff_path> <output_raw_path>")
        sys.exit(1)

    tiff_path = sys.argv[1]
    output_path = sys.argv[2]

    tiff_to_argb_raw(tiff_path, output_path)