#ifndef BMP_H
#define BMP_H

#include <GL/glut.h> // Include OpenGL headers
#include <stdio.h>
#include <stdlib.h>

// BMP header structures
#pragma pack(push, 1) // Ensure no padding in structures

typedef struct {
    unsigned short bfType;      // BMP file type
    unsigned int bfSize;       // Size of the BMP file
    unsigned short bfReserved1; // Reserved
    unsigned short bfReserved2; // Reserved
    unsigned int bfOffBits;    // Offset to the pixel data
} BMPHeader;

typedef struct {
    unsigned int biSize;       // Size of this header
    int biWidth;               // Width of the bitmap
    int biHeight;              // Height of the bitmap
    unsigned short biPlanes;    // Number of color planes
    unsigned short biBitCount;  // Number of bits per pixel
    unsigned int biCompression; // Compression type
    unsigned int biSizeImage;   // Size of the image data
    int biXPelsPerMeter;        // Horizontal resolution
    int biYPelsPerMeter;        // Vertical resolution
    unsigned int biClrUsed;     // Number of colors in the color palette
    unsigned int biClrImportant; // Important colors
} BMPInfoHeader;

#pragma pack(pop)

// Function to load a BMP file
GLuint loadBMP(const char* filePath);

#endif // BMP_H