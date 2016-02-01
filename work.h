#ifndef WORK_H
#define WORK_H

#include <QObject>
#include "cloudtypelist.h"

class Work : public QObject
{
    Q_OBJECT
public:
    Work();
public slots:
    void doWork(const QString &param);
    void Density(bool);
signals:
    void resultReady(const Clowd &result);
    void progress(int);
    void logLine1(QString);
    void logLine2(QString);
private:
    bool density;
    QByteArray data;
    void convert();
};

#endif // WORK_H
