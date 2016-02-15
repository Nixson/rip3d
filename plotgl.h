#ifndef PLOTGL_H
#define PLOTGL_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include "cloudtypelist.h"
#include "scobject.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class PlotGl : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    PlotGl(ScObject *sc, QWidget *parent = 0);
    ~PlotGl();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    void setScale(int);
    void updateSc();

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void cleanup();

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void setupVertexAttribs();
    void setupVertexAttribsLast();

    int m_xRot;
    int m_yRot;
    int m_zRot;
    float m_scale;
    QByteArray data;
    void initData();
    QPoint m_lastPos;
    ScObject *m_Sc;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLVertexArrayObject m_vaoLast;
    QOpenGLBuffer m_ScVbo, m_ScVboLast;
    QOpenGLShaderProgram *m_program;
    int m_projMatrixLoc;
    int m_mvMatrixLoc;
    int m_normalMatrixLoc;
    int m_fragColorLoc;
    int m_lightPosLoc;
    int m_RedColorLoc;
    int m_GreenColorLoc;
    int m_BlueColorLoc;

    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;
};

#endif // PLOTGL_H
