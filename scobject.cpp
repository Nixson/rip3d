#include "scobject.h"
#include <qmath.h>
#include <iostream>
using namespace std;

ScObject::ScObject(QObject *parent)
    :QObject(parent),
    m_count(0),
    maxColor(0)
{
    clearData();
    loadDataImage();
    Size = 0;
    AngleMin = 0;
    AngleMax = 0;
    OffsetMin = 0;
    OffsetMax = 0;
    delY = 1.0f;

}
void ScObject::setAngle(int min,int max){
    AngleMin = min+180;
    AngleMax = max+180;
    delY = (GLfloat)32.0f/(181-max);
    reLine();
}
void ScObject::setOffset(unsigned int min, unsigned int max){
    OffsetMin = min;
    OffsetMax = max;
    reLine();
}
void ScObject::setSizeBlock(unsigned int size){
    Size = size;
    reLine();
}
void ScObject::reLine(){
    if(Size == 0) return;
    l_count = 12*2*6;
    l_data.resize(24*6);
    sObjectLink = l_data.data();
    centerX = (GLfloat)Size/2;
    centerY =  AngleMin + (GLfloat) ( AngleMax - AngleMin ) /2;
    centerZ = OffsetMin + (GLfloat) ( OffsetMax - OffsetMin ) /2;
    //top
    appendPoint(-1,AngleMin-1,OffsetMin-1);
    appendPoint(-1,AngleMin-1,OffsetMax+1);
    appendPoint(-1,AngleMin-1,OffsetMin-1);
    appendPoint(-1,AngleMax+1,OffsetMin-1);
    appendPoint(-1,AngleMax+1,OffsetMax+1);
    appendPoint(-1,AngleMax+1,OffsetMin-1);
    appendPoint(-1,AngleMax+1,OffsetMax+1);
    appendPoint(-1,AngleMin-1,OffsetMax+1);

    appendPoint(Size+1,AngleMin-1,OffsetMin-1);
    appendPoint(Size+1,AngleMin-1,OffsetMax+1);
    appendPoint(Size+1,AngleMin-1,OffsetMin-1);
    appendPoint(Size+1,AngleMax+1,OffsetMin-1);
    appendPoint(Size+1,AngleMax+1,OffsetMax+1);
    appendPoint(Size+1,AngleMax+1,OffsetMin-1);
    appendPoint(Size+1,AngleMax+1,OffsetMax+1);
    appendPoint(Size+1,AngleMin-1,OffsetMax+1);

    appendPoint(-1,AngleMax+1,OffsetMax+1);
    appendPoint(Size+1,AngleMax+1,OffsetMax+1);

    appendPoint(-1,AngleMin-1,OffsetMax+1);
    appendPoint(Size+1,AngleMin-1,OffsetMax+1);

    appendPoint(-1,AngleMax+1,OffsetMin-1);
    appendPoint(Size+1,AngleMax+1,OffsetMin-1);

    appendPoint(-1,AngleMin-1,OffsetMin-1);
    appendPoint(Size+1,AngleMin-1,OffsetMin-1);
    /*appendPoint(Size+1,AngleMax+1,OffsetMax+1);
    appendPoint(Size+1,AngleMax+1,OffsetMax-1);*/

    /*appendPoint(-1,AngleMin-1,OffsetMax+1);
    appendPoint(-1,AngleMax+1,OffsetMax+1);
    appendPoint(-1,AngleMax+1,OffsetMin-1);
    //bottom
    appendPoint(Size+1,-1,-1);
    appendPoint(Size+1,-1,OffsetMax+1);
    appendPoint(Size+1,-1,-1);
    appendPoint(Size+1,AngleMax+1,-1);
    appendPoint(Size+1,AngleMax+1,OffsetMax+1);
    appendPoint(Size+1,-1,OffsetMax+1);
    appendPoint(Size+1,AngleMax+1,OffsetMax+1);
    appendPoint(Size+1,AngleMax+1,-1);
    appendPoint(-1,-1,-1);
    appendPoint(Size+1,-1,-1);
    appendPoint(-1,AngleMax+1,-1);
    appendPoint(Size+1,AngleMax+1,-1);
    appendPoint(-1,-1,OffsetMax+1);
    appendPoint(Size+1,-1,OffsetMax+1);
    appendPoint(-1,AngleMax+1,OffsetMax+1);
    appendPoint(Size+1,AngleMax+1,OffsetMax+1);*/
}
void ScObject::appendPoint(int x,int y,int z){
    *(sObjectLink++) = (centerX-x)/MAXBYTEFLOAT;
    *(sObjectLink++) = (centerY-y)/(MAXBYTEFLOAT*delY);
    *(sObjectLink++) = -(centerZ-z)/MAXBYTEFLOAT;
    *(sObjectLink++) = 0.0f;
    *(sObjectLink++) = 1.0f;
    *(sObjectLink++) = 0.0f;
}
void ScObject::setMax(int color){
    maxColor = color;
    m_data.clear();
    m_count = 0;
    loadDataImage();
}

void ScObject::loadDataImage(){
    m_data = c_data;
    int size = m_data.size();
/*    GLfloat *s = c_data.data();
    GLfloat *p = m_data.data();
    memcpy(p,s, size * sizeof(GLfloat) );*/
    m_count = size;
    /*int size = c_data[maxColor].size();
    GLfloat *s = c_data[maxColor].data();
    GLfloat *p = m_data.data();
    memcpy(p,s, size * sizeof(GLfloat) );
    m_count = size;
    return;
    */

    /*int size = 0;
    int sMax = maxColor+10;
    if(sMax > MAXBYTE) sMax = MAXBYTE;
    for( int i = maxColor; i < MAXBYTE; i++){
        size += c_data[i].size();
    }
    m_data.resize( size );
    m_count = 0;
    for( int i = maxColor; i < MAXBYTE; i++){
        int size = c_data[i].size();
        if(size == 0) continue;
        GLfloat *s = c_data[i].data();
        GLfloat *p = m_data.data() + m_count;
        memcpy(p,s, size * sizeof(GLfloat) );
        m_count += size;
    }*/
}
void ScObject::editData(const Clowd &result){
    c_data = result;
    loadDataImage();

}
void ScObject::clearData(){
    c_data.clear();
}
