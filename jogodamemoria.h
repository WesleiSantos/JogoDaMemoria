#ifndef JOGODAMEMORIA_H
#define JOGODAMEMORIA_H

#include <QGLWidget>
#include <QTime>

class QTimer;

class JogoDaMemoria : public QGLWidget
{
    Q_OBJECT

public:
    JogoDaMemoria();
    ~JogoDaMemoria();
    GLfloat x_carta = 0.33;
    GLfloat y_carta = -0.66;
    GLfloat win=1;
    GLint view_w, view_h;

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *event);
    void changeEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void DesenhaCarta(bool selecionado, float x_init, float y_init, int figura, bool girar);
    void DesenhaCubo(float x_init, float y_init);
    void DesenhaTriangulo(float x_init, float y_init);
    void DesenhaIgual(float x_init, float y_init);
    void DesenhaLosangulo(float x_init, float y_init);
    void areaDesenho(int x_init,int  y_init);

private:
    QTimer *timer;
};
#endif // JOGODAMEMORIA_H


