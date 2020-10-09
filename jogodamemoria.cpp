#include "jogodamemoria.h"
#include <QKeyEvent>
#include <GL/gl.h>
#include <GL/glu.h>

GLint cartaSelecionada = 6;

// Constructor
JogoDaMemoria::JogoDaMemoria() {
    setWindowTitle("jogo da memória");
}

// Empty destructor
JogoDaMemoria::~JogoDaMemoria() {}

// Initialize OpenGL
void JogoDaMemoria::initializeGL() {
    qglClearColor(Qt::white);

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
            DesenhaCarta(i == cartaSelecionada, -0.7+ 0.35*(i%4), 0.7, i%4+1);
        }
        else {
            DesenhaCarta(i == cartaSelecionada, -0.7 + 0.35*(i%4) , 0, i%4+1);
        }
    }
}

void JogoDaMemoria::DesenhaCarta(bool selecionado, float x_init, float y_init, int figura){

    glColor3f(0.7, 0.7, 0.7);//trocando cor para vermelho
    glBegin(GL_QUADS);
        glVertex2f(x_init, y_init);
        glVertex2f(x_carta + x_init, y_init);
        glVertex2f(x_carta + x_init, y_carta + y_init);
        glVertex2f(x_init, y_carta + y_init);
    glEnd();

    if (figura == 1){
        DesenhaCubo(x_init, y_init);
    }
    else if (figura == 2){
        DesenhaTriangulo(x_init, y_init);
    }
    else if (figura == 3){
        DesenhaIgual(x_init, y_init);
    }
    else if (figura == 4) {
        DesenhaLosangulo(x_init, y_init);
    }

    if (selecionado){
        glColor3f(1.0f, 1.0f, 0);//trocando cor para amarelo
    }
    else {
        glColor3f(0.0f, 0.0f, 0.0f);//trocando cor para branco
    }

    glLineWidth(5.0f);
    glBegin(GL_LINE_LOOP);//desenhando a borda da carta
        glVertex2f(x_init, y_init);
        glVertex2f(x_carta + x_init, y_init);
        glVertex2f(x_carta + x_init, y_carta + y_init);
        glVertex2f(x_init, y_carta + y_init);
    glEnd();
}

void JogoDaMemoria::DesenhaCubo(float x_init, float y_init){
    //pontos da area onde o desenho sera gerado
    x_init = x_init + x_carta/5;
    y_init = y_init + y_carta/4;
    float x_end = x_init + 3*x_carta/5;
    float y_end = y_init + 2*y_carta/4;

    glColor3f(1, 0, 0);//trocando cor para vermelho
    glBegin(GL_QUADS);
        glVertex2f(x_init, y_init);
        glVertex2f(x_end, y_init);
        glVertex2f(x_end, y_end);
        glVertex2f(x_init, y_end);
    glEnd();
}

void JogoDaMemoria::DesenhaTriangulo(float x_init, float y_init){
    //pontos da area onde o desenho sera gerado
    x_init = x_init + x_carta/5;
    y_init = y_init + y_carta/4;
    float x_end = x_init + 3*x_carta/5;
    float y_end = y_init + 2*y_carta/4;

    glColor3f(0, 0, 1);//trocando cor para azul
    glBegin(GL_POLYGON);
        glVertex2f(x_end/2+x_init/2, y_init);
        glVertex2f(x_end, y_end);
        glVertex2f(x_init, y_end);
    glEnd();
}
void JogoDaMemoria::DesenhaIgual(float x_init, float y_init){
    //pontos da area onde o desenho sera gerado
    x_init = x_init + x_carta/5;
    y_init = y_init + y_carta/4;
    float x_end = x_init + 3*x_carta/5;
    float y_end = y_init + 2*y_carta/4;

    glColor3f(0, 1, 0.3);//trocando cor para azul
    glBegin(GL_QUADS);
        glVertex2f(x_init, y_init);
        glVertex2f(x_end, y_init);
        glVertex2f(x_end, y_init + 1.5*y_carta/8);
        glVertex2f(x_init, y_init + 1.5*y_carta/8);

        glVertex2f(x_init, y_init + 2.5*y_carta/8);
        glVertex2f(x_end, y_init + 2.5*y_carta/8);
        glVertex2f(x_end, y_end);
        glVertex2f(x_init, y_end);
    glEnd();
}
void JogoDaMemoria::DesenhaLosangulo(float x_init, float y_init){
    //pontos da area onde o desenho sera gerado
    x_init = x_init + x_carta/5;
    y_init = y_init + y_carta/4;
    float x_end = x_init + 3*x_carta/5;
    float y_end = y_init + 2*y_carta/4;

    glColor3f(0.6, 0, 0.8);//trocando cor para azul
    glBegin(GL_QUADS);
    glVertex2f(x_init, (y_init + y_end)/2);
    glVertex2f((x_init + x_end)/2, y_init);
    glVertex2f(x_end, (y_init + y_end)/2);
    glVertex2f((x_init + x_end)/2, y_end);
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

void JogoDaMemoria::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        cartaSelecionada < 8 ? ++cartaSelecionada: cartaSelecionada=0;
        QPoint point = event->pos();
        setWindowTitle("X="+QString::number(point.x())+" "+"Y="+ QString::number(point.y()));
    }else if(event->button() == Qt::MidButton ){
        cartaSelecionada < 8 ? ++cartaSelecionada: cartaSelecionada=0;
    }else{
        setWindowTitle("Não");
    }
    updateGL();
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
