#ifndef WORK_H
#define WORK_H

#include <QDateTime>
#include <QObject>
#include <QMap>
#include "cloudtypelist.h"
#include "ripmath.h"
#include "mathworker.h"

class Work : public QObject
{
    Q_OBJECT
public:
    Work();
public slots:
    void doWork(const QByteArray &param);
    void Density(bool);
    void setMax(unsigned int b);
    void setAngle(int min,int max);
    void setOffset(unsigned int min,unsigned int max);
    void sChangeInt(QString, int);
    void sChangeDouble(QString, double);
    void sChangeBool(QString, bool);
    void sChangeString(QString, QString);
    void result(const MathVector &buf);
    void resultXX(const MathVector &buf);
    void resultYY(const MathVector &buf);
    void result1(const MathVector &buf);
    void result2(const MathVector &buf);
    void result3(const MathVector &buf);

signals:
    void resultReady(const Clowd &result);
    void progress(int);
    void logLine1(QString);
    void logLine2(QString);
    void progressTimer();
    void run1();
    void run2();
    void Buffer1(const MathVector &buf, int numStart, int numEnd);
    void Buffer2(const MathVector &buf, int numStart, int numEnd);
    void BufferNext1(int num);
    void BufferNext2(int num);
    void initPulse(int leSubBufNum, double leFreq);
    void MaxColorValue(int);
    void ShowXX(IntVector &amp,IntVector &ph);
    void ShowYY(IntVector &amp,IntVector &ph);
    void setSizeBlock(unsigned int);
private:
    unsigned int bSize,bSizeProgress, progressTimerVal;
    bool density,end1,end2, end3;
    QByteArray data;
    void convert();
    void convertCor();
    void convert1024();
    void convertLast();
    RipMath ripMath;
    unsigned int MaxBarier;
    unsigned int MaxAngle;
    unsigned int MinAngle;
    unsigned int MaxOffset;
    unsigned int MinOffset;
    unsigned int MaxColor;
    GLfloat *sObjectLink;
    QMap <QString, int> cInt;
    QMap <QString, double> cDouble;
    QMap <QString, bool> cBool;
    QMap <QString, QString> cString;
    MathVector dataResult0;
    MathVector dataResult1;
    MathVector dataResult2;
    MathVector dataDouble;
    QVector <unsigned int> histA;
    QVector <unsigned int> histY;
    GLfloat centerX;
    GLfloat centerY;
    GLfloat centerZ;
    GLfloat delY;
    Clowd sObject;
    void Proc();
    void plot();
    void plotFlower(int x,int y,int z, unsigned int color);
    void appendPoint(int x,int y,int z, int color);
    unsigned int Size;
    bool rePlot, plotWork;
    QDateTime dt;
    void smooth(double *input, double *output, int n, int window);
};

#endif // WORK_H
