import cv2
import numpy as np
import matplotlib.pyplot as plt
import sys
import os

def apply_white_balance(image):
    wb = cv2.xphoto.createSimpleWB()
    return wb.balanceWhite(image)

def plot_histogram(image, title="Histogram", position=122):
    plt.subplot(position)
    plt.title(title)
    plt.xlabel("Pixel Value")
    plt.ylabel("Frequency")

    if len(image.shape) == 2:
        hist = cv2.calcHist([image], [0], None, [256], [0, 256])
        plt.plot(hist, color='black')
    else:
        colors = ('r', 'g', 'b')
        for i, color in enumerate(colors):
            hist = cv2.calcHist([image], [i], None, [256], [0, 256])
            plt.plot(hist, color=color)
    plt.xlim([0, 256])

def auto_contrast(image, clip_limit=0.01):
    channels = cv2.split(image)
    adj_channels = []

    for ch in channels:
        hist, _ = np.histogram(ch.flatten(), 256, [0, 256])
        cdf = hist.cumsum()
        cdf_normalized = cdf / cdf.max()

        lower_bound = np.searchsorted(cdf_normalized, clip_limit)
        upper_bound = np.searchsorted(cdf_normalized, 1 - clip_limit)

        if lower_bound >= upper_bound:
            return image  

        lut = np.interp(np.arange(256), [lower_bound, upper_bound], [0, 255]).astype('uint8')
        adj_channels.append(cv2.LUT(ch, lut))

    adjusted_image = cv2.merge(adj_channels)
    return adjusted_image

def main(image_path, clip_limit=0.01):
    image = cv2.imread(image_path)
    if image is None:
        print(f"Error: Cannot open image {image_path}")
        sys.exit(1)
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

    # Apply white balance correction
    image = apply_white_balance(image)

    adjusted_image = auto_contrast(image, clip_limit)

    plt.figure(figsize=(12, 6))

    plt.subplot(221)
    plt.imshow(image)
    plt.title("White Balanced Image")
    plt.axis("off")
    plot_histogram(image, "White Balance Histogram", 222)

    plt.subplot(223)
    plt.imshow(adjusted_image)
    plt.title("Auto-Contrast Image")
    plt.axis("off")
    plot_histogram(adjusted_image, "Auto-Contrast Histogram", 224)

    plt.tight_layout()
    plt.show()

    output_filename = os.path.splitext(image_path)[0] + "_wb_hist_corrected.tiff"
    adjusted_image = cv2.cvtColor(adjusted_image, cv2.COLOR_RGB2BGR)
    cv2.imwrite(output_filename, adjusted_image)
    print(f"Saved adjusted image as {output_filename}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python main.py <image_path>")
        sys.exit(1)

    image_path = sys.argv[1]
    clip_limit = 0.005

    main(image_path, clip_limit)
