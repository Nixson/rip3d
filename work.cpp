#include "work.h"

Work::Work()
{
    density = false;
}
void Work::Density(bool dens){
    density = dens;
    if(data.count() > 0)
        convert();
}
void Work::doWork(const QByteArray &param){
    data = QByteArray(param);
    convert();
}
void Work::convert(){
    emit logLine1("Определение максимумов");
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
    emit resultReady(c_data);
}
