import cv2
import numpy as np
import matplotlib.pyplot as plt
import sys  # argv


def calc_hist(image_channel, size):
    hist = np.zeros(256, dtype=np.int32)
    for i in range(size):
        hist[image_channel[i]] += 1
    return hist
def auto_contrast(image_channel, clip_limit=0.01):
    # hist, bins = np.histogram(image_channel.flatten(), 256, [0, 256])
    hist = calc_hist(image_channel, 3072 * 4096)
    cdf = hist.cumsum()
    cdf_normalized = cdf / cdf.max()
    
    lower_bound = np.searchsorted(cdf_normalized, clip_limit)
    upper_bound = np.searchsorted(cdf_normalized, 1 - clip_limit)

    # print("lower_bound: ", lower_bound)
    # print("upper_bound: ", upper_bound)
    lut =  np.zeros(256, dtype=np.uint8)
    scale = 255 / (upper_bound - lower_bound)
    for i in range(256):
        if i < lower_bound:
            lut[i] = 0
        elif i > upper_bound:
            lut[i] = 255
        else:
            lut[i] = (i - lower_bound) * scale
    # lut = np.interp(np.arange(256), [lower_bound, upper_bound], [0, 255]).astype('uint8')
    # adjusted_image_channel = cv2.LUT(image_channel, lut)
    for i in range(3072 * 4096):
        image_channel[i] = lut[image_channel[i]]
    
    return image_channel


def main(image_path, clip_limit=0.01):
    with open(image_path, 'rb') as f:
        image = np.fromfile(f, dtype=np.uint8)
    original_image = image.copy()
    total_image_pixels_num = 4096 * 3072
    imga = np.zeros(total_image_pixels_num, dtype=np.uint8)
    imgr = np.zeros(total_image_pixels_num, dtype=np.uint8)
    imgg = np.zeros(total_image_pixels_num, dtype=np.uint8)
    imgb = np.zeros(total_image_pixels_num, dtype=np.uint8)
    for i in range(0, total_image_pixels_num):
        # abgr format
        # imga[i] = image[i * 4]
        # imgr[i] = image[i * 4 + 1]
        # imgg[i] = image[i * 4 + 2]
        # imgb[i] = image[i * 4 + 3]
        # argb format
        # imga[i] = image[i * 4]
        # imgr[i] = image[i * 4 + 1]
        # imgg[i] = image[i * 4 + 2]
        # imgb[i] = image[i * 4 + 3]
        # bgra format
        imgb[i] = image[i * 4]
        imgg[i] = image[i * 4 + 1]
        imgr[i] = image[i * 4 + 2]
        imga[i] = image[i * 4 + 3]

    # imga = auto_contrast(imga, clip_limit)
    imgr = auto_contrast(imgr, clip_limit)
    imgg = auto_contrast(imgg, clip_limit)
    imgb = auto_contrast(imgb, clip_limit)
    for i in range(0, total_image_pixels_num):
        # abgr format
        # image[i * 4] = imga[i]
        # image[i * 4 + 1] = imgr[i]
        # image[i * 4 + 2] = imgg[i]
        # image[i * 4 + 3] = imgb[i]
        # argb format
        # image[i * 4] = imga[i]
        # image[i * 4 + 1] = imgr[i]
        # image[i * 4 + 2] = imgg[i]
        # image[i * 4 + 3] = imgb[i]
        # opencv bgra format
        image[i * 4] = imgb[i]
        image[i * 4 + 1] = imgg[i]
        image[i * 4 + 2] = imgr[i]
        image[i * 4 + 3] = imga[i]
        # rgba format
        # image[i * 4] = imgb[i]
        # image[i * 4 + 1] = imgg[i]
        # image[i * 4 + 2] = imgr[i]
        # image[i * 4 + 3] = imga[i]

    plt.figure(figsize=(12, 6))

    # original image
    plt.subplot(1, 2, 1)
    original_image = cv2.cvtColor(original_image.reshape(3072, 4096, 4), cv2.COLOR_BGRA2RGBA)
    plt.imshow(original_image)
    plt.title("Original Image")
    plt.axis("off")

    # adjusted image
    plt.subplot(1, 2, 2)
    image = cv2.cvtColor(image.reshape(3072, 4096, 4), cv2.COLOR_BGRA2RGBA)
    plt.imshow(image)
    plt.title("Adjusted Image")
    plt.axis("off")

    plt.tight_layout()
    plt.show()

    image.tofile("adjusted_image.bin")
    

        


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