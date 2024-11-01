#include "bmp.h"

GLuint loadBMP(const char* filePath) {
    BMPHeader bmpHeader;
    BMPInfoHeader bmpInfoHeader;

    // Open the BMP file
    FILE* file = fopen(filePath, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open BMP file: %s\n", filePath);
        return 0;
    }

    // Read BMP header
    fread(&bmpHeader, sizeof(BMPHeader), 1, file);
    if (bmpHeader.bfType != 0x4D42) { // Check if it's a BMP file
        fprintf(stderr, "Not a BMP file: %s\n", filePath);
        fclose(file);
        return 0;
    }

    // Read BMP info header
    fread(&bmpInfoHeader, sizeof(BMPInfoHeader), 1, file);

    // Allocate memory for pixel data
    unsigned char* image = NULL;
    int bytesPerPixel = bmpInfoHeader.biBitCount / 8;

    // Calculate image size if needed
    if (bmpInfoHeader.biSizeImage == 0) {
        bmpInfoHeader.biSizeImage = bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * bytesPerPixel;
    }

    // Allocate memory based on the bit count
    image = (unsigned char*)malloc(bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * bytesPerPixel);
    if (!image) {
        fprintf(stderr, "Failed to allocate memory for image\n");
        fclose(file);
        return 0;
    }

    // Move file pointer to the pixel data
    fseek(file, bmpHeader.bfOffBits, SEEK_SET);

    // Read pixel data
    for (int y = 0; y < bmpInfoHeader.biHeight; ++y) {
        fread(image + (bmpInfoHeader.biWidth * (bmpInfoHeader.biHeight - 1 - y) * bytesPerPixel), bmpInfoHeader.biWidth * bytesPerPixel, 1, file);
        // Skip padding
        fseek(file, (4 - (bmpInfoHeader.biWidth * bytesPerPixel) % 4) % 4, SEEK_CUR);
    }
    fclose(file);

    // If 32-bit, convert to 24-bit (discard alpha)
    if (bytesPerPixel == 4) {
        unsigned char* tempImage = (unsigned char*)malloc(bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * 3);
        for (int i = 0; i < bmpInfoHeader.biWidth * bmpInfoHeader.biHeight; ++i) {
            tempImage[i * 3 + 0] = image[i * 4 + 2]; // R
            tempImage[i * 3 + 1] = image[i * 4 + 1]; // G
            tempImage[i * 3 + 2] = image[i * 4 + 0]; // B
        }
        free(image);
        image = tempImage;
    }
    // Handle 8-bit BMP
    else if (bytesPerPixel == 1) {
        // Read the color palette
        unsigned char palette[256][4]; // 256 colors + alpha
        fread(palette, sizeof(unsigned char), 256 * 4, file); // Read the palette

        unsigned char* tempImage = (unsigned char*)malloc(bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * 3);
        for (int y = 0; y < bmpInfoHeader.biHeight; ++y) {
            for (int x = 0; x < bmpInfoHeader.biWidth; ++x) {
                unsigned char index = image[y * bmpInfoHeader.biWidth + x];
                tempImage[(y * bmpInfoHeader.biWidth + x) * 3 + 0] = palette[index][2]; // R
                tempImage[(y * bmpInfoHeader.biWidth + x) * 3 + 1] = palette[index][1]; // G
                tempImage[(y * bmpInfoHeader.biWidth + x) * 3 + 2] = palette[index][0]; // B
            }
        }
        free(image);
        image = tempImage;
        bytesPerPixel = 3; // Update bytesPerPixel for OpenGL upload
    }

    // Generate texture ID
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, (bytesPerPixel == 4) ? GL_RGBA : GL_RGB, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight, 0, (bytesPerPixel == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);

    // Clean up
    free(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}