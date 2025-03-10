import cv2
import numpy as np
import matplotlib.pyplot as plt
import sys  # argv

def plot_histogram(image, title="Histogram", position=122):
    colors = ('r', 'g', 'b')
    plt.subplot(position)
    plt.title(title)
    plt.xlabel("Pixel Value")
    plt.ylabel("Frequency")
    for i, color in enumerate(colors):
        hist = cv2.calcHist([image], [i], None, [256], [0, 256])
        plt.plot(hist, color=color)
        plt.xlim([0, 256])

def auto_contrast(image, clip_limit=0.01):
    channels = cv2.split(image)
    adj_channels = []
    for ch in channels:
        hist, bins = np.histogram(ch.flatten(), 256, [0, 256])
        cdf = hist.cumsum()
        cdf_normalized = cdf / cdf.max()
        
        lower_bound = np.searchsorted(cdf_normalized, clip_limit)
        upper_bound = np.searchsorted(cdf_normalized, 1 - clip_limit)
        
        lut = np.interp(np.arange(256), [lower_bound, upper_bound], [0, 255]).astype('uint8')
        adj_channels.append(cv2.LUT(ch, lut))
    
    adjusted_image = cv2.merge(adj_channels)
    return adjusted_image


C_PATTERN_GEN=0
def main(image_path, clip_limit=0.01):
    if (C_PATTERN_GEN == 0) :
        image = cv2.imread(image_path)
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    else :
    # Test
        imageWidth=640
        imageHeight=480
        
        mean_read = 12      # Mean (μ)
        mean_green = 30      # Mean (μ)
        mean_blue = 178      # Mean (μ)
        # std_dev = 1   # Standard deviation (σ)
        # num_samples = imageWidth * imageHeight  # Number of samples
        
        redNp=np.random.normal(mean_read, np.sqrt(mean_read), (imageHeight, imageWidth))
        greenNp=np.random.normal(mean_green, np.sqrt(mean_green), (imageHeight, imageWidth))
        blueNp=np.random.normal(mean_blue, np.sqrt(mean_blue), (imageHeight, imageWidth))
        
        
        
        redNp = redNp.astype(np.uint8)
        greenNp = greenNp.astype(np.uint8)
        blueNp = blueNp.astype(np.uint8)
        
        image = np.dstack((redNp, greenNp, blueNp))
    
    
    adjusted_image = auto_contrast(image, clip_limit)
    
    plt.figure(figsize=(12, 6))
    
    # Original Image and Histogram
    plt.subplot(221)
    plt.imshow(image)
    plt.title("Original Image")
    plt.axis("off")
    plot_histogram(image, "Original Histogram", 222)
    
    # Adjusted Image and Histogram
    plt.subplot(223)
    plt.imshow(adjusted_image)
    plt.title("Auto-Contrast Image")
    plt.axis("off")
    plot_histogram(adjusted_image, "Auto-Contrast Histogram", 224)
    
    plt.tight_layout()
    plt.show()
    
    
    adjusted_image=cv2.cvtColor(adjusted_image, cv2.COLOR_RGB2BGR)
    cv2.imwrite("hist_cor.png", adjusted_image)

if __name__ == "__main__":
    # receive argv parameters

    if len(sys.argv) == 2:
        image_path = sys.argv[1]
    else:
        print("Usage: python main.py <image_path>")
        sys.exit(1)
    # image_path = "H:\\20250123_LL2_Focus\\20250204\\2_101_Bot_Out\\20250204-140034_RGB.tiff"  # 替換為你的圖片路徑
    # image_path = "H:\\20250123_LL2_Focus\\20250204\\3_101_Top_Out\\20250204-140457_RGB.tiff"  # 替換為你的圖片路徑
    # image_path = "H:\\20250123_LL2_Focus\\20250122\\9_10ms_950um_center\\20250122-165247_RGB.tiff"  # 替換為你的圖片路徑
    # image_path = "H:\\20250123_LL2_Focus\\20250122\\13_42ms_950um_lower_left\\20250122-172514_RGB.tiff"  # 替換為你的圖片路徑
    # image_path = "D:\\project\\pyproject\\Estimation\\11.tif"  # 替換為你的圖片路徑
    # image_path = "D:\\project\\pyproject\\Estimation\\0219.png"  # 替換為你的圖片路徑
    
    
    
    clip_limit = 0.005  # 設定要砍掉的百分比
    main(image_path, clip_limit)