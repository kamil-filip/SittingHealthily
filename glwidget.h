#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();

 protected:
    void initializeGL();
    void paintGL();
    void resize(int width, int height);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

private:
    int xRot;
    int yRot;
    int zRot;

    void update();
    void draw();

    QPoint lastPos;



public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);


signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);


};

#endif // GLWIDGET_H
