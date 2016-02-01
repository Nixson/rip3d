#ifndef CLOUDTYPELIST_H
#define CLOUDTYPELIST_H

#include <qopengl.h>
#include <QVector>

#define MAXBYTE 256
#define MAXBYTEFLOAT 256.0f

typedef struct Clowd {
    QVector<GLfloat>& operator[](int i) { return byte[i]; }
    QVector<GLfloat> byte[MAXBYTE];
} Clowd;

#endif // CLOUDTYPELIST_H
