#include "rcontrol.h"
#include "work.h"
#include <QCoreApplication>

Rcontrol::Rcontrol(QObject *parent) : QObject(parent)
{
//    QString path(QCoreApplication::applicationFilePath()+".conf");
    settings = new QSettings();
    connect(this,&Rcontrol::changeInt,this,&Rcontrol::sChangeInt);
    connect(this,&Rcontrol::changeBool,this,&Rcontrol::sChangeBool);
    connect(this,&Rcontrol::changeDouble,this,&Rcontrol::sChangeDouble);
    connect(this,&Rcontrol::changeString,this,&Rcontrol::sChangeString);
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
Rcontrol::~Rcontrol()
{
    workerThread.quit();
    workerThread.wait();
    corThread1.quit();
    corThread1.wait();
    corThread2.quit();
    corThread2.wait();
}


void Rcontrol::init(){

    int xRotation = settings->value("xRotation",0).toInt();
    emit setxValue(xRotation);
    setInt("xRotation",xRotation);
    int yRotation = settings->value("yRotation",0).toInt();
    emit setyValue(yRotation);
    setInt("yRotation",yRotation);
    int zRotation = settings->value("zRotation",0).toInt();
    emit setzValue(zRotation);
    setInt("zRotation",zRotation);
    int colorValue = settings->value("colorValue",0).toInt();
    emit setVerticalSlider(colorValue);
    setInt("colorValue",colorValue);


    int leSubBufNum = settings->value("leSubBufNum",4).toInt();
    emit setleSubBufNum(leSubBufNum);
    setInt("leSubBufNum",leSubBufNum);
    int leFreq = settings->value("leFreq",1777).toInt();
    emit setleFreq(leFreq);
    setDouble("leFreq",leFreq);


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

void Rcontrol::show(QString formName){
    if(formName=="mmSettings"){
        formsettings->setModal(false);
        formsettings->show();
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
}
void Rcontrol::sChangeInt(QString name, int val){
    settings->sync();
    cInt[name] = val;
}
void Rcontrol::sChangeDouble(QString name, double val){
    settings->sync();
    cDouble[name] = val;
}
void Rcontrol::sChangeBool(QString name, bool val){
    settings->sync();
    cBool[name] = val;
}
void Rcontrol::sChangeString(QString name, QString val){
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
    settings->setValue(name,value);
    emit changeInt(name,value);
}
void Rcontrol::setBool(QString name,bool value){
    settings->setValue(name,value);
    emit changeBool(name,value);
}
void Rcontrol::setDouble(QString name,double value){
    settings->setValue(name,value);
    emit changeDouble(name,value);
}
void Rcontrol::setString(QString name,QString value){
    settings->setValue(name,value);
    emit changeString(name,value);
}
