#include "mathworker.h"

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
}
void MathWorker::run(){
    ResUlst.resize((NumLast-NumStart)*BLOCKLANGTH);
    Math();
    emit result(ResUlst);
    clear();
}


void MathWorker::MyCorrelation(double* in, int dataSize, double* kernel, int kernelSize, double* out)
{
        int i, j, k;

        // Проверка параметров
        if(!in || !out || !kernel) return;
        if(dataSize <=0 || kernelSize <= 0) return;

        // convolution from out[0] to out[kernelSize-2]
        for(i = 0; i < BLOCKLANGTH; ++i)
        {
                out[i] = 0;                             // init to 0 before sum
                for(j = i, k = 0; j >= 0; --j, ++k){
                        out[i] += in[j] * kernel[k];
                }
        }
        return;
}
void MathWorker::Math()
{
    int j, k;

    int position = 0;
    for(int iNum = NumStart; iNum < NumLast; iNum++){
        DataBuf = mBuffer.data()+iNum*BLOCKRANGE;


        a0XX = DataBuf+BLOCKLANGTH*0;
        a0YY = DataBuf+BLOCKLANGTH*1;
        a1XX = DataBuf+BLOCKLANGTH*2;
        a1YY = DataBuf+BLOCKLANGTH*3;
        memset(a0XXsv0,0,BLOCKLANGTH*sizeof(double));
        memset(a0XXsv1,0,BLOCKLANGTH*sizeof(double));
        memset(a1XXsv0,0,BLOCKLANGTH*sizeof(double));
        memset(a1XXsv1,0,BLOCKLANGTH*sizeof(double));
        memset(a0YYsv0,0,BLOCKLANGTH*sizeof(double));
        memset(a0YYsv1,0,BLOCKLANGTH*sizeof(double));
        memset(a1YYsv0,0,BLOCKLANGTH*sizeof(double));
        memset(a1YYsv1,0,BLOCKLANGTH*sizeof(double));

        for(unsigned int i = 0; i < BLOCKLANGTH; ++i)
        {
            for(j = i, k = 0; j >= 0; --j, ++k){
                double re = OriginalPulseRe[k];
                double im = OriginalPulseRe[k];
                a0XXsv0[i] += a0XX[j] * re;
                a0XXsv1[i] += a0XX[j] * im;
                a1XXsv0[i] += a1XX[j] * re;
                a1XXsv1[i] += a1XX[j] * im;
                a0YYsv0[i] += a0YY[j] * re;
                a0YYsv1[i] += a0YY[j] * im;
                a1YYsv0[i] += a1YY[j] * re;
                a1YYsv1[i] += a1YY[j] * im;
            }
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

         memcpy(ResUlst.data()+position,ResXXAbs,BLOCKLANGTH*sizeof(double));
         position+=BLOCKLANGTH;
     }
}
