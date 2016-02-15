#include "rcontrol.h"
#include "work.h"
#include <QCoreApplication>

Rcontrol::Rcontrol(QObject *parent) : QObject(parent)
{
    noSync = false;

//    QString path(QCoreApplication::applicationFilePath()+".conf");
    settings = new QSettings();
    connect(this,&Rcontrol::changeInt,this,&Rcontrol::sChangeInt);
    connect(this,&Rcontrol::changeBool,this,&Rcontrol::sChangeBool);
    connect(this,&Rcontrol::changeDouble,this,&Rcontrol::sChangeDouble);
    connect(this,&Rcontrol::changeString,this,&Rcontrol::sChangeString);

    plotterXX = new Plotter;
    plotterYY = new Plotter;
    connect(this, &Rcontrol::setMax, plotterXX, &Plotter::setMax);
    connect(this, &Rcontrol::setAngle, plotterXX, &Plotter::setAngle);
    connect(this, &Rcontrol::setOffset, plotterXX, &Plotter::setOffset);
    connect(this, &Rcontrol::setMax, plotterYY, &Plotter::setMax);
    connect(this, &Rcontrol::setAngle, plotterYY, &Plotter::setAngle);
    connect(this, &Rcontrol::setOffset, plotterYY, &Plotter::setOffset);

    formsettings = new formSettings;

    connect(formsettings,&formSettings::leSubBufNum,this,&Rcontrol::leSubBufNum);
    connect(this,&Rcontrol::setleSubBufNum,formsettings,&formSettings::setleSubBufNum);
    connect(formsettings,&formSettings::leBurstLen,this,&Rcontrol::leBurstLen);
    connect(this,&Rcontrol::setleBurstLen,formsettings,&formSettings::setleBurstLen);


    connect(formsettings,&formSettings::leGeterodin,this,&Rcontrol::leGeterodin);
    connect(this,&Rcontrol::setleGeterodin,formsettings,&formSettings::setleGeterodin);
    connect(formsettings,&formSettings::leFreq,this,&Rcontrol::leFreq);
    connect(this,&Rcontrol::setleFreq,formsettings,&formSettings::setleFreq);
    connect(formsettings,&formSettings::leAmp,this,&Rcontrol::leAmp);
    connect(this,&Rcontrol::setleAmp,formsettings,&formSettings::setleAmp);
    connect(formsettings,&formSettings::cbPulseMod,this,&Rcontrol::cbPulseMod);
    connect(this,&Rcontrol::setcbPulseMod,formsettings,&formSettings::setcbPulseMod);
    connect(formsettings,&formSettings::cbUWB,this,&Rcontrol::cbUWB);
    connect(this,&Rcontrol::setcbUWB,formsettings,&formSettings::setcbUWB);
    connect(formsettings,&formSettings::cbLFM,this,&Rcontrol::cbLFM);
    connect(this,&Rcontrol::setcbLFM,formsettings,&formSettings::setcbLFM);
    connect(formsettings,&formSettings::lePeriod,this,&Rcontrol::lePeriod);
    connect(this,&Rcontrol::setlePeriod,formsettings,&formSettings::setlePeriod);
    connect(formsettings,&formSettings::leDuration,this,&Rcontrol::leDuration);
    connect(this,&Rcontrol::setleDuration,formsettings,&formSettings::setleDuration);
    connect(formsettings,&formSettings::leFreqRange,this,&Rcontrol::leFreqRange);
    connect(this,&Rcontrol::setleFreqRange,formsettings,&formSettings::setleFreqRange);
    connect(formsettings,&formSettings::cbGate,this,&Rcontrol::cbGate);
    connect(this,&Rcontrol::setcbGate,formsettings,&formSettings::setcbGate);
    connect(formsettings,&formSettings::leGateDelay,this,&Rcontrol::leGateDelay);
    connect(this,&Rcontrol::setleGateDelay,formsettings,&formSettings::setleGateDelay);
    connect(formsettings,&formSettings::leGateDuration,this,&Rcontrol::leGateDuration);
    connect(this,&Rcontrol::setleGateDuration,formsettings,&formSettings::setleGateDuration);
    connect(formsettings,&formSettings::cbCont,this,&Rcontrol::cbCont);
    connect(this,&Rcontrol::setcbCont,formsettings,&formSettings::setcbCont);
    connect(formsettings,&formSettings::cbCoherentAccum,this,&Rcontrol::cbCoherentAccum);
    connect(this,&Rcontrol::setcbCoherentAccum,formsettings,&formSettings::setcbCoherentAccum);
    connect(formsettings,&formSettings::cbDDSReset,this,&Rcontrol::cbDDSReset);
    connect(this,&Rcontrol::setcbDDSReset,formsettings,&formSettings::setcbDDSReset);
    connect(formsettings,&formSettings::cbLOGM,this,&Rcontrol::cbLOGM);
    connect(this,&Rcontrol::setcbLOGM,formsettings,&formSettings::setcbLOGM);
    connect(formsettings,&formSettings::rbDdsRstBurst,this,&Rcontrol::rbDdsRstBurst);
    connect(this,&Rcontrol::setrbDdsRstBurst,formsettings,&formSettings::setrbDdsRstBurst);
    connect(formsettings,&formSettings::rbDdsRstPulse,this,&Rcontrol::rbDdsRstPulse);
    connect(this,&Rcontrol::setrbDdsRstPulse,formsettings,&formSettings::setrbDdsRstPulse);

    Work *worker = new Work;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &Rcontrol::operate, worker, &Work::doWork);
    connect(worker, &Work::resultReady, this, &Rcontrol::sresultReady);
    connect(worker, &Work::logLine1, this, &Rcontrol::slogLine1);
    connect(worker, &Work::logLine2, this, &Rcontrol::slogLine2);
    connect(worker, &Work::progress, this, &Rcontrol::sprogress);
    connect(worker, &Work::progressTimer, this, &Rcontrol::sprogressTimer);
    connect(this,&Rcontrol::Density,worker,&Work::Density);
    connect(this, &Rcontrol::setMax, worker, &Work::setMax);
    connect(this, &Rcontrol::setAngle, worker, &Work::setAngle);
    connect(this, &Rcontrol::setOffset, worker, &Work::setOffset);
    connect(worker, &Work::MaxColorValue, this, &Rcontrol::sMaxColorValue);
    connect(worker, &Work::MaxColorValue, plotterXX, &Plotter::MaxColorValue);
    connect(worker, &Work::MaxColorValue, plotterYY, &Plotter::MaxColorValue);
    //connect(worker, &Work::ShowXX, this, &Rcontrol::ShowXX);
    //connect(worker, &Work::ShowYY, this, &Rcontrol::ShowYY);
    connect(worker, &Work::ShowXX, plotterXX, &Plotter::ShowDataVector);
    connect(worker, &Work::ShowYY, plotterYY, &Plotter::ShowDataVector);
    connect(worker, &Work::setSizeBlock,this,&Rcontrol::ssetSizeBlock);


    connect(this,&Rcontrol::changeBool,worker,&Work::sChangeBool);
    connect(this,&Rcontrol::changeDouble,worker,&Work::sChangeDouble);
    connect(this,&Rcontrol::changeInt,worker,&Work::sChangeInt);
    connect(this,&Rcontrol::changeString,worker,&Work::sChangeString);

    workerThread.start();


    MathWorker *mWorker1 = new MathWorker;
    mWorker1->moveToThread(&corThread1);
    connect(&corThread1, &QThread::finished, mWorker1, &QObject::deleteLater);
    connect(worker, &Work::initPulse, mWorker1, &MathWorker::initPulse);
    connect(worker, &Work::run1, mWorker1, &MathWorker::run);
    connect(worker, &Work::Buffer1, mWorker1, &MathWorker::Buffer);
    connect(mWorker1, &MathWorker::result, worker, &Work::result1);
    corThread1.start();
    MathWorker *mWorker2 = new MathWorker;
    mWorker2->moveToThread(&corThread2);
    connect(&corThread2, &QThread::finished, mWorker2, &QObject::deleteLater);
    connect(worker, &Work::initPulse, mWorker2, &MathWorker::initPulse);
    connect(worker, &Work::run2, mWorker2, &MathWorker::run);
    connect(worker, &Work::Buffer2, mWorker2, &MathWorker::Buffer);
    connect(mWorker2, &MathWorker::result, worker, &Work::result2);
    corThread2.start();

}
void Rcontrol::ssetSizeBlock(unsigned int val){
    emit setSizeBlock(val);
}

Rcontrol::~Rcontrol()
{
    workerThread.quit();
    workerThread.wait();
    corThread1.quit();
    corThread1.wait();
    corThread2.quit();
    corThread2.wait();
}

void Rcontrol::sMaxColorValue(int val){
    settings->setValue("MaxColorValue",val);
    emit MaxColorValue(val);
}
void Rcontrol::init(){
    noSync = true;

    int xRotation = settings->value("xRotation",0).toInt();
    emit setxValue(xRotation);
    setInt("xRotation",xRotation);
    int yRotation = settings->value("yRotation",0).toInt();
    emit setyValue(yRotation);
    setInt("yRotation",yRotation);
    int zRotation = settings->value("zRotation",0).toInt();
    emit setzValue(zRotation);
    setInt("zRotation",zRotation);


    int leSubBufNum = settings->value("leSubBufNum",4).toInt();
    emit setleSubBufNum(leSubBufNum);
    setInt("leSubBufNum",leSubBufNum);
    int leFreq = settings->value("leFreq",1777).toInt();
    emit setleFreq(leFreq);
    setDouble("leFreq",leFreq);

    emit MaxColorValue(settings->value("MaxColorValue",64000).toInt());

    int colorValue = settings->value("colorValue",0).toInt();
    emit setVerticalSlider(colorValue);
    setInt("colorValue",colorValue);


    int ArgMin = settings->value("ArgMin",0).toInt();
    emit setArgMin(ArgMin);
    setInt("ArgMin",ArgMin);
    int ArgMax = settings->value("ArgMax",1024).toInt();
    emit setArgMax(ArgMax);
    setInt("ArgMax",ArgMax);
    int PhMin = settings->value("PhMin",10).toInt();
    emit setPhMin(PhMin);
    setInt("PhMin",PhMin);

    emit setMax(colorValue);
    emit setAngle(-PhMin,PhMin);
    emit setOffset((unsigned int)ArgMin,(unsigned int) ArgMax);




    emit setleBurstLen(settings->value("leBurstLen",1).toInt());

    emit setleGeterodin(settings->value("leGeterodin",8000).toInt());
    emit setleAmp(settings->value("leAmp",100).toInt());
    emit setleFreqRange(settings->value("leFreqRange",100).toInt());
    emit setleGateDelay(settings->value("leGateDelay",0).toInt());
    emit setleGateDuration(settings->value("leGateDuration",0.01).toDouble());
    emit setcbPulseMod(settings->value("cbPulseMod",true).toBool());
    emit setcbUWB(settings->value("cbUWB",false).toBool());
    emit setcbLFM(settings->value("cbLFM",false).toBool());
    emit setlePeriod(settings->value("lePeriod",0.1).toDouble());
    emit setleDuration(settings->value("leDuration",0.0033).toDouble());
    emit setcbGate(settings->value("cbGate",false).toBool());
    emit setcbCont(settings->value("cbCont",false).toBool());
    emit setcbCoherentAccum(settings->value("cbCoherentAccum",false).toBool());
    emit setcbDDSReset(settings->value("cbDDSReset",true).toBool());
    emit setcbLOGM(settings->value("cbLOGM",false).toBool());
    emit setrbDdsRstBurst(settings->value("rbDdsRstBurst",false).toBool());
    emit setrbDdsRstPulse(settings->value("rbDdsRstPulse",true).toBool());
    noSync = false;


}
void Rcontrol::doOperate(const QByteArray &param){
    emit operate(param);
}
void Rcontrol::doDensity(bool val){
    emit Density(val);
}

void Rcontrol::sprogress(int val){
    emit progress(val);
}
void Rcontrol::sprogressTimer(){
    emit progressTimer();
}
void Rcontrol::slogLine1(QString str){
    emit logLine1(str);
}
void Rcontrol::slogLine2(QString str){
    emit logLine2(str);
}
void Rcontrol::sresultReady(const Clowd &result){
    emit resultReady(result);
}

void Rcontrol::reInitDock(){
    emit setArgMin(cInt["ArgMin"]);
    emit setArgMax(cInt["ArgMax"]);
    emit setPhMin(cInt["PhMin"]);

}

void Rcontrol::show(QString formName){
    if(formName=="mmSettings"){
        formsettings->setModal(false);
        formsettings->show();
    } else if(formName=="mmPlotXX") {
        plotterXX->setWindowTitle("Горизонтальная поляризация");
        plotterXX->show();
        plotterXX->plot();
    } else if(formName=="mmPlotYY") {
        plotterYY->setWindowTitle("Вертикальная поляризация");
        plotterYY->show();
        plotterYY->plot();
    }
}
void Rcontrol::leSubBufNum(int val){
    settings->value("leSubBufNum",val);
    setInt("leSubBufNum",val);
}
void Rcontrol::leBurstLen(int val){
    settings->value("leBurstLen",val);
    setInt("leBurstLen",val);
}

void Rcontrol::leGeterodin(int val){
    settings->value("leGeterodin",val);
    setInt("leGeterodin",val);
}
void Rcontrol::leFreq(int val){
    settings->value("leFreq",val);
    setInt("leFreq",val);
}
void Rcontrol::leAmp(int val){
    settings->value("leAmp",val);
    setInt("leAmp",val);
}
void Rcontrol::cbPulseMod(bool val){
    settings->value("cbPulseMod",val);
    setBool("cbPulseMod",val);
}
void Rcontrol::cbUWB(bool val){
    settings->value("cbUWB",val);
    setBool("cbUWB",val);
}
void Rcontrol::cbLFM(bool val){
    settings->value("cbLFM",val);
    setBool("cbLFM",val);
}
void Rcontrol::lePeriod(double val){
    settings->value("lePeriod",val);
    setDouble("lePeriod",val);
}
void Rcontrol::leDuration(double val){
    settings->value("leDuration",val);
    setDouble("leDuration",val);
}
void Rcontrol::leFreqRange(int val){
    settings->value("leFreqRange",val);
    setInt("leFreqRange",val);
}
void Rcontrol::cbGate(bool val){
    settings->value("cbGate",val);
    setBool("cbGate",val);
}
void Rcontrol::leGateDelay(int val){
    settings->value("leGateDelay",val);
    setInt("leGateDelay",val);
}
void Rcontrol::leGateDuration(double val){
    settings->value("leGateDuration",val);
    setDouble("leGateDuration",val);
}
void Rcontrol::cbCont(bool val){
    settings->value("cbCont",val);
    setBool("cbCont",val);
}
void Rcontrol::cbCoherentAccum(bool val){
    settings->value("cbCoherentAccum",val);
    setBool("cbCoherentAccum",val);
}
void Rcontrol::cbDDSReset(bool val){
    settings->value("cbDDSReset",val);
    setBool("cbDDSReset",val);
}
void Rcontrol::cbLOGM(bool val){
    settings->value("cbLOGM",val);
    setBool("cbLOGM",val);
}
void Rcontrol::rbDdsRstBurst(bool val){
    settings->value("rbDdsRstBurst",val);
    setBool("rbDdsRstBurst",val);
}
void Rcontrol::rbDdsRstPulse(bool val){
    settings->value("rbDdsRstPulse",val);
    setBool("rbDdsRstPulse",val);
}



void Rcontrol::xValueChanged(int val){
    settings->value("xRotation",val);
    setInt("xRotation",val);
}
void Rcontrol::yValueChanged(int val){
    settings->value("yRotation",val);
    setInt("yRotation",val);
}
void Rcontrol::zValueChanged(int val){
    settings->value("zRotation",val);
    setInt("zRotation",val);
}
void Rcontrol::verticalSliderChanged(int val){
    settings->value("colorValue",val);
    setInt("colorValue",val);
    emit setMax(val);
}
void Rcontrol::sChangeInt(QString name, int val){
    if(!noSync)
        settings->sync();
    cInt[name] = val;
}
void Rcontrol::sChangeDouble(QString name, double val){
    if(!noSync)
        settings->sync();
    cDouble[name] = val;
}
void Rcontrol::sChangeBool(QString name, bool val){
    if(!noSync)
        settings->sync();
    cBool[name] = val;
}
void Rcontrol::sChangeString(QString name, QString val){
    if(!noSync)
        settings->sync();
    cString[name] = val;
}
int Rcontrol::getInt(QString name){
    if(cInt.contains(name)){
        return cInt[name];
    }
    else {
        int resp = settings->value(name,0).toInt();
        cInt[name] = resp;
        return resp;
    }
}
double Rcontrol::getDouble(QString name){
    if(cDouble.contains(name)){
        return cDouble[name];
    }
    else {
        double resp = settings->value(name,0).toDouble();
        cDouble[name] = resp;
        return resp;
    }
}
bool Rcontrol::getBool(QString name){
    if(cBool.contains(name)){
        return cBool[name];
    }
    else {
        bool resp = settings->value(name,false).toBool();
        cBool[name] = resp;
        return resp;
    }
}
QString Rcontrol::getString(QString name){
    if(cString.contains(name)){
        return cString[name];
    }
    else {
        QString resp = settings->value(name,"").toString();
        cString[name] = resp;
        return resp;
    }
}
void Rcontrol::setInt(QString name,int value){
    if(!noSync)
        settings->setValue(name,value);
    emit changeInt(name,value);
}
void Rcontrol::setBool(QString name,bool value){
    if(!noSync)
        settings->setValue(name,value);
    emit changeBool(name,value);
}
void Rcontrol::setDouble(QString name,double value){
    if(!noSync)
        settings->setValue(name,value);
    emit changeDouble(name,value);
}
void Rcontrol::setString(QString name,QString value){
    if(!noSync)
        settings->setValue(name,value);
    emit changeString(name,value);
}
void Rcontrol::ArgMinChanged(int val){
    settings->setValue("ArgMin",val);
    emit changeInt("ArgMin",val);
    emit setOffset((unsigned int)val,(unsigned int) cInt["ArgMax"]);
}
void Rcontrol::ArgMaxChanged(int val){
    settings->setValue("ArgMax",val);
    emit changeInt("ArgMax",val);
    emit setOffset((unsigned int)cInt["ArgMin"],(unsigned int) val);
}
void Rcontrol::PhMinChanged(int val){
    settings->setValue("PhMin",val);
    emit changeInt("PhMin",val);
    emit setAngle(-val,val);
}
