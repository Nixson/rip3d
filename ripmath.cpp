#include "ripmath.h"

RipMath::RipMath()
{
    CurrentTxPacketNum = 1;
    OriginalPulseRe = new double[RIPMAX];
    OriginalPulseIm = new double[RIPMAX];
    /*a0XXsv0 = new double[RIPMAX];
    a0XXsv1 = new double[RIPMAX];
    a1XXsv0 = new double[RIPMAX];
    a1XXsv1 = new double[RIPMAX];
    a0YYsv0 = new double[RIPMAX];
    a0YYsv1 = new double[RIPMAX];
    a1YYsv0 = new double[RIPMAX];
    a1YYsv1 = new double[RIPMAX];

    ResXXRe = new double[RIPMAX];
    ResXXIm = new double[RIPMAX];
    ResYYRe = new double[RIPMAX];
    ResYYIm = new double[RIPMAX];
    ResXXAbs = new double[RIPMAX];
    ResXXAng = new double[RIPMAX];
    ResYYAbs = new double[RIPMAX];
    ResYYAng = new double[RIPMAX];
    ResXXPhase = new double[RIPMAX];
    ResYYPhase = new double[RIPMAX];
    ResXXReAvg = new double[RIPMAX];
    ResXXImAvg = new double[RIPMAX];
    ResYYReAvg = new double[RIPMAX];
    ResYYImAvg = new double[RIPMAX];*/
}
void RipMath::initPulse(int leSubBufNum, double leFreq){
    unsigned short BuffSize = 4;
    unsigned char *Buffer = new unsigned char[BuffSize];
    unsigned char *DataPtr = Buffer;

    *((unsigned int *)(DataPtr)) = leSubBufNum*sizeof(int);
    emit SendCmdPacket(4, Buffer, 0x001F, CurrentTxPacketNum);
    delete [] Buffer;
    unsigned int BufSize = leSubBufNum*1024;
    memset(OriginalPulseRe,0,BLOCKLANGTH*sizeof(double));
    memset(OriginalPulseIm,0,BLOCKLANGTH*sizeof(double));
    for(unsigned int i=1; i < BufSize;i++)
    {
           if(i < 10)
           {
               double re = 128*cos(2*M_PI*i/Fd*leFreq*1e6);
               OriginalPulseRe[i] =re;
               double im = 128*sin(2*M_PI*i/Fd*leFreq*1e6);
               OriginalPulseIm[i] = im;
           }
    }
}
void RipMath::OriginalPulse(double* outRe, double* outIm){
    memcpy(outRe,OriginalPulseRe,BLOCKLANGTH*sizeof(double));
    memcpy(outIm,OriginalPulseIm,BLOCKLANGTH*sizeof(double));
}
void RipMath::MyCorrelation(double* in, int dataSize, double* kernel, int kernelSize, double* out)
{
        int i, j, k;
        int num = 0;

        // Проверка параметров
        if(!in || !out || !kernel) return;
        if(dataSize <=0 || kernelSize <= 0) return;

        // start convolution from out[kernelSize-1] to out[dataSize-1] (last)
        for(i = kernelSize-1; i < dataSize; ++i)
        {
                out[i] = 0;                             // init to 0 before accumulate

                for(j = i, k = 0; k < kernelSize; --j, ++k){
                    out[i] += in[j] * kernel[k];
                    num++;
                }
        }

        // convolution from out[0] to out[kernelSize-2]
        for(i = 0; i < kernelSize - 1; ++i)
        {
                out[i] = 0;                             // init to 0 before sum

                for(j = i, k = 0; j >= 0; --j, ++k){
                        out[i] += in[j] * kernel[k];
                        num++;
                }
        }


        return;
}


void RipMath::Math1(unsigned int BufSize, double *DataBuf)
{
     double *a0XX, *a1XX, *a0YY, *a1YY;

     unsigned int Size = BufSize/4;

     a0XX = DataBuf+Size*0;
     a1XX = DataBuf+Size*1;
     a0YY = DataBuf+Size*2;
     a1YY = DataBuf+Size*3;

     MyCorrelation(a0XX, Size, OriginalPulseRe, Size, a0XXsv0);
     MyCorrelation(a0XX, Size, OriginalPulseIm, Size, a0XXsv1);
     MyCorrelation(a1XX, Size, OriginalPulseRe, Size, a1XXsv0);
     MyCorrelation(a1XX, Size, OriginalPulseIm, Size, a1XXsv1);
     MyCorrelation(a0YY, Size, OriginalPulseRe, Size, a0YYsv0);
     MyCorrelation(a0YY, Size, OriginalPulseIm, Size, a0YYsv1);
     MyCorrelation(a1YY, Size, OriginalPulseRe, Size, a1YYsv0);
     MyCorrelation(a1YY, Size, OriginalPulseIm, Size, a1YYsv1);

     for(unsigned int i=0; i<Size; i++)
     {
            // произведение сигналов с двух антенн в режиме ХХ с комплексным сопряжением
                        // Изменил знаки, как в YY, стало лучше, но сдвиг углов между гор. и верт. поляризац. около 3 град.
            ResXXRe[i] = (a0XXsv0[i]*a1XXsv0[i] + a0XXsv1[i]*a1XXsv1[i])/1152/8;//*cos(M_PI/2);

                        // 	ResXXRe[i] = -(a0XXsv0[i]*a1XXsv0[i] + a0XXsv1[i]*a1XXsv1[i])/1152/8;//*cos(M_PI/2);
            // изменён порядок вычисления нумерации антенн 2015.01.31
            ResXXIm[i] = (a0XXsv0[i]*a1XXsv1[i] - a0XXsv1[i]*a1XXsv0[i])/1152/8;//*sin(M_PI/2);
                       // - заменил на +
                       // ResXXIm[i] = (-a0XXsv0[i]*a1XXsv1[i] - a0XXsv1[i]*a1XXsv0[i])/1152/8;//*sin(M_PI/2);
            // вычисление фазы сигнала с антенны 0 в режиме ХХ
            if((a0XXsv1[i] != 0) && (a0XXsv0[i] != 0))

                 ResXXPhase[i] = 180*(atan2(a0XXsv1[i], a0XXsv0[i])+M_PI)/M_PI;
                                 //Убрал из   ResXXPhase[i]    +M_PI для устранения фазового сдвига между каналами - не помогло
                                // ResXXPhase[i] = 180*(atan2(a0XXsv1[i], a0XXsv0[i]))/M_PI;

            if((a0YYsv1[i] != 0) && (a0YYsv0[i] != 0))
                 ResYYPhase[i] = 180*(atan2(a0YYsv1[i], a0YYsv0[i])+M_PI)/M_PI;

            // вычисление модуля и аргумента произведения сигналов в режиме ХХ
            ResXXAbs[i] = pow(ResXXRe[i]*ResXXRe[i] + ResXXIm[i]*ResXXIm[i], 0.5);
            if(ResXXAbs[i] >=1e-13) ResXXAng[i] = RAD*(atan2(ResXXIm[i], ResXXRe[i]));
            else ResXXAng[i] = 0; // аргумент от -180 до 180 градусов

            // произведение сигналов с двух антенн в режиме YY с комплексным сопряжением
            // изменён порядок вычисления нумерации антенн 2015.01.31
            // !!!!! УБРАНА ИНВЕРСИЯ ПРОИЗВЕДЕНИЯ СИГНАЛОВ АНТЕНН ДЛЯ УСТРАНЕНИЯ СИСТЕМАТИЧЕСКОЙ
            // ПОГРЕШНОСТИ РАЗНОСТИ ФАЗ В 180 ГРАДУСОВ
            ResYYRe[i] = (a0YYsv0[i]*a1YYsv0[i] + a0YYsv1[i]*a1YYsv1[i])/1152/8;
            ResYYIm[i] = (a0YYsv0[i]*a1YYsv1[i] - a0YYsv1[i]*a1YYsv0[i])/1152/8;

            // вычисление модуля и аргумента произведения сигналов в режиме YY
            ResYYAbs[i] = pow(ResYYRe[i]*ResYYRe[i] + ResYYIm[i]*ResYYIm[i], 0.5);
            if(ResYYAbs[i] >= 1e-13) ResYYAng[i] = RAD*(atan2(ResYYIm[i], ResYYRe[i]));
            else ResYYAng[i] = 0;
     }
//   formExpDraw->DrawOscCoherentAccum(ResXXAbs, Size);
}

void RipMath::MathAndPlot(int BufSize, int CurBufNum, double *DataBuf)
{
        // корреляционная обработка перекрёстных поляризаций
        Math1(BufSize, DataBuf);
        if(CurBufNum > 0){
            //
        }
}
