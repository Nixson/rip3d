#include "scobject.h"
#include <qmath.h>
#include <iostream>
using namespace std;

ScObject::ScObject(QObject *parent)
    :QObject(parent),
    m_count(0)
{
    maxColor = 200;
    z_step = 0.24f;

    clearData();
    loadDataImage();

}
void ScObject::setMax(int color){
    maxColor = color;
    m_data.clear();
    m_count = 0;
    loadDataImage();
}

void ScObject::loadDataImage(){
    int size = 0;
    int sMax = maxColor+10;
    if(sMax > 255) sMax = 255;
    for( int i = maxColor; i < 255; i++){
        size += c_data[i].size();
    }
    m_data.resize( size );
    m_count = 0;
    for( int i = maxColor; i < 255; i++){
        int size = c_data[i].size();
        if(size == 0) continue;
        GLfloat *s = c_data[i].data();
        GLfloat *p = m_data.data() + m_count;
        memcpy(p,s, size * sizeof(GLfloat) );
        m_count += size;
    }
}
void ScObject::editData(const Clowd &result){
    c_data = result;
    loadDataImage();

}
void ScObject::clearData(){
    for(int i = 0; i < 256; i++){
        c_data[i].clear();
    }
}
