#ifndef RIPMATH_H
#define RIPMATH_H

#include "cloudtypelist.h"
#include <QObject>

#define RIPMAX 1024

class RipMath : public QObject
{
    Q_OBJECT
public:
    RipMath();
    void MyCorrelation(double* in, int dataSize, double* kernel, int kernelSize, double* out);
    void Math1(unsigned int BufSize, double *DataBuf);
    void initPulse(int leSubBufNum, double leFreq);
    void OriginalPulse(double* outRe, double* outIm);
    unsigned int CurrentTxPacketNum;
    void MathAndPlot(int BufSize, int CurBufNum, double *DataBuf);

public slots:
signals:
    void SendCmdPacket(unsigned short BufferSize, unsigned char *Buffer, unsigned short CmdNum, unsigned int &PacketNum);
private:
    double *OriginalPulseRe;
    double *OriginalPulseIm;
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


};

#endif // RIPMATH_H
