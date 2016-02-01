#ifndef RIP3D_H
#define RIP3D_H

#include <QMainWindow>
#include <QThread>
#include "cloudtypelist.h"
#include "scobject.h"
#include "plotgl.h"

namespace Ui {
class Rip3d;
}

class Rip3d : public QMainWindow
{
    Q_OBJECT
    QThread workerThread;

public:
    explicit Rip3d(QWidget *parent = 0);
    ~Rip3d();
signals:
    void xValueChanged(int);
    void yValueChanged(int);
    void zValueChanged(int);
    void operate(const QByteArray &param);
    void Density(bool);

private slots:
    void on_loadFile_triggered();
    void on_xRotation_valueChanged(int value);
    void on_yRotation_valueChanged(int value);
    void on_zRotation_valueChanged(int value);
    void on_checkBox_toggled(bool checked);
    void on_colorValue_valueChanged(int arg1);
    void on_scale_valueChanged(int value);
    void xRotationChanged(int);
    void yRotationChanged(int);
    void zRotationChanged(int);

    void progress(int);
    void logLine1(QString);
    void logLine2(QString);
    void handleResults(const Clowd &result);

private:
    Ui::Rip3d *ui;
    PlotGl *plotGl;
    ScObject *sc;
};

#endif // RIP3D_H
