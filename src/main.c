#include <GL/glut.h>
#include <stdio.h>

float xf = 50.0f; // Posição inicial x do retângulo
float yf = 50.0f; // Posição inicial y do retângulo
int win = 100;    // Tamanho da janela
int view_w = 800; // Largura da visualização
int view_h = 600; // Altura da visualização

// Buffer para exibir texto
char texto[100];

// Função para inicializar a janela
void inicializa() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Fundo branco
    glColor3f(0.0f, 0.0f, 0.0f);           // Cor do retângulo (preto)

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-win, win, -win, win, -1.0, 1.0); // Projeção ortográfica
    glMatrixMode(GL_MODELVIEW);
}

// Função para desenhar o retângulo
void Draw() {
    glClear(GL_COLOR_BUFFER_BIT); // Limpa a tela

    // Desenhar o retângulo
    glBegin(GL_POLYGON);
        glVertex2f(xf, yf);
        glVertex2f(xf + 50, yf);
        glVertex2f(xf + 50, yf + 50);
        glVertex2f(xf, yf + 50);
    glEnd();

    glFlush(); // Renderiza agora
}

// Função para lidar com cliques do mouse
void Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Atualiza a posição do retângulo ao clicar
        xf = ((2 * win * x) / view_w) - win;
        yf = (((2 * win) * (y - view_h)) / -view_h) - win;

        glutPostRedisplay(); // Solicita um novo desenho
    }
}

// Função para lidar com o movimento do mouse com o botão pressionado
void MoveMouseComBotaoPressionado(int x, int y) {
    sprintf(texto, "Botao pressionado (%d,%d)", x, y);
    xf = ((2 * win * x) / view_w) - win;
    yf = (((2 * win) * (y - view_h)) / -view_h) - win;

    glutPostRedisplay(); // Solicita um novo desenho
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(view_w, view_h);
    glutCreateWindow("Retângulo Móvel");

    inicializa();

    glutDisplayFunc(Draw);
    glutMouseFunc(Mouse);
    glutMotionFunc(MoveMouseComBotaoPressionado);

    glutMainLoop(); // Entra no loop de eventos do GLUT
    return 0;
}