#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>

void Desenha(void)
{
    glMatrixMode(GL_TEXTURE);

    glLoadIdentity();

    int c = GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
    printf("%d %x",c,c);
    glClear(c);

    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_QUADS); // Quadrado de cor azul

    glVertex2i(100, 150);

    glVertex2i(100, 100);

    glColor3f(0.0f, 0.0f, 1.0f);

    glVertex2i(150, 100);

    glVertex2i(150, 150);

    glEnd();

    glFlush();
}

void Inicializa(void)

{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void MudaTamanhoJanela(GLsizei w, GLsizei h)
{
    if (h == 0)
        h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    if (w <= h)
        gluOrtho2D(0.0f, 250.0f, 0.0f, 250.0f * h / w);
    else
        gluOrtho2D(0.0f, 250.0f * w / h, 0.0f, 250.0f);
}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    int a = GLUT_DOUBLE | GLUT_SINGLE | GLUT_RGBA | GLUT_RGB | GLUT_INDEX | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE;
    int b = GLUT_SINGLE | GLUT_RGB;
    printf("%d %x %d %x",a,a,b,b);
    glutInitDisplayMode(b);

    glutInitWindowSize(500, 350);

    glutInitWindowPosition(50, 30);

    glutCreateWindow("Quadrado");

    glutDisplayFunc(Desenha);

    glutReshapeFunc(MudaTamanhoJanela);

    Inicializa();

    glutMainLoop();
}