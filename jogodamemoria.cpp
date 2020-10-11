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
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);
}

// This is called when the OpenGL window is resized
void JogoDaMemoria::resizeGL(int width, int height) {
    // Prevent divide by zero
    if (height == 0)
        height = 1;

    glViewport(0, 0, width, height); // Reset current viewport

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // Reset projection matrix

    gluPerspective(45.0f, static_cast<GLfloat>(width)/height, 5.f, 250.0f); // Calculate aspect ratio

    // Especifica posição do observador e do alvo
    gluLookAt(40.f,60.f,100.f, 0,0,0, 0,1,0);
}

// OpenGL painting code goes here
void JogoDaMemoria::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen and depth buffer
    glLoadIdentity(); // Reset current modelview matrix
    for (int i = 0; i<8; i++) {
        if( i < 4 ){
            DesenhaCarta(i == cartaSelecionada, -0.7+ 0.35*(i%4), 0.7, i%4+1);
        }
        else {
            //glRotatef(15, 1, 0, 0);
            DesenhaCarta(i == cartaSelecionada, -0.7 + 0.35*(i%4) , 0, i%4+1);
            //glRotatef(-15, 1, 0, 0);
        }
    }
}

void JogoDaMemoria::DesenhaCarta(bool selecionado, float x_init, float y_init, int figura){
    glColor3f(0.7, 0.7, 0.7);//trocando cor para cinza

    glBegin(GL_QUADS);
        glVertex3f(x_init, y_init, 0.1);
        glVertex3f(x_carta + x_init, y_init, 0.1);
        glVertex3f(x_carta + x_init, y_carta + y_init, 0.1);
        glVertex3f(x_init, y_carta + y_init, 0.1);

        glVertex3f(x_init, y_init, 0);
        glVertex3f(x_carta + x_init, y_init, 0);
        glVertex3f(x_carta + x_init, y_carta + y_init, 0);
        glVertex3f(x_init, y_carta + y_init, 0);
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
        glVertex3f(x_init, y_init, 1);
        glVertex3f(x_carta + x_init, y_init, 1);
        glVertex3f(x_carta + x_init, y_carta + y_init, 1);
        glVertex3f(x_init, y_carta + y_init, 1);
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
        glVertex3f(x_init, y_init, 0);
        glVertex3f(x_end, y_init, 0);
        glVertex3f(x_end, y_end, 0);
        glVertex3f(x_init, y_end, 0);
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
        glVertex3f(x_end/2+x_init/2, y_init, 0);
        glVertex3f(x_end, y_end, 0);
        glVertex3f(x_init, y_end, 0);
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
        glVertex3f(x_init, y_init, 0);
        glVertex3f(x_end, y_init, 0);
        glVertex3f(x_end, y_init + 1.5*y_carta/8, 0);
        glVertex3f(x_init, y_init + 1.5*y_carta/8, 0);

        glVertex3f(x_init, y_init + 2.5*y_carta/8, 0);
        glVertex3f(x_end, y_init + 2.5*y_carta/8, 0);
        glVertex3f(x_end, y_end, 0);
        glVertex3f(x_init, y_end, 0);
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
    glVertex3f(x_init, (y_init + y_end)/2, 0);
    glVertex3f((x_init + x_end)/2, y_init, 0);
    glVertex3f(x_end, (y_init + y_end)/2, 0);
    glVertex3f((x_init + x_end)/2, y_end, 0);
    glEnd();
}

// Key handler
void JogoDaMemoria::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Up:{
        int soma = cartaSelecionada +4;
        if(soma<8){
            cartaSelecionada = soma;
        }else{
            cartaSelecionada = soma - 8;
        }
        break;
    }case Qt::Key_Down:{
        int soma = cartaSelecionada +4;
        if(soma<8){
            cartaSelecionada = soma;
        }else{
            cartaSelecionada = soma - 8;
        }
        break;
    }case Qt::Key_Right:
            cartaSelecionada = cartaSelecionada + 1;
             break;
    case Qt::Key_Left:
            cartaSelecionada = cartaSelecionada - 1;
             break;
    case Qt::Key_Escape:
        close(); // Quit on Escape
        break;
    case Qt::Key_F1:
        setWindowState(windowState() ^ Qt::WindowFullScreen); // Toggle fullscreen on F1
        break;
    default:
        QGLWidget::keyPressEvent(event); // Let base class handle the other keys
    }
    updateGL();
}

void JogoDaMemoria::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        cartaSelecionada < 8 ? ++cartaSelecionada: cartaSelecionada=0;
        QPoint point = event->pos();
        GLfloat xf = (( (2 * win * point.x()) / view_w) - win);
        GLfloat yf = (( ( (2 * win) * (point.y()-view_h) ) / -view_h) - win);
        setWindowTitle("X="+QString::number(xf)+" "+"Y="+ QString::number(yf));
        //setWindowTitle("X="+QString::number(point.x())+" "+"Y="+ QString::number(point.y()));
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
