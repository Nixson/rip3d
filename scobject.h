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
    int count() const { return m_count; }
    int vertexCount() const { return m_count / 6; }
    void editData(const Clowd &result);
    void setMax(int);

signals:

public slots:

private:
    QVector<GLfloat> m_data;
    QMap<int, QVector<GLfloat> > c_data;
    GLfloat z_step;
    int m_count;
    int maxColor;
    int maxX;
    int maxY;
    int maxZ;
    void loadDataImage();
    void clearData();


};

#endif // SCOBJECT_H
