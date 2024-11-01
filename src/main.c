#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

float xf = -50.0f; // Initial x position of the rectangle
float yf = 0.0f;   // Initial y position of the rectangle
int win = 100;     // Window size
int view_w = 800;  // Width of the view
int view_h = 600;  // Height of the view
float timeElapsed = 0.0f; // Time variable for color animation
bool dragging = false; // Flag to check if dragging is active
int lastMouseX, lastMouseY; // Store last mouse position

// Function to initialize the window
void inicializa() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-win, win, -win, win, -1.0, 1.0); // Set orthographic projection
    glMatrixMode(GL_MODELVIEW);
}

// Function to draw the sine wave rectangle
void Draw() {
    glClear(GL_COLOR_BUFFER_BIT); // Clear the screen

    // Draw a series of rectangles to represent a sine wave
    for (int x = -win; x < win; x += 5) {
        float height = sin(x * 0.1 + timeElapsed) * 50; // Height based on sine function and time
        // Change color based on the sine wave
        glColor3f((sin(height * 0.1 + timeElapsed) + 1) / 2, 0.5f, (cos(height * 0.1 + timeElapsed) + 1) / 2); // Color gradient

        glBegin(GL_POLYGON);
            glVertex2f(x, 0);
            glVertex2f(x + 5, 0);
            glVertex2f(x + 5, height);
            glVertex2f(x, height);
        glEnd();
    }

    // Draw the draggable rectangle
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color for the draggable rectangle
    glBegin(GL_POLYGON);
        glVertex2f(xf, yf);
        glVertex2f(xf + 50, yf);
        glVertex2f(xf + 50, yf + 50);
        glVertex2f(xf, yf + 50);
    glEnd();

    glFlush(); // Render now
}

// Timer function to update time
void Timer(int value) {
    timeElapsed += 0.05f; // Increment time
    glutPostRedisplay(); // Request a redraw
    glutTimerFunc(16, Timer, 0); // Call Timer again after 16ms (~60 FPS)
}

// Function to handle mouse button presses
void Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            // Check if the mouse is inside the rectangle
            if (x >= (xf + win) && x <= (xf + 50 + win) && (view_h - y) >= (yf + win) && (view_h - y) <= (yf + 50 + win)) {
                dragging = true; // Start dragging
                lastMouseX = x;
                lastMouseY = y;
            }
        } else if (state == GLUT_UP) {
            dragging = false; // Stop dragging
        }
    }
}

// Function to handle mouse movement with button pressed
void MoveMouseComBotaoPressionado(int x, int y) {
    if (dragging) {
        // Calculate the change in mouse position
        int deltaX = x - lastMouseX;
        int deltaY = (view_h - y) - lastMouseY;

        // Update the rectangle position
        xf += deltaX;
        yf -= deltaY; // Invert y to match OpenGL coordinates

        lastMouseX = x;
        lastMouseY = (view_h - y); // Update last mouse position

        glutPostRedisplay(); // Request a redraw
    }
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(view_w, view_h);
    glutCreateWindow("Draggable Rectangle with Sine Wave and Color Changing");
    inicializa();

    glutDisplayFunc(Draw);
    glutMouseFunc(Mouse);
    glutMotionFunc(MoveMouseComBotaoPressionado);
    glutTimerFunc(0, Timer, 0); // Initialize timer

    glutMainLoop(); // Enter the GLUT event processing loop
    return 0;
}