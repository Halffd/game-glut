#include "bmp.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Global variables for the main square
GLuint textureID;
int mouseX, mouseY;
bool isMousePressed = false;
bool isDragging = false;
bool autoMove = false;

// Square dimensions and movement
float squareSize = 50.0f;
float squareX = 200.0f, squareY = 200.0f;
float squareVelocityX = 5.0f, squareVelocityY = 5.0f;

// Color values
float red = 1.0f, green = 1.0f, blue = 1.0f;

// Structure to hold square data
typedef struct {
    float x, y;
    float size;
    float velocityX, velocityY;
    float red, green, blue;
} Square;

Square squares[10]; // Array of squares
int squareCount = 1; // Start with one square

void drawSquare(Square square) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glColor3f(square.red, square.green, square.blue); // Set the color
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(square.x, square.y);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(square.x + square.size, square.y);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(square.x + square.size, square.y + square.size);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(square.x, square.y + square.size);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw all squares
    for (int i = 0; i < squareCount; i++) {
        drawSquare(squares[i]);
    }

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y) {
    // Check if mouse is inside any square
    for (int i = 0; i < squareCount; i++) {
        if (x >= squares[i].x && x <= squares[i].x + squares[i].size &&
            y >= squares[i].y && y <= squares[i].y + squares[i].size) {
            if (button == GLUT_LEFT_BUTTON) {
                if (state == GLUT_DOWN) {
                    isMousePressed = true;
                    isDragging = true;
                    mouseX = x;
                    mouseY = y;
                } else if (state == GLUT_UP) {
                    isMousePressed = false;
                    isDragging = false;
                }
            } else if (button == GLUT_RIGHT_BUTTON) {
                static int lastClickTime = 0;
                int currentTime = glutGet(GLUT_ELAPSED_TIME);
                if (currentTime - lastClickTime < 300) { // Double click detected
                    squares[i].velocityX *= 1.5f;
                    squares[i].velocityY *= 1.5f;
                }
                lastClickTime = currentTime;
                autoMove = !autoMove; // Toggle automatic movement
            }
            break; // Exit loop if we found a square
        }
    }
}

void motion(int x, int y) {
    if (isDragging) {
        // Update position of the square while dragging
        for (int i = 0; i < squareCount; i++) {
            if (mouseX >= squares[i].x && mouseX <= squares[i].x + squares[i].size &&
                mouseY >= squares[i].y && mouseY <= squares[i].y + squares[i].size) {
                squares[i].x += x - mouseX;
                squares[i].y += y - mouseY;
                break; // Only drag one square
            }
        }
        mouseX = x;
        mouseY = y;
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // Escape key
        exit(0);
    } else if (key == 's') {
        // Scale up the size of the first square
        if (squareCount > 0) {
            squares[0].size *= 1.1f; // Increase size by 10%
        }
    } else if (key == 'a') {
        // Scale down the size of the first square
        if (squareCount > 0) {
            squares[0].size /= 1.1f; // Decrease size by 10%
        }
    }
    glutPostRedisplay(); // Request to redraw
}

void specialKeyboard(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        if (squareCount > 0) {
            squares[0].size *= 1.1f; // Increase size
        }
    } else if (key == GLUT_KEY_DOWN) {
        if (squareCount > 0) {
            squares[0].size /= 1.1f; // Decrease size
        }
    }
    glutPostRedisplay(); // Request to redraw
}

void ctrlC(unsigned char key, int x, int y) {
    if (key == 'c' && (glutGetModifiers() & GLUT_ACTIVE_CTRL)) {
        // Duplicate the square
        if (squareCount < 10) { // Prevent exceeding array size
            squares[squareCount].x = squares[squareCount - 1].x + 10; // Offset duplicate
            squares[squareCount].y = squares[squareCount - 1].y + 10; // Offset duplicate
            squares[squareCount].size = squares[squareCount - 1].size;
            squares[squareCount].red = squares[squareCount - 1].red;
            squares[squareCount].green = squares[squareCount - 1].green;
            squares[squareCount].blue = squares[squareCount - 1].blue;
            squareCount++;
        }
    }
}

void update(int value) {
    // Update square position based on velocity if auto-move is enabled
    if (autoMove) {
        for (int i = 0; i < squareCount; i++) {
            squares[i].x += squares[i].velocityX;
            squares[i].y += squares[i].velocityY;

            // Check for collisions with window borders
            if (squares[i].x < 0 || squares[i].x + squares[i].size > glutGet(GLUT_WINDOW_WIDTH)) {
                squares[i].velocityX *= -1;
            }
            if (squares[i].y < 0 || squares[i].y + squares[i].size > glutGet(GLUT_WINDOW_HEIGHT)) {
                squares[i].velocityY *= -1;
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // 60 FPS
}

int main(int argc, char** argv) {
    // Initialize the first square
    squares[0].x = squareX;
    squares[0].y = squareY;
    squares[0].size = squareSize;
    squares[0].velocityX = squareVelocityX;
    squares[0].velocityY = squareVelocityY;
    squares[0].red = red;
    squares[0].green = green;
    squares[0].blue = blue;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Draggable, Bounceable Squares");

    textureID = loadBMP("image.bmp"); // Set your BMP path here

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutKeyboardUpFunc(ctrlC); // Use keyboard up function for Ctrl+C check

    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}