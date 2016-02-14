#ifndef MATHWORKER_H
#define MATHWORKER_H

#include <QObject>
#include "cloudtypelist.h"

class MathWorker: public QObject
{
    Q_OBJECT
public:
    MathWorker();
    ~MathWorker();
    void runGet(MathVector &result);

signals:
    void result(const MathVector &buf);
    void resultXX(const MathVector &buf);
    void resultYY(const MathVector &buf);
public slots:
    void run();
    void Buffer(const MathVector &buf, int numStart, int numLast);
    void initPulse(int leSubBufNum, double leFreq);

private:
    void clear();
    int Num, NumStart, NumLast;
    double *OriginalPulseRe;
    double *OriginalPulseIm;
    void MyCorrelation(double* in, int dataSize, double* kernel, int kernelSize, double* out);
    void Math();
    void Math1(unsigned int BufSize, double *DataBuf);
    double *wBuffer;
    int     BuferSize;
    double *a0XXsv0;
    double *a0XXsv1;
    double *a1XXsv0;
    double *a1XXsv1;
    double *a0YYsv0;
    double *a0YYsv1;
    double *a1YYsv0;
    double *a1YYsv1;

    double *ResXXRe, *ResXXIm;
    double *ResYYRe, *ResYYIm;
    double *ResXXAbs, *ResXXAng;
    double *ResYYAbs, *ResYYAng;
    double *ResXXPhase, *ResYYPhase;
    double *ResXXReAvg, *ResXXImAvg;
    double *ResYYReAvg, *ResYYImAvg;
    MathVector mBuffer;
    MathVector ResUlst;
    double *a0XX, *a1XX, *a0YY, *a1YY;
    double *DataBuf;
};

#endif // MATHWORKER_H
