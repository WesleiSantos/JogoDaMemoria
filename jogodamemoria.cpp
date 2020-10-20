#include "jogodamemoria.h"
#include <QKeyEvent>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QTimer>

GLint cartaSelecionada = 6;// carta atualmente selecionada
void comparaCarta();
carta cartas[8];
int cartaA= -1;// primeira carta selecionada
int cartaB= -1;// segunda carta selecionada
int cartaE= -1;// valor da carta escolhida incorretamente


GLfloat aspecto, up=0, escala=1;
GLint largura, altura, ang=0;
bool girar= false;
// Constructor
JogoDaMemoria::JogoDaMemoria() {
    setWindowTitle("jogo da memória");
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()),this,SLOT(updateGL()));
}

// Empty destructor
JogoDaMemoria::~JogoDaMemoria() {}

// Initialize OpenGL
void JogoDaMemoria::initializeGL() {
    glShadeModel(GL_SMOOTH); // Enable smooth shading
    qglClearColor(Qt::white);
    glClearDepth(1.0f);									// Depth Buffer Setup
    glClearStencil(0);									// Stencil Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

    //Habilita o uso de texturas
     glEnable(GL_TEXTURE_2D);
     //carrega uma imagem BMP
     QImage img = convertToGLFormat(QImage("fundo.bmp"));
     QImage img2 = convertToGLFormat(QImage("frente.bmp"));
     //definir a forma de armazenamento dos pixeis na textura (1 = alinhamento por byte)
     glGenTextures(2, texture);

     //define a textura corrente
     glBindTexture(GL_TEXTURE_2D, texture[0]);
     //GL_TEXTURE_2D ==> define que será usanda uma textura 2D (bitmaps)
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexImage2D(GL_TEXTURE_2D, 0, 3, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());

     //define a textura corrente
     glBindTexture(GL_TEXTURE_2D, texture[1]);
     //GL_TEXTURE_2D ==> define que será usanda uma textura 2D (bitmaps)
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexImage2D(GL_TEXTURE_2D, 0, 3, img2.width(), img2.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img2.bits());


     // Set up lighting
     GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
     GLint especMaterial = 60;
     GLfloat ambLight[] = {0.8f, 0.8f, 0.8f, 1.0f};
     GLfloat diffLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
     GLfloat especLight[]={1.0, 1.0, 1.0, 1.0};// "brilho"
     GLfloat lightPos[] = {0.0f, 0.0f, 2.0f, 1.0f};
     // Define a refletância do material
     glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE, especularidade);
     // Define a concentração do brilho
     glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
     // Ativa o uso da luz ambiente
     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambLight);
     glLightfv(GL_LIGHT1, GL_AMBIENT, ambLight);
     glLightfv(GL_LIGHT1, GL_DIFFUSE, diffLight);
     glLightfv(GL_LIGHT1, GL_SPECULAR, especLight );
     glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
     // Habilita a definição da cor do material a partir da cor corrente
     glEnable(GL_COLOR_MATERIAL);
     //Habilita o uso de iluminação
     glEnable(GL_LIGHTING);
     // Habilita a luz de número 1
     glEnable(GL_LIGHT1);
     // Habilita o depth-buffering
     glEnable(GL_DEPTH_TEST);
}

// This is called when the OpenGL window is resized
void JogoDaMemoria::resizeGL(int width, int height) {
    // Prevent divide by zero
    if (height == 0)
        height = 1;
    glViewport(0, 0, width, height); // Reset current viewport

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // Reset projection matrix

    gluPerspective(8.0f, static_cast<GLfloat>(width)/height, 5.f, 250.0f); // Calculate aspect ratio
    // Especifica posição do observador e do alvo
    gluLookAt(0.f, -0.5, 15.f, 0,0,0, 0,1,0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// OpenGL painting code goes here
void JogoDaMemoria::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen and depth buffer
    glMatrixMode(GL_MODELVIEW);

    //glRotatef(90, 1, 0, 0);
    for (int i = 0; i<8; i++) {
        cartas[i].figura = i%4;
        cartas[i].id = i;
        if( i < 4 ){
            DesenhaCarta(i == cartaSelecionada, -0.7+ 0.38*(i%4), 0.8, cartas[i]);
        }
        else {
            DesenhaCarta(i == cartaSelecionada, -0.7 + 0.38*(i%4) , -0.14, cartas[i]);
        }
    }
}

void JogoDaMemoria::DesenhaCarta(bool selecionado, float x_init, float y_init, carta carta){

    if(girar && selecionado){
        glPushMatrix();
        ang+=1;
        up+=0.94/180;
        escala+= ang <= 90 ? (float) 1/90 : (float) -1/90;// aumenta a escala até da meia volta e depois reduz até completa a volta
        float y_up = y_init > 0 ? -up : up;
        glRotatef(y_init > 0 ? ang : -ang, 1, 0, 0);
        glTranslatef(0, y_up, 0);
        glScalef(escala, escala, 1);
        timer->start(15);
    }
    else if (carta.escolhida  || carta.id == cartaE){
        glRotatef(-180, 1, 0, 0);
        y_init += y_init > 0 ? -0.94 : 0.94;
    }

    glColor3f(0.7, 0.7, 0.7);//trocando cor para cinza
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(x_init, y_init, 0.01);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(x_carta + x_init, y_init, 0.01);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(x_carta + x_init, y_carta + y_init, 0.01);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(x_init, y_carta + y_init, 0.01);
    glEnd();
    glColor3f(0.7, 0.7, 0.7);//trocando cor para cinza
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(x_init, y_init, 0);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(x_carta + x_init, y_init, 0);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(x_carta + x_init, y_carta + y_init, 0);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(x_init, y_carta + y_init, 0);
    glEnd();

    if (carta.figura == 0){
        DesenhaCubo(x_init, y_init);
    }
    else if (carta.figura == 1){
        DesenhaTriangulo(x_init, y_init);
    }
    else if (carta.figura == 2){
        DesenhaIgual(x_init, y_init);
    }
    else if (carta.figura == 3) {
        DesenhaLosangulo(x_init, y_init);
    }


    if (carta.escolhida){
        glColor3f(0, 1, 0);// trocando cor para verde nas cartas escolhidas
    }
    else if (carta.id == cartaE || ( cartaE > -1 && girar && selecionado)){
        glColor3f(1, 0, 0);// trocando cor da borda para vermelho pela escolha errada
    }
    else if (selecionado){
        glColor3f(1.0f, 1.0f, 0);//trocando cor para amarelo
    }
    else {
        glColor3f(0.0f, 0.0f, 0.0f);//trocando cor para preto
    }

    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);//desenhando a borda da carta
        glVertex3f(x_init, y_init, 0.01);
        glVertex3f(x_carta + x_init, y_init, 0.01);
        glVertex3f(x_carta + x_init, y_carta + y_init, 0.01);
        glVertex3f(x_init, y_carta + y_init, 0.01);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex3f(x_init, y_init, 0);
        glVertex3f(x_carta + x_init, y_init, 0);
        glVertex3f(x_carta + x_init, y_carta + y_init, 0);
        glVertex3f(x_init, y_carta + y_init, 0);
    glEnd();
    if(girar && selecionado){
        glPopMatrix();
        if(ang == 180){
            up=0;
            ang=0;
            timer->stop();
            girar = false;
        }
    }
    else if (carta.escolhida || carta.id == cartaE){
        glRotatef(180, 1, 0, 0);
    }
}

void JogoDaMemoria::DesenhaCubo(float x_init, float y_init){
    //pontos da area onde o desenho sera gerado
    x_init = x_init + x_carta/5;
    y_init = y_init + y_carta/4;
    float x_end = x_init + 3*x_carta/5;
    float y_end = y_init + 2*y_carta/4;

    glColor3f(1, 0, 0);//trocando cor para vermelho
    glBegin(GL_QUADS);
        glVertex3f(x_init, y_init, -0.01);
        glVertex3f(x_end, y_init, -0.01);
        glVertex3f(x_end, y_end, -0.01);
        glVertex3f(x_init, y_end, -0.01);
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
        glVertex3f(x_end/2+x_init/2, y_init, -0.01);
        glVertex3f(x_end, y_end, -0.01);
        glVertex3f(x_init, y_end, -0.01);
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
        glVertex3f(x_init, y_init, -0.01);
        glVertex3f(x_end, y_init, -0.01);
        glVertex3f(x_end, y_init + 1.5*y_carta/8, -0.01);
        glVertex3f(x_init, y_init + 1.5*y_carta/8, -0.01);

        glVertex3f(x_init, y_init + 2.5*y_carta/8, -0.01);
        glVertex3f(x_end, y_init + 2.5*y_carta/8, -0.01);
        glVertex3f(x_end, y_end, -0.01);
        glVertex3f(x_init, y_end, -0.01);
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
    glVertex3f(x_init, (y_init + y_end)/2, -0.01);
    glVertex3f((x_init + x_end)/2, y_init, -0.01);
    glVertex3f(x_end, (y_init + y_end)/2, -0.01);
    glVertex3f((x_init + x_end)/2, y_end, -0.01);
    glEnd();
}

// Key handler
void JogoDaMemoria::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Up:{
        if( !girar){
            cartaE = -1;
            int soma = cartaSelecionada +4;
            cartaSelecionada = soma < 8 ? soma : soma-8;
        }
        break;
    }case Qt::Key_Down:{
        if(!girar){
            int soma = cartaSelecionada +4;
            cartaE = -1;
            cartaSelecionada = soma < 8 ? soma : soma - 8;
        }
        break;
    }case Qt::Key_Right:
        if( !girar){
            cartaSelecionada = (cartaSelecionada + 1)%8;
            cartaE = -1;
        }
        break;
    case Qt::Key_Left:
        if( !girar){
            cartaE = -1;
            cartaSelecionada = cartaSelecionada > 0 ? (--cartaSelecionada)%8 : sizeof (cartas);
        }
        break;
    case Qt::Key_Enter:
    case Qt::Key_Space:
        if(!cartas[cartaSelecionada].escolhida){
            girar = true;
            cartas[cartaSelecionada].escolhida = true;
            if ( cartaA == -1 ){
                cartaA = cartaSelecionada;
            }
            else {
                cartaB = cartaSelecionada;
            }
        }
        comparaCarta();
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

void comparaCarta(){
    if ( cartaA >=0 && cartaB >= 0){//se duas cartas foram escolhidas
        if( cartas[cartaA].figura != cartas[cartaB].figura){
                cartas[cartaA].escolhida = false;
                cartas[cartaB].escolhida = false;
        }
        cartaE = cartaA;
        cartaA = -1;
        cartaB = -1;
    }
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
