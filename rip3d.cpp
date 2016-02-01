#include "rip3d.h"
#include "work.h"
#include "ui_rip3d.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

Rip3d::Rip3d(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Rip3d)
{
    ui->setupUi(this);
    sc = new ScObject;
    plotGl = new PlotGl(sc);
    ui->vlLayout->addWidget(plotGl);
    ui->progressBar->setVisible(false);
    sc->setMax(ui->colorValue->value());

    connect(this, SIGNAL(xValueChanged(int)), plotGl, SLOT(setXRotation(int)));
    connect(plotGl, SIGNAL(xRotationChanged(int)), this, SLOT(xRotationChanged(int)));
    connect(this, SIGNAL(yValueChanged(int)), plotGl, SLOT(setYRotation(int)));
    connect(plotGl, SIGNAL(yRotationChanged(int)), this, SLOT(yRotationChanged(int)));
    connect(this, SIGNAL(zValueChanged(int)), plotGl, SLOT(setZRotation(int)));
    connect(plotGl, SIGNAL(zRotationChanged(int)), this, SLOT(zRotationChanged(int)));

    Work *worker = new Work;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &Rip3d::operate, worker, &Work::doWork);
    connect(worker, &Work::resultReady, this, &Rip3d::handleResults);
    connect(worker, &Work::logLine1, this, &Rip3d::logLine1);
    connect(worker, &Work::logLine2, this, &Rip3d::logLine2);
    connect(worker, &Work::progress, this, &Rip3d::progress);
    connect(this,&Rip3d::Density,worker,&Work::Density);
    workerThread.start();

}
void Rip3d::handleResults(const Clowd &result){
    sc->editData(result);
    plotGl->updateSc();
}
void Rip3d::xRotationChanged(int val){
    ui->xRotation->setValue(val);
}
void Rip3d::yRotationChanged(int val){
    ui->yRotation->setValue(val);
}
void Rip3d::zRotationChanged(int val){
    ui->zRotation->setValue(val);
}
void Rip3d::progress(int pr){
    ui->progressBar->setValue(pr);
    if(pr == 100)
        ui->progressBar->setVisible(false);
    else {
        if(!ui->progressBar->isVisible())
            ui->progressBar->setVisible(true);
    }
}
void Rip3d::logLine1(QString text){
    ui->logLine1->setText(text);
}
void Rip3d::logLine2(QString text){
    ui->logLine2->setText(text);
}

Rip3d::~Rip3d()
{
    delete ui;
    workerThread.quit();
    workerThread.wait();
}

void Rip3d::on_loadFile_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Файл 3d объекта"), "",
            tr("Бинарные файлы (*.3d);;Все файлы (*.*)"));
    if (fileName != "") {
            logLine1("Загрузка файла");
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
                return;
            }
            ui->progressBar->setVisible(true);
            ui->progressBar->setValue(0);
            logLine1("Запись в массив");
            QByteArray data = file.readAll();
            emit operate(data);
//            plotGl->setData(data);
            file.close();
        }
}

void Rip3d::on_xRotation_valueChanged(int value)
{
    emit xValueChanged(value);
}

void Rip3d::on_yRotation_valueChanged(int value)
{
    emit yValueChanged(value);
}

void Rip3d::on_zRotation_valueChanged(int value)
{
    emit zValueChanged(value);
}

void Rip3d::on_checkBox_toggled(bool checked)
{
    emit Density(checked);
}

void Rip3d::on_colorValue_valueChanged(int arg1)
{
    sc->setMax(arg1);
    plotGl->updateSc();
}

void Rip3d::on_scale_valueChanged(int value)
{
    plotGl->setScale(ui->scale->maximum()-value);
}
