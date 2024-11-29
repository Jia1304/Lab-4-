#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define WIDTH 512
#define HEIGHT 512
#define PIXEL_COUNT (WIDTH * HEIGHT)

typedef unsigned char pixel_t;

// Function prototypes
int readPGMText(const char *filename, pixel_t *pixels, int width, int height);
int writePGMText(const char *filename, const pixel_t *pixels, int width, int height);
int writePGMBinary(const char *filename, const pixel_t *pixels, int width, int height);
void embedLSB(pixel_t *coverPixels, const pixel_t *secretPixels, int width, int height);
void extractLSB(const pixel_t *stegoPixels, pixel_t *outputPixels, int width, int height);

// Error handling function
void handleError(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    if (errno != 0) {
        fprintf(stderr, "System error: %s\n", strerror(errno));
    }
    exit(EXIT_FAILURE);
}

int main() {
    const char *cover_image = "baboon.pgm";
    const char *secret_image = "farm.pgm";
    const char *stego_image = "stego_image_bin.pgm";
    const char *extracted_secret = "extracted_secret.pgm";

    pixel_t *coverPixels = malloc(PIXEL_COUNT * sizeof(pixel_t));
    pixel_t *secretPixels = malloc(PIXEL_COUNT * sizeof(pixel_t));
    pixel_t *outputPixels = malloc(PIXEL_COUNT * sizeof(pixel_t));

    if (!coverPixels || !secretPixels || !outputPixels) {
        handleError("Memory allocation failed");
    }

    if (readPGMText(cover_image, coverPixels, WIDTH, HEIGHT) != 0 ||
        readPGMText(secret_image, secretPixels, WIDTH, HEIGHT) != 0) {
        handleError("Failed to read input images");
    }

    embedLSB(coverPixels, secretPixels, WIDTH, HEIGHT);

    if (writePGMBinary(stego_image, coverPixels, WIDTH, HEIGHT) != 0) {
        handleError("Failed to write stego image");
    }

    extractLSB(coverPixels, outputPixels, WIDTH, HEIGHT);

    if (writePGMText(extracted_secret, outputPixels, WIDTH, HEIGHT) != 0) {
        handleError("Failed to write extracted secret image");
    }

    free(coverPixels);
    free(secretPixels);
    free(outputPixels);

    printf("Steganography process completed successfully.\n");
    return EXIT_SUCCESS;
}

// Function to read a PGM image in text format.
// 'pixels' stores the pixel values of the image specified by 'filename'.
int readPGMText(const char *filename, pixel_t *pixels, int width, int height) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        handleError("Error opening file for reading");
    }

    char buffer[16];
    if (fgets(buffer, sizeof(buffer), file) == NULL || buffer[0] != 'P' || buffer[1] != '2') {
        fclose(file);
        handleError("Invalid PGM format");
    }

    int imgWidth, imgHeight, maxGray;
    while (fgets(buffer, sizeof(buffer), file) && buffer[0] == '#');
    if (sscanf(buffer, "%d %d", &imgWidth, &imgHeight) != 2 ||
        imgWidth != width || imgHeight != height) {
        fclose(file);
        handleError("Invalid image dimensions");
    }

    if (fscanf(file, "%d", &maxGray) != 1 || maxGray != 255) {
        fclose(file);
        handleError("Unsupported max gray value");
    }

    for (int i = 0; i < width * height; ++i) {
        if (fscanf(file, "%hhu", &pixels[i]) != 1) {
            fclose(file);
            handleError("Error reading pixel data");
        }
    }

    fclose(file);
    return 0;
}

// Function to write a PGM image in text format.
// The dereferenced values of 'pixels' are stored in the PGM image specified by 'filename'.
int writePGMText(const char *filename, const pixel_t *pixels, int width, int height) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        handleError("Error opening file for writing");
    }

    fprintf(file, "P2\n%d %d\n255\n", width, height);
    for (int i = 0; i < width * height; ++i) {
        fprintf(file, "%hhu\n", pixels[i]);
    }

    fclose(file);
    return 0;
}

// Function to write a PGM image in binary format.
// The dereferenced values of 'pixels' are stored in the PGM image specified by 'filename' in binary format.
int writePGMBinary(const char *filename, const pixel_t *pixels, int width, int height) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        handleError("Error opening file for writing");
    }

    fprintf(file, "P5\n%d %d\n255\n", width, height);
    if (fwrite(pixels, sizeof(pixel_t), width * height, file) != width * height) {
        fclose(file);
        handleError("Error writing pixel data");
    }

    fclose(file);
    return 0;
}

// Function to hide a secret image using the 4-bit LSB steganography algorithm.
// The dereferenced values of 'coverPixels' (cover image) are updated with the dereferenced values of
// 'secretPixels' (secret image) based on the 4-bit LSB steganography algorithm
void embedLSB(pixel_t *coverPixels, const pixel_t *secretPixels, int width, int height) {
    for (int i = 0; i < width * height; ++i) {
        coverPixels[i] = (coverPixels[i] & 0xF0) | (secretPixels[i] >> 4);
    }
}

// Function to extract the secret image using 4-LSB steganography algorithm.
// The extracted pixel values, derived from the stego image referenced by 'coverPixels',
// are stored in the memory space pointed to by 'outputPixels'
void extractLSB(const pixel_t *stegoPixels, pixel_t *outputPixels, int width, int height) {
    for (int i = 0; i < width * height; ++i) {
        outputPixels[i] = (stegoPixels[i] & 0x0F) << 4;
    }
}
