#ifndef RCONTROL_H
#define RCONTROL_H

#include <QObject>
#include <QSettings>
#include <QThread>
#include "formsettings.h"
#include "cloudtypelist.h"
#include "mathworker.h"
#include <iostream>

class Rcontrol : public QObject
{
    Q_OBJECT
    QThread workerThread, corThread1, corThread2;

public:
    explicit Rcontrol(QObject *parent = 0);
    ~Rcontrol();
    void init();

    void show(QString);

    int     getInt(QString);
    bool    getBool(QString);
    double  getDouble(QString);
    QString getString(QString);
    void    setInt(QString,int);
    void    setBool(QString,bool);
    void    setDouble(QString,double);
    void    setString(QString,QString);


signals:
    void operate(const QByteArray &param);
    void Density(bool);
    void progress(int);
    void progressTimer();
    void logLine1(QString);
    void logLine2(QString);
    void resultReady(const Clowd &result);

    void setxValue(int);
    void setyValue(int);
    void setzValue(int);
    void setVerticalSlider(int);
    void setleSubBufNum(int);
    void setleBurstLen(int);

    void setleGeterodin(int);
    void setleFreq(int);
    void setleAmp(int);
    void setcbPulseMod(bool);
    void setcbUWB(bool);
    void setcbLFM(bool);
    void setlePeriod(double);
    void setleDuration(double);
    void setleFreqRange(int);
    void setcbGate(bool);
    void setleGateDelay(int);
    void setleGateDuration(double);
    void setcbCont(bool);
    void setcbCoherentAccum(bool);
    void setcbDDSReset(bool);
    void setcbLOGM(bool);
    void setrbDdsRstBurst(bool);
    void setrbDdsRstPulse(bool);

    void changeInt(QString,int);
    void changeDouble(QString,double);
    void changeBool(QString,bool);
    void changeString(QString,QString);

public slots:
    void doOperate(const QByteArray &param);
    void doDensity(bool);
    void sprogress(int);
    void sprogressTimer();
    void slogLine1(QString);
    void slogLine2(QString);
    void sresultReady(const Clowd &result);

    void xValueChanged(int);
    void yValueChanged(int);
    void zValueChanged(int);
    void verticalSliderChanged(int);
    void leSubBufNum(int);
    void leBurstLen(int);

    void leGeterodin(int);
    void leFreq(int);
    void leAmp(int);
    void cbPulseMod(bool);
    void cbUWB(bool);
    void cbLFM(bool);
    void lePeriod(double);
    void leDuration(double);
    void leFreqRange(int);
    void cbGate(bool);
    void leGateDelay(int);
    void leGateDuration(double);
    void cbCont(bool);
    void cbCoherentAccum(bool);
    void cbDDSReset(bool);
    void cbLOGM(bool);
    void rbDdsRstBurst(bool);
    void rbDdsRstPulse(bool);



    void sChangeInt(QString, int);
    void sChangeDouble(QString, double);
    void sChangeBool(QString, bool);
    void sChangeString(QString, QString);
private:
    QSettings *settings;
    QMap <QString, int> cInt;
    QMap <QString, double> cDouble;
    QMap <QString, bool> cBool;
    QMap <QString, QString> cString;
    formSettings *formsettings;
};

#endif // RCONTROL_H
