#include "plotgl.h"
#include <QMouseEvent>
#include <QCoreApplication>
#include <QPainter>
#include <math.h>

PlotGl::PlotGl(ScObject *sc, QWidget *parent)
    : QOpenGLWidget(parent),
      m_xRot(0),
      m_yRot(0),
      m_zRot(0),
      m_program(0)
{
    m_Sc = sc;
    m_scale = 2.0;
}
void PlotGl::setScale(int scale){
    m_scale = (float)scale/50;
    update();
}

PlotGl::~PlotGl()
{
    cleanup();
}
QSize PlotGl::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize PlotGl::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void PlotGl::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

void PlotGl::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        emit yRotationChanged(angle);
        update();
    }
}

void PlotGl::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        emit zRotationChanged(angle);
        update();
    }
}

void PlotGl::cleanup()
{
    makeCurrent();
    m_ScVbo.destroy();
    delete m_program;
    m_program = 0;
    doneCurrent();
}

static const char *vertexShaderSource =
    "attribute vec4 vertex;\n"
    //"attribute vec3 normal;\n"
    "attribute vec3 aVertexColor;\n"
    "varying vec3 vert;\n"
    "varying vec3 vertNormal;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 mvMatrix;\n"
    "uniform mat3 normalMatrix;\n"
    "varying highp vec4 vColor;\n"
    "void main() {\n"
    "   vert = vertex.xyz;\n"
    "   vertNormal = normalMatrix * vertex.xyz;\n"
    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
    "   vColor = vec4(aVertexColor.x,0,aVertexColor.z,aVertexColor.y);\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying highp vec3 vert;\n"
    "varying highp vec3 vertNormal;\n"
    "uniform highp vec3 lightPos;\n"
    "varying highp vec4 vColor;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos - vert);\n"
    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
    "   highp vec4 col = clamp(vColor * 0.85 + vColor * 0.95 * NL, 0.0, 1.0);\n"
    "   gl_FragColor = col;\n"
    "}\n";


void PlotGl::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &PlotGl::cleanup);

    initializeOpenGLFunctions();
    glClearColor(0.1, 0.1, 0.1, 1);

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("aVertexColor", 1);
    m_program->link();

    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
    m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
    m_fragColorLoc = m_program->uniformLocation("fragColor");
    m_lightPosLoc = m_program->uniformLocation("lightPos");



    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    m_ScVbo.create();
    m_ScVbo.bind();
    m_ScVbo.allocate(m_Sc->constData(), m_Sc->count() * sizeof(GLfloat));
    vaoBinder.rebind();

    setupVertexAttribs();

    m_camera.setToIdentity();
    m_camera.translate(0, 0, -0.2);

    m_program->setUniformValue(m_lightPosLoc, QVector3D(0, 0, 2));

    m_program->release();
}
void PlotGl::setupVertexAttribs()
{
    m_ScVbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    m_ScVbo.release();
}
void PlotGl::updateSc(){
    m_ScVbo.release();
    m_ScVbo.bind();
    m_ScVbo.allocate(m_Sc->constData(), m_Sc->count() * sizeof(GLfloat));
    setupVertexAttribs();
    m_program->release();
    update();
}
void PlotGl::paintGL()
{
    /*QPainter painter;
    painter.begin(this);

    painter.beginNativePainting();*/
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);
    /*glEnable(GL_CULL_FACE);*/
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_camera.setToIdentity();
    m_camera.translate(0, 0, - m_scale);
    m_world.setToIdentity();
    m_world.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
    m_world.rotate(m_yRot / 16.0f, 0, 1, 0);
    m_world.rotate(m_zRot / 16.0f, 0, 0, 1);

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
    QMatrix3x3 normalMatrix = m_world.normalMatrix();
    m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

//    glDrawArrays(GL_POINTS, 0, m_Sc->vertexCount());
    glDrawArrays(GL_TRIANGLES, 0, m_Sc->vertexCount());
    //vaoBinder.release();


    m_program->release();

    /*painter.endNativePainting();


        painter.setPen(Qt::white);
        painter.drawText(0, 1, " paintGL calls / s");

    painter.end();*/

}
void PlotGl::initData(){


}
void PlotGl::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
//    m_proj.perspective(100.0f, GLfloat(w) / h, 0.01f, 100.0f);
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void PlotGl::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void PlotGl::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(m_xRot + 8 * dy);
        setYRotation(m_yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);
    }
    m_lastPos = event->pos();
}
