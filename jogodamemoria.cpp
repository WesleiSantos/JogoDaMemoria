#include "jogodamemoria.h"
#include <QKeyEvent>
#include <GL/gl.h>
#include <GL/glu.h>

// Constructor
JogoDaMemoria::JogoDaMemoria() {
    setWindowTitle("jogo da memória");
}

// Empty destructor
JogoDaMemoria::~JogoDaMemoria() {}

// Initialize OpenGL
void JogoDaMemoria::initializeGL() {
}

// This is called when the OpenGL window is resized
void JogoDaMemoria::resizeGL(int width, int height) {
    // Prevent divide by zero (in the gluPerspective call)
    if (height == 0)
        height = 1;

    glViewport(0, 0, width, height); // Reset current viewport
    glMatrixMode(GL_MODELVIEW); // Select modelview matrix
    glLoadIdentity(); // Reset modelview matrix
}

// OpenGL painting code goes here
void JogoDaMemoria::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen and depth buffer
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // Reset current modelview matrix
    for (int i = 0; i<8; i++) {
        if( i < 4 ){
            DesenhaCarta(i == 5, -0.5+ 0.25*(i%4),0.5);
        }
        else {
            DesenhaCarta(i == 5, -0.5 + 0.25*(i%4) , 0 );
        }
    }
}

void JogoDaMemoria::DesenhaCarta(bool selecionado, float x_init, float y_init){

    glColor3f(1.0f, 0, 0);//trocando cor para vermelho
    glBegin(GL_QUADS);
        glVertex2f(x_init, y_init);
        glVertex2f(0.23 + x_init, y_init);
        glVertex2f( 0.23 + x_init, -0.46 + y_init);
        glVertex2f(x_init, -0.46 + y_init);
    glEnd();

    //depois colocar codigo do desenho aqui a partir de uma variavel pra dizer o que pintar
    if (selecionado){
        glColor3f(1.0f, 1.0f, 0);//trocando cor para amarelo
    }
    else {
        glColor3f(1.0f, 1.0f, 1.0f);//trocando cor para branco
    }
    glLineWidth(5.0f);
    glBegin(GL_LINE_LOOP);//desenhando a borda da carta
        glVertex2f(x_init, y_init);
        glVertex2f(0.23 + x_init, y_init);
        glVertex2f( 0.23 + x_init, -0.46 + y_init);
        glVertex2f(x_init, -0.46 + y_init);
    glEnd();
}

// Key handler
void JogoDaMemoria::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Escape:
        close(); // Quit on Escape
        break;
    case Qt::Key_F1:
        setWindowState(windowState() ^ Qt::WindowFullScreen); // Toggle fullscreen on F1
        break;
    default:
        QGLWidget::keyPressEvent(event); // Let base class handle the other keys
    }
}

void JogoDaMemoria::changeEvent(QEvent *event) {
    switch (event->type()) {
    case QEvent::WindowStateChange:
        // Hide cursor if the window is fullscreen, otherwise show it
        if (windowState() == Qt::WindowFullScreen)
            setCursor(Qt::BlankCursor);
        else if(windowState() == Qt::WindowMaximized) {
            update();
        }
        else
            unsetCursor();
        break;
    default:
        break;
    }
}
