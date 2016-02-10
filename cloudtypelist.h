#ifndef CLOUDTYPELIST_H
#define CLOUDTYPELIST_H

#define _USE_MATH_DEFINES
#define BLOCKRANGE 4096
#define BLOCKLANGTH 1024
#define MAXBYTE 256
#define MAXBYTEFLOAT 256.0f
#define RAD 57.295779513082320877
#define MaxSpeed 50

#include <qopengl.h>
#include <QVector>
#include <cmath>


typedef struct Clowd {
    QVector<GLfloat>& operator[](int i) { return byte[i]; }
    QVector<GLfloat> byte[MAXBYTE];
} Clowd;


typedef QVector<double> MathVector;

const unsigned int AVG_DATA_LEN=1024*8*4;
const unsigned int MAX_PKT_DATA_LEN=1024;

const double Fd=2400000000.0f;
const double Td=(1.0/Fd);

const double Ti=10000*1e-6f;
const double Di=5*1.0e-6f;
const unsigned LenN=floor(Di/Td);

const double LFM_dev=300*1e6f;
const double LFM_step=(LFM_dev/LenN / 2);

const double Noise=0;


#endif // CLOUDTYPELIST_H
