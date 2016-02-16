#include "mathworker.h"
#include <iostream>

MathWorker::MathWorker()
{
    OriginalPulseRe = new double[BLOCKLANGTH];
    OriginalPulseIm = new double[BLOCKLANGTH];
}
MathWorker::~MathWorker(){
    delete[] OriginalPulseRe;
    delete[] OriginalPulseIm;
}
void MathWorker::initPulse(int leSubBufNum, double leFreq){
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
void MathWorker::Buffer(const MathVector &buf, int numStart, int numLast){
    mBuffer = buf;
    NumStart = numStart;
    NumLast = numLast;
    BuferSize = BLOCKLANGTH*4;
    ResXXRe = new double[BLOCKLANGTH];
    ResXXIm = new double[BLOCKLANGTH];
    ResYYRe = new double[BLOCKLANGTH];
    ResYYIm = new double[BLOCKLANGTH];

    ResXXAbs = new double[BLOCKLANGTH];
    ResXXAng = new double[BLOCKLANGTH];
    ResYYAbs = new double[BLOCKLANGTH];
    ResYYAng = new double[BLOCKLANGTH];

    ResXXPhase = new double[BLOCKLANGTH];
    ResYYPhase = new double[BLOCKLANGTH];
    ResXXReAvg = new double[BLOCKLANGTH];
    ResXXImAvg = new double[BLOCKLANGTH];
    ResYYReAvg = new double[BLOCKLANGTH];
    ResYYImAvg = new double[BLOCKLANGTH];

    a0XXsv0 = new double[BLOCKLANGTH];
    a0XXsv1 = new double[BLOCKLANGTH];
    a1XXsv0 = new double[BLOCKLANGTH];
    a1XXsv1 = new double[BLOCKLANGTH];
    a0YYsv0 = new double[BLOCKLANGTH];
    a0YYsv1 = new double[BLOCKLANGTH];
    a1YYsv0 = new double[BLOCKLANGTH];
    a1YYsv1 = new double[BLOCKLANGTH];

}
void MathWorker::runGet(MathVector &result){
    ResUlst = result;
    Math();
    clear();
}
void MathWorker::clear(){
    delete[] ResXXRe;
    delete[] ResXXIm;
    delete[] ResYYRe;
    delete[] ResYYIm;

    delete[] ResXXAbs;
    delete[] ResXXAng;
    delete[] ResYYAbs;
    delete[] ResYYAng;

    delete[] ResXXPhase;
    delete[] ResYYPhase;
    delete[] ResXXReAvg;
    delete[] ResXXImAvg;
    delete[] ResYYReAvg;
    delete[] ResYYImAvg;

    delete[] a0XXsv0;
    delete[] a0XXsv1;
    delete[] a1XXsv0;
    delete[] a1XXsv1;
    delete[] a0YYsv0;
    delete[] a0YYsv1;
    delete[] a1YYsv0;
    delete[] a1YYsv1;
    mBuffer.clear();
    ResUlst.clear();
    ResUlstY.clear();
}
void MathWorker::run(){
    isFirst = true;
    ResUlst.resize((NumLast-NumStart)*BLOCKLANGTH*2);
    ResUlstY.resize((NumLast-NumStart)*BLOCKLANGTH*2);
    int step = 0;
    //Math();
    for(int iNum = NumStart; iNum < NumLast; iNum++){
        double *DataBuf;
        DataBuf = mBuffer.data()+iNum*BLOCKRANGE;
        Math1(BLOCKRANGE,DataBuf);
        double index = 0;
        for(int i=0; i<BLOCKLANGTH; i++)
        {
               ResUlst[step] = ResXXAbs[i];
               ResUlstY[step] = ResYYAbs[i];
               step++;
               // !!! значения аргумента в диапазоне от -180 до 180
               index = round(ResXXAng[i]+180); // округление аргумента для определения индекса ячейки
               // приведение фазы к диапазону от 0 до 360
               while(index >= 360) index -= 360;
               while(index < 0) index += 360;
               // накопление значений
               ResUlst[step] = index;
               // !!! значения аргумента в диапазоне от -180 до 180
               index = round(ResYYAng[i]+180); // округление аргумента для определения индекса ячейки
               // приведение фазы к диапазону от 0 до 360
               while(index >= 360) index -= 360;
               while(index < 0) index += 360;
               // накопление значений
               ResUlstY[step] = index;
               step++;
        }

    }

    emit result(ResUlst);
    emit resultXX(ResUlst);
    emit resultYY(ResUlst);
    clear();
}


void MathWorker::Math()
{
    int j, k;
    double ResYYPhase, ResXXPhase;
    double ResXXAbs, ResYYAbs;
    double ResXXAng, ResYYAng;
    double ResXXRe, ResXXIm;
    double ResYYRe, ResYYIm;
    double a0XXsv0, a0XXsv1, a1XXsv0, a1XXsv1;
    double a0YYsv0, a0YYsv1, a1YYsv0, a1YYsv1;

    int position = 0;
    double *XXRsp = new double[BLOCKLANGTH*2];
    double *YYRsp = new double[BLOCKLANGTH*2];
    bool fst = true;
    for(int iNum = NumStart; iNum < NumLast; iNum++){
        DataBuf = mBuffer.data()+iNum*BLOCKRANGE;


        a0XX = DataBuf+BLOCKLANGTH*0;
        a1XX = DataBuf+BLOCKLANGTH*1;
        a0YY = DataBuf+BLOCKLANGTH*2;
        a1YY = DataBuf+BLOCKLANGTH*3;

        for(unsigned int i = 0; i < BLOCKLANGTH; ++i)
        {
            a0XXsv0 = 0;
            a0XXsv1 = 0;
            a1XXsv0 = 0;
            a1XXsv1 = 0;
            a0YYsv0 = 0;
            a0YYsv1 = 0;
            a1YYsv0 = 0;
            a1YYsv1 = 0;
            for(j = i, k = 0; j >= 0; --j, ++k){
                double re = OriginalPulseRe[k];
                double im = OriginalPulseIm[k];
                a0XXsv0 += a0XX[j] * re;
                a0XXsv1 += a0XX[j] * im;
                a1XXsv0 += a1XX[j] * re;
                a1XXsv1 += a1XX[j] * im;
                a0YYsv0 += a0YY[j] * re;
                a0YYsv1 += a0YY[j] * im;
                a1YYsv0 += a1YY[j] * re;
                a1YYsv1 += a1YY[j] * im;
            }
            // произведение сигналов с двух антенн в режиме ХХ с комплексным сопряжением
                        // Изменил знаки, как в YY, стало лучше, но сдвиг углов между гор. и верт. поляризац. около 3 град.
            ResXXRe = (a0XXsv0*a1XXsv0 + a0XXsv1*a1XXsv1)/1152/8;//*cos(M_PI/2);

                        // 	ResXXRe = -(a0XXsv0*a1XXsv0 + a0XXsv1*a1XXsv1)/1152/8;//*cos(M_PI/2);
            // изменён порядок вычисления нумерации антенн 2015.01.31
            ResXXIm = (a0XXsv0*a1XXsv1 - a0XXsv1*a1XXsv0)/1152/8;//*sin(M_PI/2);
                       // - заменил на +
                       // ResXXIm = (-a0XXsv0*a1XXsv1 - a0XXsv1*a1XXsv0)/1152/8;//*sin(M_PI/2);
            // вычисление фазы сигнала с антенны 0 в режиме ХХ
            if((a0XXsv1 != 0) && (a0XXsv0 != 0))

                 ResXXPhase = 180*(atan2(a0XXsv1, a0XXsv0)+M_PI)/M_PI;
                                 //Убрал из   ResXXPhase    +M_PI для устранения фазового сдвига между каналами - не помогло
                                // ResXXPhase = 180*(atan2(a0XXsv1, a0XXsv0))/M_PI;

            if((a0YYsv1 != 0) && (a0YYsv0 != 0))
                 ResYYPhase = 180*(atan2(a0YYsv1, a0YYsv0)+M_PI)/M_PI;

            // вычисление модуля и аргумента произведения сигналов в режиме ХХ
            ResXXAbs = pow(ResXXRe*ResXXRe + ResXXIm*ResXXIm, 0.5);
            if(ResXXAbs >=1e-13) ResXXAng = RAD*(atan2(ResXXIm, ResXXRe));
            else ResXXAng = 0; // аргумент от -180 до 180 градусов

            // произведение сигналов с двух антенн в режиме YY с комплексным сопряжением
            // изменён порядок вычисления нумерации антенн 2015.01.31
            // !!!!! УБРАНА ИНВЕРСИЯ ПРОИЗВЕДЕНИЯ СИГНАЛОВ АНТЕНН ДЛЯ УСТРАНЕНИЯ СИСТЕМАТИЧЕСКОЙ
            // ПОГРЕШНОСТИ РАЗНОСТИ ФАЗ В 180 ГРАДУСОВ
            ResYYRe = (a0YYsv0*a1YYsv0 + a0YYsv1*a1YYsv1)/1152/8;
            ResYYIm = (a0YYsv0*a1YYsv1 - a0YYsv1*a1YYsv0)/1152/8;

            // вычисление модуля и аргумента произведения сигналов в режиме YY
            ResYYAbs = pow(ResYYRe*ResYYRe + ResYYIm*ResYYIm, 0.5);
            if(ResYYAbs >= 1e-13) ResYYAng = RAD*(atan2(ResYYIm, ResYYRe));
            else ResYYAng = 0;
            // !!! значения аргумента в диапазоне от -180 до 180
            double index = round(ResXXAng+180); // округление аргумента для определения индекса ячейки
            // приведение фазы к диапазону от 0 до 360
            while(index >= 360) index -= 360;
            while(index < 0) index += 360;
            // накопление значений

            *(XXRsp++) = ResXXAbs;
            *(XXRsp++) = index;
            // !!! значения аргумента в диапазоне от -180 до 180
            index = round(ResYYAng+180); // округление аргумента для определения индекса ячейки
            // приведение фазы к диапазону от 0 до 360
            while(index >= 360) index -= 360;
            while(index < 0) index += 360;
            // накопление значений

            *(YYRsp++) = ResYYAbs;
            *(YYRsp++) = index;
        }
        fst = false;

         memcpy(ResUlst.data()+position,XXRsp,BLOCKLANGTH*sizeof(double)*2);
         memcpy(ResUlstY.data()+position,YYRsp,BLOCKLANGTH*sizeof(double)*2);
         //memcpy(ResUlst.data()+position,ResXXPhase,BLOCKLANGTH*sizeof(double));
         /*memcpy(
          * .data()+position*3,ResYYAbs,BLOCKLANGTH*sizeof(double));
         memcpy(ResUlst.data()+position*4,ResYYPhase,BLOCKLANGTH*sizeof(double));*/
         position+=BLOCKLANGTH*2;
     }
    delete[] XXRsp;
    delete[] YYRsp;
}


void MathWorker::MyCorrelation(double* in, int dataSize, double* kernel, int kernelSize, double* out)
{
        int i, j, k;

        // check validity of params
        if(!in || !out || !kernel) return;
        if(dataSize <=0 || kernelSize <= 0) return;

        // start convolution from out[kernelSize-1] to out[dataSize-1] (last)
        for(i = kernelSize-1; i < dataSize; ++i)
        {
                out[i] = 0;                             // init to 0 before accumulate

                for(j = i, k = 0; k < kernelSize; --j, ++k)
                        out[i] += in[j] * kernel[k];
        }

        // convolution from out[0] to out[kernelSize-2]
        for(i = 0; i < kernelSize - 1; ++i)
        {
                out[i] = 0;                             // init to 0 before sum

                for(j = i, k = 0; j >= 0; --j, ++k)
                        out[i] += in[j] * kernel[k];
        }

        return;
}
void MathWorker::Math1(unsigned int BufSize, double *DataBuf)
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
            /*
            if(isFirst){
                std::cout << ResXXAbs[i] << " : " << ResYYAbs[i] << " : " << ResXXPhase[i] << " : " << ResXXPhase[i] << std::endl;
            }
            */

     }
     isFirst = false;
     //   formExpDraw->DrawOscCoherentAccum(ResXXAbs, Size);
}
