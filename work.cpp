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
    QMap<int,int> Position;
    QMap<int,int> PositionMax;
    QMap<int,int> PositionDensity;
    for(int pc = 0; pc < 256; pc++){
        Position[pc] = 0;
        PositionMax[pc] = 0;
        PositionDensity[pc] = 0;
    }
    while(iter < cntD){
        int xPos = (int)data[iter];
        if(xPos < 0)
            xPos = 256 + xPos;
        iter++;
        int yPos = (int)data[iter];
        if(yPos < 0)
            yPos = 256 + yPos;
        iter++;
        int zPos = (int)data[iter];
        if(zPos < 0)
            zPos = 256 + zPos;
        iter++;
        int color = (int)data[iter];
        if(color < 0)
            color = 256 + color;
        if(density){
            if(PositionDensity[color] == 0){
                PositionMax[color] = PositionMax[color] + 6;
            }
            PositionDensity[color] = PositionDensity[color] + 1;
            if(PositionDensity[color] > 6)
                PositionDensity[color] = 0;
        }
        else
            PositionMax[color] = PositionMax[color] + 6;
        if(color > max){
            max = color;
            maxXpos = xPos;
            maxYpos = yPos;
            maxZpos = zPos;
        }
        iter++;
        float progressBF = (float) 100 * iter / cntD;
        progressB = (int) progressBF;
        if(lastprogress!=progressB){
            emit logLine2(QString::number(iter) + "/" + QString::number(cntD));
            emit progress(progressB);
            lastprogress = progressB;
        }
    }
    for(int pc = 0; pc < 256; pc++){
        if(PositionMax[pc] > 0)
            c_data[pc].resize(PositionMax[pc]);
        PositionDensity[pc] = 0;
    }
    iter = 0;
    progressB = 0;
    lastprogress = 0;
    emit logLine1("Конвертация:");
    while(iter < cntD){
        int x = (int)data[iter];
        if(x < 0)
            x = 256 + x;
        iter++;
        int y = (int)data[iter];
        if(y < 0)
            y = 256 + y;
        iter++;
        int z = (int)data[iter];
        if(z < 0)
            z = 256 + z;
        iter++;
        int color = (int)data[iter];
        if(color < 0)
            color = 256 + color;
        iter++;
        GLfloat sX = (float)(maxXpos - x)/256;
        GLfloat sY = (float)(maxYpos - y)/256;
        GLfloat sZ = (float)(maxZpos - z)/256;
        GLfloat R = (float)color/255;
        GLfloat G = 0;
        GLfloat B = (float)(255-color)/255;
        bool insertP = false;
        if(density){
            if(PositionDensity[color] == 0){
                insertP = true;
            }
            PositionDensity[color] = PositionDensity[color] + 1;
            if(PositionDensity[color] > 6)
                PositionDensity[color] = 0;
        }
        else
            insertP = true;
        if(insertP){
            GLfloat *pD = c_data[color].data() + Position[color];
            *pD++ = sX;
            *pD++ = sY;
            *pD++ = sZ;
            *pD++ = R;
            *pD++ = G;
            *pD++ = B;
            Position[color] = Position[color] + 6;
        }
        float progressBF = (float) 100 * iter / cntD;
        progressB = (int) progressBF;
        if(lastprogress!=progressB){
            emit logLine2(QString::number(iter) + "/" + QString::number(cntD));
            emit progress(progressB);
            lastprogress = progressB;
        }
    }
    emit logLine1("");
    emit logLine2("");
    emit resultReady(c_data);
}
