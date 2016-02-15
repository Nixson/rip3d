#ifndef SCOBJECT_H
#define SCOBJECT_H

#include <QObject>
#include "cloudtypelist.h"

class ScObject : public QObject
{
    Q_OBJECT
public:
    explicit ScObject(QObject *parent = 0);
    const GLfloat *constData() const { return m_data.constData(); }
    const GLfloat *constDataLines() const { return l_data.constData(); }
    int count() const { return m_count; }
    int countLines() const { return l_count; }
    int vertexCount() const { return m_count / 6; }
    int vertexCountLines() const { return l_count / 6; }
    void editData(const Clowd &result);
    void setMax(int);
    void setAngle(int,int);
    void setOffset(unsigned int, unsigned int);
    void setSizeBlock(unsigned int);

signals:

public slots:

private:
    QVector<GLfloat> m_data;
    QVector<GLfloat> l_data;
    Clowd c_data;
    int m_count;
    int l_count;
    int maxColor;
    int maxX;
    int maxY;
    int maxZ;
    void loadDataImage();
    void clearData();
    int AngleMin, AngleMax;
    unsigned int OffsetMin;
    unsigned int OffsetMax;
    unsigned int Size;
    void reLine();
    GLfloat centerX, centerY, centerZ;
    void appendPoint(int x,int y,int z);
    GLfloat delY;
    GLfloat *sObjectLink;


};

#endif // SCOBJECT_H
