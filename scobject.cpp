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
