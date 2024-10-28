#include <GL/glut.h>
#include <stdlib.h>

float line1StartX = 40, line1StartY = 200;
float line1EndX = 200, line1EndY = 10;

float line2StartX = 100, line2StartY = 100;
float line2EndX = 150, line2EndY = 150;

int line1Color = 0; // 0 for red, 1 for green
int line2Color = 1; // 0 for red, 1 for green

void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);

int main(int argc, char **argv) {
    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(256, 256); // Set window dimensions
    glutInitWindowPosition(100, 100); // Set window position
    glutCreateWindow("Desenhando linhas"); // Create window

    init();
    glutDisplayFunc(display); // Register display function
    glutKeyboardFunc(keyboard); // Register keyboard function
    glutSpecialFunc(specialKeys); // Register special keys (arrow keys)
    
    glutMainLoop(); // Enter GLUT event processing loop
    return 0;
}

// Initialization function
void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Background color
    glOrtho(0, 256, 0, 256, -1, 1); // Orthographic projection
}

// Display function
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT); // Clear the window

    // Draw first line (red or green)
    glColor3f(line1Color == 0 ? 1.0 : 0.0, 0.0, line1Color == 1 ? 1.0 : 0.0);
    glBegin(GL_LINES);
        glVertex2f(line1StartX, line1StartY);
        glVertex2f(line1EndX, line1EndY);
    glEnd();

    // Draw second line (red or green)
    glColor3f(line2Color == 0 ? 1.0 : 0.0, 0.0, line2Color == 1 ? 1.0 : 0.0);
    glBegin(GL_LINES);
        glVertex2f(line2StartX, line2StartY);
        glVertex2f(line2EndX, line2EndY);
    glEnd();

    glFlush(); // Flush drawing commands
}

// Keyboard function
void keyboard(unsigned char key, int x, int y) {
    if (key == 32) { // Space key to invert colors
        line1Color = 1 - line1Color;
        line2Color = 1 - line2Color;
    }
    if (key == 'x' || key == 'X') { // Invert direction
        float tempX = line1StartX;
        float tempY = line1StartY;
        line1StartX = line1EndX;
        line1StartY = line1EndY;
        line1EndX = tempX;
        line1EndY = tempY;

        tempX = line2StartX;
        tempY = line2StartY;
        line2StartX = line2EndX;
        line2StartY = line2EndY;
        line2EndX = tempX;
        line2EndY = tempY;
    }
    glutPostRedisplay(); // Redraw the scene
}

// Special keys function (for arrow keys)
void specialKeys(int key, int x, int y) {
    const float moveAmount = 5.0f;
    switch (key) {
        case GLUT_KEY_UP:
            line1StartY += moveAmount;
            line1EndY += moveAmount;
            line2StartY += moveAmount;
            line2EndY += moveAmount;
            break;
        case GLUT_KEY_DOWN:
            line1StartY -= moveAmount;
            line1EndY -= moveAmount;
            line2StartY -= moveAmount;
            line2EndY -= moveAmount;
            break;
        case GLUT_KEY_LEFT:
            line1StartX -= moveAmount;
            line1EndX -= moveAmount;
            line2StartX -= moveAmount;
            line2EndX -= moveAmount;
            break;
        case GLUT_KEY_RIGHT:
            line1StartX += moveAmount;
            line1EndX += moveAmount;
            line2StartX += moveAmount;
            line2EndX += moveAmount;
            break;
    }
    glutPostRedisplay(); // Redraw the scene
}