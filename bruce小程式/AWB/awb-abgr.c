#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define HIST_SIZE 256

// Structure for ARGB image data
typedef struct {
    uint8_t* a;
    uint8_t* r;
    uint8_t* g;
    uint8_t* b;
    int width;
    int height;
} ARGBImage;

// Function to calculate histogram (unchanged)
void calc_histogram(uint8_t* channel, int size, uint32_t* hist) {
    for (int i = 0; i < HIST_SIZE; i++) {
        hist[i] = 0;
    }
    for (int i = 0; i < size; i++) {
        hist[channel[i]]++;
    }
}

// Function to calculate cumulative distribution function (unchanged)
void calc_cdf(uint32_t* hist, float* cdf, int size) {
    uint32_t sum = 0;
    for (int i = 0; i < HIST_SIZE; i++) {
        sum += hist[i];
        cdf[i] = (float)sum / size;
    }
}

// Auto-contrast function (unchanged)
void auto_contrast_channel(uint8_t* channel, int size, float clip_limit, uint8_t* output) {
    uint32_t hist[HIST_SIZE];
    float cdf[HIST_SIZE];
    
    calc_histogram(channel, size, hist);
    calc_cdf(hist, cdf, size);
    
    int lower_bound = 0;
    int upper_bound = 255;
    for (int i = 0; i < HIST_SIZE; i++) {
        if (cdf[i] >= clip_limit) {
            lower_bound = i;
            break;
        }
    }
    for (int i = HIST_SIZE - 1; i >= 0; i--) {
        if (cdf[i] <= (1.0f - clip_limit)) {
            upper_bound = i;
            break;
        }
    }
    
    uint8_t lut[HIST_SIZE];
    float scale = 255.0f / (upper_bound - lower_bound);
    for (int i = 0; i < HIST_SIZE; i++) {
        if (i <= lower_bound) {
            lut[i] = 0;
        } else if (i >= upper_bound) {
            lut[i] = 255;
        } else {
            lut[i] = (uint8_t)((i - lower_bound) * scale);
        }
    }
    
    for (int i = 0; i < size; i++) {
        output[i] = lut[channel[i]];
    }
}

// Main processing function
int process_image(const char* input_path, const char* output_path, int width, int height, float clip_limit) {
    // Calculate sizes
    int pixel_count = width * height;
    int size = pixel_count * 4;  // 4 bytes per pixel for both input (ABGR) and output (ARGB)
    
    // Allocate memory
    uint8_t* input_data = (uint8_t*)malloc(size);
    if (!input_data) {
        printf("Failed to allocate input buffer\n");
        return 1;
    }
    
    // Read input file
    FILE* fp = fopen(input_path, "rb");
    if (!fp) {
        printf("Failed to open input file\n");
        free(input_data);
        return 1;
    }
    
    if (fread(input_data, 1, size, fp) != size) {
        printf("Failed to read input file\n");
        fclose(fp);
        free(input_data);
        return 1;
    }
    fclose(fp);
    
    // Separate channels
    ARGBImage img;
    img.width = width;
    img.height = height;
    img.a = (uint8_t*)malloc(pixel_count);
    img.r = (uint8_t*)malloc(pixel_count);
    img.g = (uint8_t*)malloc(pixel_count);
    img.b = (uint8_t*)malloc(pixel_count);
    
    if (!img.a || !img.r || !img.g || !img.b) {
        printf("Failed to allocate channel buffers\n");
        free(input_data);
        free(img.a);
        free(img.r);
        free(img.g);
        free(img.b);
        return 1;
    }
    
    // Extract ABGR channels (input is ABGR)
    for (int i = 0, j = 0; i < pixel_count; i++, j += 4) {
        img.a[i] = input_data[j];     // A (byte 0)
        img.b[i] = input_data[j + 1]; // B (byte 1)
        img.g[i] = input_data[j + 2]; // G (byte 2)
        img.r[i] = input_data[j + 3]; // R (byte 3)
    }
    
    // Process RGB channels (leaving alpha unchanged)
    uint8_t* out_r = (uint8_t*)malloc(pixel_count);
    uint8_t* out_g = (uint8_t*)malloc(pixel_count);
    uint8_t* out_b = (uint8_t*)malloc(pixel_count);
    
    if (!out_r || !out_g || !out_b) {
        printf("Failed to allocate output buffers\n");
        goto cleanup;
    }
    
    auto_contrast_channel(img.r, pixel_count, clip_limit, out_r);
    auto_contrast_channel(img.g, pixel_count, clip_limit, out_g);
    auto_contrast_channel(img.b, pixel_count, clip_limit, out_b);
    
    // Combine channels into output buffer including alpha (output is ARGB)
    uint8_t* output_data = (uint8_t*)malloc(size);
    if (!output_data) {
        printf("Failed to allocate output buffer\n");
        goto cleanup;
    }
    
    for (int i = 0, j = 0; i < pixel_count; i++, j += 4) {
        output_data[j] = img.a[i];    // A (byte 0)
        output_data[j + 1] = out_r[i]; // R (byte 1)
        output_data[j + 2] = out_g[i]; // G (byte 2)
        output_data[j + 3] = out_b[i]; // B (byte 3)
    }
    
    // Write output file
    fp = fopen(output_path, "wb");
    if (!fp) {
        printf("Failed to open output file\n");
        goto cleanup;
    }
    
    if (fwrite(output_data, 1, size, fp) != size) {
        printf("Failed to write output file\n");
    }
    fclose(fp);
    
cleanup:
    free(input_data);
    free(img.a);
    free(img.r);
    free(img.g);
    free(img.b);
    free(out_r);
    free(out_g);
    free(out_b);
    free(output_data);
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Usage: %s <input_path> <output_path> <width> <height>\n", argv[0]);
        return 1;
    }
    
    const char* input_path = argv[1];
    const char* output_path = argv[2];
    int width = atoi(argv[3]);
    int height = atoi(argv[4]);
    float clip_limit = 0.005f;
    
    return process_image(input_path, output_path, width, height, clip_limit);
}