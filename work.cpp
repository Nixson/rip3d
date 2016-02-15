#include "work.h"
#include <iostream>

Work::Work(): MaxAngle(360), MinAngle(0), MaxOffset(BLOCKLANGTH), MinOffset(0), Size(0)
{
    density = false;
    plotWork = false;
    rePlot = false;
}
void Work::Density(bool dens){
    density = dens;
    if(data.count() > 0)
        convertCor();
}
void Work::doWork(const QByteArray &param){
    dt = QDateTime::currentDateTime();
    emit logLine1(dt.toString()+" Загрузка данных");

    data = QByteArray(param);
    // берем 0, 16, 15, 31 блоки. Соединяем в строку и отправляем в кор обработку. 1024*4 блоков
    // внутри они разбираются и обрабатываются
    Proc();
}
void Work::Proc(){
    MathWorker *mWorker = new MathWorker();
    mWorker->initPulse(cInt["leSubBufNum"],cDouble["leFreq"]);
    int step = BLOCKLANGTH*32; // определяем количество отсчетов
    int cntD = data.count();
    bSize = cntD/step;
    int bNum[4] = {0, 16, 15, 31};
    //dataDouble = new double[cntD/8];
    dataDouble.resize(cntD/8);
    double *dataDoubleAddr = dataDouble.data();

    for( unsigned int bStep = 0; bStep < bSize; bStep++){
        //номер пачки
        int packet = bStep*step;
        // берем 0, 16, 15, 31 блоки
        //0
        for( int pStep = 0; pStep < 4; pStep++){
            for( int sStep = 0; sStep < BLOCKLANGTH; sStep++){
                int vl = (unsigned int)data[packet+bNum[pStep]*BLOCKLANGTH+sStep];
                if(vl < 0) vl += MAXBYTE;
                *dataDoubleAddr++ = (double)(vl - 128);
            }
        }
    }
    mWorker->Buffer(dataDouble,0,bSize);
    connect(mWorker,&MathWorker::result,this,&Work::result);
    connect(mWorker,&MathWorker::resultXX,this,&Work::resultXX);
    connect(mWorker,&MathWorker::resultYY,this,&Work::resultYY);

    mWorker->run();
}
void Work::resultXX(const MathVector &buf){
    IntVector resultA;
    IntVector resultY;
    //QVector<double> resultYd;
    resultA.resize(bSize*BLOCKLANGTH);
    resultY.resize(bSize*BLOCKLANGTH);
    //resultYd.resize(bSize*BLOCKLANGTH);
    unsigned int *dataResultInt = resultA.data();
    unsigned int *dataResultIntY = resultY.data();
    //double *dataResultIntYd = resultYd.data();
    unsigned int bStepNormal = 0;
    for( unsigned int bStep = 0; bStep < bSize*BLOCKLANGTH; bStep++){
            *(dataResultInt++) = (unsigned int)round(buf[bStepNormal]);
            bStepNormal++;
            *(dataResultIntY++) = (unsigned int)round(buf[bStepNormal]);
            //*(dataResultIntYd++) = round(buf[bStepNormal]);
            bStepNormal++;
    }

    emit ShowXX(resultA,resultY);
}
void Work::resultYY(const MathVector &buf){
    IntVector resultA;
    IntVector resultY;
    resultA.resize(bSize*BLOCKLANGTH);
    resultY.resize(bSize*BLOCKLANGTH);
    unsigned int *dataResultInt = resultA.data();
    unsigned int *dataResultIntY = resultY.data();
    unsigned int bStepNormal = 0;
    for( unsigned int bStep = 0; bStep < bSize*BLOCKLANGTH; bStep++){
            *(dataResultInt++) = (unsigned int)round(buf[bStepNormal]);
            bStepNormal++;
            *(dataResultIntY++) = (unsigned int)round(buf[bStepNormal]);
            bStepNormal++;
    }
    emit ShowYY(resultA,resultY);
}
void Work::result(const MathVector &buf){
    histA.clear();
    histY.clear();
    histA.resize(bSize*BLOCKLANGTH);
    histY.resize(bSize*BLOCKLANGTH);
    unsigned int *dataResultInt = histA.data();
    unsigned int *dataResultIntY = histY.data();
    unsigned int bStepNormal = 0;
    MaxColor = 0;
    for( unsigned int bStep = 0; bStep < bSize*BLOCKLANGTH; bStep++){
            unsigned int dResult = (unsigned int)round(buf[bStepNormal]);
            *(dataResultInt++) = dResult;
            bStepNormal++;
            *(dataResultIntY++) = (unsigned int)round(buf[bStepNormal]);
            bStepNormal++;
            if(MaxColor < dResult){
                MaxColor = dResult;
            }
    }
    MaxColor = MaxColor*0.7;
    emit MaxColorValue(MaxColor);
    Size = (unsigned int)bSize;
    emit setSizeBlock(Size);
    plot();
}
void Work::setAngle(int min, int max){
    MinAngle = (unsigned int)min+180;
    MaxAngle = (unsigned int)max+180;
    delY = 32.0f/(181-max);
    plot();
}
void Work::setOffset(unsigned int min, unsigned int max){
    MinOffset = min;
    MaxOffset = max;
    plot();
}
void Work::setMax(unsigned int b){
    MaxBarier = b;
    plot();
}
void Work::plot(){
    if(histA.length() == 0)
        return;
    if(plotWork){
        rePlot = true;
        return;
    }
    emit logLine1("Start");
    rePlot = false;
    plotWork = true;

    sObject.clear();
    //размер = длина * ширина * 24 точки по 6 координат в каждой
    sObject.resize( (MaxOffset-MinOffset) * Size * 144);

    sObjectLink = sObject.data();


    centerX = (GLfloat)Size/2;
    centerY =  MinAngle + (GLfloat) ( MaxAngle - MinAngle ) /2;
    centerZ = MinOffset + (GLfloat) ( MaxOffset - MinOffset ) /2;
    int realSize = 0;

    for( unsigned int x = 0; x < Size; x++){
        //номер пачки
        int packet = x*BLOCKLANGTH;
        for(unsigned int z = MinOffset; z < MaxOffset; z++){
            unsigned int y = histY[packet+z];
            if(y >= MinAngle && y <= MaxAngle){
                unsigned int color = histA[packet+z];
                if(color >= MaxBarier){
                    plotFlower((int)x,(int)y,(int)z,color);
                    realSize++;
                }
            }
        }

    }
    sObject.resize(realSize*24*6);
    emit resultReady(sObject);
    emit logLine1("Done");
    plotWork = false;
    if(rePlot){
        plot();
    }

}
void Work::appendPoint(int x,int y,int z, int color){
    *(sObjectLink++) = (centerX-x)/MAXBYTEFLOAT;
    *(sObjectLink++) = (centerY-y)/(MAXBYTEFLOAT*delY);
    *(sObjectLink++) = -(centerZ-z)/MAXBYTEFLOAT;
    if(color == 0) {
        *(sObjectLink++) = 0;
        *(sObjectLink++) = 0;
        *(sObjectLink++) = 0;
    }
    else {
        *(sObjectLink++) = (float)color/MAXBYTEFLOAT;
        *(sObjectLink++) = 0.0f;
        *(sObjectLink++) = (float)(MAXBYTEFLOAT - color)/MAXBYTEFLOAT;
    }
}
void Work::plotFlower(int x,int y,int z, unsigned int color){
    double NormalColor = (double)(MaxColor);
    int nColor = (int)MAXBYTE*((double)color)/NormalColor;
    if(nColor > MAXBYTE)
        nColor = MAXBYTE;
    appendPoint(x-1,y,z-1,nColor);
    appendPoint(x+1,y,z-1,nColor);
    appendPoint(x,y+1,z,nColor);
    appendPoint(x-1,y,z+1,nColor);
    appendPoint(x+1,y,z+1,nColor);
    appendPoint(x,y+1,z,nColor);
    appendPoint(x+1,y,z-1,nColor);
    appendPoint(x+1,y,z+1,nColor);
    appendPoint(x,y+1,z,nColor);
    appendPoint(x-1,y,z-1,nColor);
    appendPoint(x-1,y,z+1,nColor);
    appendPoint(x,y+1,z,nColor);

    appendPoint(x-1,y,z-1,nColor);
    appendPoint(x+1,y,z-1,nColor);
    appendPoint(x,y-1,z,nColor);
    appendPoint(x-1,y,z+1,nColor);
    appendPoint(x+1,y,z+1,nColor);
    appendPoint(x,y-1,z,nColor);
    appendPoint(x+1,y,z-1,nColor);
    appendPoint(x+1,y,z+1,nColor);
    appendPoint(x,y-1,z,nColor);
    appendPoint(x-1,y,z-1,nColor);
    appendPoint(x-1,y,z+1,nColor);
    appendPoint(x,y-1,z,nColor);
}
void Work::convertLast(){
    /*
    dt = QDateTime::currentDateTime();
    emit logLine1(dt.toString()+" Обрабатываем");
    emit logLine2("результат");
    dataResult0 += dataResult1;
    dataResult1.clear();
    dataResult0 += dataResult2;
    dataResult2.clear();

    double maxColor = 0;
    unsigned int bStepNormal = 0;
    unsigned int bMaxSize = bSize*BLOCKLANGTH;
    for( unsigned int bStep = 0; bStep < bMaxSize; bStep++){
        double dResult = dataResult0[bStepNormal];
        if(maxColor < dResult)
            maxColor = dResult;
        bStepNormal+=2;
    }
    int maxYpos = 0;
    maxColor =maxColor*0.5;
    unsigned int *PositionMax = new unsigned int[MAXBYTE];
    memset(PositionMax,0,MAXBYTE*sizeof(unsigned int));
    unsigned int *dataResultInt = new unsigned int[bSize*BLOCKRANGE];
    unsigned int *dataResultIntY = new unsigned int[bSize*BLOCKRANGE];
    memset(dataResultInt,0,bSize*BLOCKRANGE*sizeof(unsigned int));
    memset(dataResultIntY,0,bSize*BLOCKRANGE*sizeof(unsigned int));
    //обходим отсчеты
    bStepNormal = 0;
    for( unsigned int bStep = 0; bStep < bMaxSize; bStep++){
            int dResult = (int)((dataResult0[bStepNormal])*MAXBYTE/maxColor);
            if (dResult > MAXBYTE)
                dResult = MAXBYTE;
    //            unsigned int color = (unsigned int)round(MAXBYTE*dResult/maxColor);
            dataResultInt[bStep] = dResult;
            PositionMax[dResult] += 6;
            bStepNormal++;
            int yResult = (int)dataResult0[bStepNormal];
            dataResultIntY[bStep] = yResult;
            if(maxYpos < yResult)
                maxYpos = yResult;
            bStepNormal++;

    }
    dataResult0.clear();
    Clowd c_data;

    GLfloat *pDColor[MAXBYTE];
    for(unsigned int pc = 0; pc < MAXBYTE; pc++){
        if(PositionMax[pc] > 0){
            int max = (int)PositionMax[pc];
            c_data[pc].resize(max);
            pDColor[pc] = c_data[pc].data();
        }
    }
    delete[] PositionMax;

    //обходим отсчеты
    int maxXpos = bSize/2;
    int offset = BLOCKLANGTH/200;
    int start = 60*offset;
    int maxZpos = (BLOCKLANGTH-start)/2;
    maxYpos = maxYpos/2;
    int progressB = 0, lastprogress = 0;
    for (int bStep = 0; bStep < bSize; bStep++) {
        //номер пачки
        int packet = bStep*BLOCKLANGTH;
        //пока Y = 0;
        //обходим Z;
        for(int zPos = 0; zPos < BLOCKLANGTH-start; zPos++){
            int color = dataResultInt[packet+zPos];
            int y = dataResultIntY[packet+zPos];//+color/4;
           // y = 0;
            *(pDColor[color])++ = (maxXpos - bStep)/MAXBYTEFLOAT;
            *(pDColor[color])++ = (y)/MAXBYTEFLOAT;
//            *(pDColor[color])++ = 0.0f;
            *(pDColor[color])++ = (maxZpos - zPos)/MAXBYTEFLOAT;
            *(pDColor[color])++ = color/MAXBYTEFLOAT;
            *(pDColor[color])++ = color/MAXBYTEFLOAT;
            *(pDColor[color])++ = (float)(MAXBYTEFLOAT - color)/MAXBYTEFLOAT;

        }
        progressB = 10 * (bStep) / bSize;
        if(lastprogress!=progressB){
            emit logLine2(QString::number(bStep) + "/" + QString::number(bSize));
            emit progress(progressB*10);
            lastprogress = progressB;
        }
    }
    delete[] dataResultInt;
    delete[] dataResultIntY;
    dt = QDateTime::currentDateTime();
    emit logLine1(dt.toString()+" Готово");

    emit progress(100);
    emit logLine2(QString::number(maxColor));

    emit resultReady(c_data);
    */
}
void Work::smooth(double *input, double *output, int n, int window)
{
   int i,j,z,k1,k2,hw;
   double tmp;
   if(fmod(window,2)==0) window++;
   hw=(window-1)/2;
   output[0]=input[0];

   for (i=1;i<n;i++){
       tmp=0;
       if(i<hw){
           k1=0;
           k2=2*i;
           z=k2+1;
       }
       else if((i+hw)>(n-1)){
           k1=i-n+i+1;
           k2=n-1;
           z=k2-k1+1;
       }
       else{
           k1=i-hw;
           k2=i+hw;
           z=window;
       }

       for (j=k1;j<=k2;j++){
           tmp=tmp+input[j];
       }
       output[i]=tmp/z;
   }
}
void Work::result1(const MathVector &buf){
    dataResult1 = buf;
    end1 = true;
    if(end1==true && end2==true && end3==true){
        convertLast();
    }
}
void Work::result2(const MathVector &buf){
    dataResult2 = buf;
    end2 = true;
    if(end1==true && end2==true && end3==true){
        convertLast();
    }
}
void Work::result3(const MathVector &buf){
    dataResult0 = buf;
    dataResult0.reserve(bSize*BLOCKLANGTH);
    end3 = true;
    if(end1==true && end2==true && end3==true){
        convertLast();
    }
}
void Work::convertCor(){
    MathWorker *mWorker = new MathWorker();
    mWorker->initPulse(cInt["leSubBufNum"],cDouble["leFreq"]);
    emit initPulse(cInt["leSubBufNum"],cDouble["leFreq"]);
    dt = QDateTime::currentDateTime();
    emit logLine1(dt.toString()+" Конвертируем в double");
    emit progress(1);
    progressTimerVal = 1;
    int step = BLOCKLANGTH*32; // определяем количество отсчетов
    int cntD = data.count();
    bSize = cntD/step;
    int bSizeF = bSize/3;
    bSizeProgress = 1;
    end1 = false;
    end2 = false;
    end3 = false;

    int bNum[4] = {0, 16, 15, 31};
    //dataDouble = new double[cntD/8];
    dataDouble.resize(cntD/8);
    double *dataDoubleAddr = dataDouble.data();
    unsigned int numData = 0;
    for(unsigned int bStep = 0; bStep < bSize; bStep++){
        //номер пачки
        int packet = bStep*step;
        // берем 0, 16, 15, 31 блоки
        //0
        for( int pStep = 0; pStep < 4; pStep++){
            for( int sStep = 0; sStep < BLOCKLANGTH; sStep++){
                int vl = (unsigned int)data[packet+bNum[pStep]+sStep];
                if(vl < 0) vl += MAXBYTE;
                *dataDoubleAddr++ = (double)(vl - 128);
                numData++;
                //std::cout << dataDouble[numData] << ": " << (dataDouble+sizeof(double)) << std::endl;

            }
        }
    }
    dt = QDateTime::currentDateTime();
    emit logLine1(dt.toString()+" Запускаем");
    emit logLine2("Кор обработку");
    emit progressTimer();
    emit Buffer1(dataDouble,bSizeF+1,bSizeF*2+1);
    emit Buffer2(dataDouble,bSizeF*2+1,bSize+1);
    emit run1();
    emit run2();
    mWorker->Buffer(dataDouble,0,bSizeF+1);
    connect(mWorker,&MathWorker::result,this,&Work::result3);
    mWorker->run();



    /*
    for(int i = 0; i < bSize; i++){
        mWorker->Buffer((dataDouble+i*BLOCKRANGE),BLOCKRANGE);
        mWorker->run();
    }*/
    //delete[] dataDouble;
//    convertLast();

}
void Work::convert1024(){
    /*int progressB = 0;
    int lastprogress = 0;
    Clowd c_data;
    int PositionMax[MAXBYTE];
    for(int pc = 0; pc < MAXBYTE; pc++){
        PositionMax[pc] = 0;
    }
    int maxXpos = 0;
    int maxYpos = 0;
    int maxZpos = 1024/2;
    int step = 1024*32; // определяем количество отсчетов
    int cntD = data.count();
    int bSize = cntD/step;
    maxXpos = bSize/2;

    //обходим отсчеты
    for (int bStep = 0; bStep < bSize; bStep++) {
        //номер пачки
        int packet = bStep*step;
        //пока Y = 0;
        //обходим Z;
        for(int zPos = 0; zPos < 1024; zPos++){
            int color = (int)data[packet+zPos];
            if(color < 0)
                color = MAXBYTE + color;
            PositionMax[color] += 6;
        }
    }

    GLfloat *pDColor[MAXBYTE];
    for(int pc = 0; pc < MAXBYTE; pc++){
        if(PositionMax[pc] > 0){
            c_data[pc].resize(PositionMax[pc]);
            GLfloat *pD = c_data[pc].data();
            pDColor[pc] = pD;
        }
    }

    //обходим отсчеты
    for (int bStep = 0; bStep < bSize; bStep++) {
        //номер пачки
        int packet = bStep*step;
        //пока Y = 0;
        //обходим Z;
        for(int zPos = 0; zPos < 1024; zPos++){
            int color = (int)data[packet+zPos];
            if(color < 0)
                color = MAXBYTE + color;
            int y = 0;
            *(pDColor[color])++ = (maxXpos - bStep)/MAXBYTEFLOAT;
            *(pDColor[color])++ = (maxYpos - y)/MAXBYTEFLOAT;
//            *(pDColor[color])++ = 0.0f;
            *(pDColor[color])++ = (maxZpos - zPos)/MAXBYTEFLOAT;
            *(pDColor[color])++ = color/MAXBYTEFLOAT;
            *(pDColor[color])++ = 0.0f;
            *(pDColor[color])++ = (float)(MAXBYTEFLOAT - color)/MAXBYTEFLOAT;
            progressB = 10 * (packet+zPos) / cntD;
            if(lastprogress!=progressB){
                emit logLine2(QString::number(packet+zPos) + "/" + QString::number(cntD));
                emit progress(progressB*10);
                lastprogress = progressB;
            }
        }
    }
    emit progress(100);
    emit logLine1("");
    emit logLine2("");
    emit resultReady(c_data);*/

}
void Work::convert(){
    /*emit logLine1("Определение максимумов");
    int progressB = 0;
    int lastprogress = 0;
    Clowd c_data;
    int cntD = data.count();
    int iter = 0;
    int max = 0;
    int maxXpos = 0;
    int maxYpos = 0;
    int maxZpos = 0;
    int PositionMax[MAXBYTE], PositionDensity[MAXBYTE];
    for(int pc = 0; pc < MAXBYTE; pc++){
        PositionMax[pc] = 0;
        PositionDensity[pc] = 0;
    }
    while(iter < cntD){
        int color = (int)data[iter+3];
        if(color < 0)
            color = MAXBYTE + color;
        if(density){
            if(PositionDensity[color] == 0){
                PositionMax[color] += 6;
            }
            PositionDensity[color]++;
            if(PositionDensity[color] > 6)
                PositionDensity[color] = 0;
        }
        else
            PositionMax[color] += 6;
        if(color > max){
            max = color;
            int xPos = (int)data[iter];
            if(xPos < 0)
                xPos = MAXBYTE + xPos;
            int yPos = (int)data[iter+1];
            if(yPos < 0)
                yPos = MAXBYTE + yPos;
            int zPos = (int)data[iter+2];
            if(zPos < 0)
                zPos = MAXBYTE + zPos;
            maxXpos = xPos;
            maxYpos = yPos;
            maxZpos = zPos;
        }
        iter +=4;
    }
    GLfloat *pDColor[MAXBYTE];
    for(int pc = 0; pc < MAXBYTE; pc++){
        if(PositionMax[pc] > 0){
            c_data[pc].resize(PositionMax[pc]);
            GLfloat *pD = c_data[pc].data();
            pDColor[pc] = pD;
        }
        PositionDensity[pc] = 0;
    }
    iter = 0;
    emit logLine1("Конвертация:");
    while(iter < cntD){
        int color = (int)data[iter+3];
        if(color < 0)
            color = MAXBYTE + color;
        iter+=4;
        bool insertP = false;
        if(density){
            if(PositionDensity[color] == 0){
                insertP = true;
            }
            PositionDensity[color]++;
            if(PositionDensity[color] > 6)
                PositionDensity[color] = 0;
        }
        else
            insertP = true;
        if(insertP){
            int x = (int)data[iter];
            if(x < 0)
                x = MAXBYTE + x;
            int y = (int)data[iter+1];
            if(y < 0)
                y = MAXBYTE + y;
            int z = (int)data[iter+2];
            if(z < 0)
                z = MAXBYTE + z;
            *(pDColor[color])++ = (maxXpos - x)/MAXBYTEFLOAT;
            *(pDColor[color])++ = (maxYpos - y)/MAXBYTEFLOAT;
            *(pDColor[color])++ = (maxZpos - z)/MAXBYTEFLOAT;
            *(pDColor[color])++ = color/MAXBYTEFLOAT;
            *(pDColor[color])++ = 0.0f;
            *(pDColor[color])++ = (float)(MAXBYTEFLOAT - color)/MAXBYTEFLOAT;


        }
        progressB = 10 * iter / cntD;
        if(lastprogress!=progressB){
            emit logLine2(QString::number(iter) + "/" + QString::number(cntD));
            emit progress(progressB*10);
            lastprogress = progressB;
        }
    }
    emit logLine1("");
    emit logLine2("");
    emit resultReady(c_data);*/
}
void Work::sChangeInt(QString name, int val){
    cInt[name] = val;
}
void Work::sChangeDouble(QString name, double val){
    cDouble[name] = val;
}
void Work::sChangeBool(QString name, bool val){
    cBool[name] = val;
}
void Work::sChangeString(QString name, QString val){
    cString[name] = val;
}
