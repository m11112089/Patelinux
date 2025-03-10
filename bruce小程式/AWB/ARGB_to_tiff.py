from PIL import Image
import numpy as np
import sys

def argb_raw_to_tiff(input_path, output_path, width, height):
    # Read ARGB raw file
    with open(input_path, 'rb') as f:
        raw_data = np.fromfile(f, dtype=np.uint8)
    
    # Check if file size matches expected size
    expected_size = width * height * 4  # 4 bytes per pixel (ARGB)
    if len(raw_data) != expected_size:
        print(f"Error: File size ({len(raw_data)}) does not match expected size ({expected_size})")
        sys.exit(1)
    
    # Reshape to image (height, width, 4) - ARGB
    argb_array = raw_data.reshape((height, width, 4))
    
    # Reorder from ARGB to RGBA for Pillow
    rgba_array = np.zeros((height, width, 4), dtype=np.uint8)
    rgba_array[:, :, 0] = argb_array[:, :, 1]  # Red
    rgba_array[:, :, 1] = argb_array[:, :, 2]  # Green
    rgba_array[:, :, 2] = argb_array[:, :, 3]  # Blue
    rgba_array[:, :, 3] = argb_array[:, :, 0]  # Alpha
    
    # Convert to PIL Image
    img = Image.fromarray(rgba_array, mode='RGBA')
    
    # Save as TIFF
    img.save(output_path, format='TIFF')
    
    print(f"Converted ARGB RAW to TIFF: {output_path}")
    print(f"Width: {width}, Height: {height}")

if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("Usage: python script.py <input_raw_path> <width> <height> <output_tiff_path>")
        sys.exit(1)
    
    input_path = sys.argv[1]
    width = int(sys.argv[2])
    height = int(sys.argv[3])
    output_path = sys.argv[4]
    
    argb_raw_to_tiff(input_path, output_path, width, height)