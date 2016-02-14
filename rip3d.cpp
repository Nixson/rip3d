#include "rip3d.h"
#include "work.h"
#include "ui_rip3d.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

Rip3d::Rip3d(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Rip3d),
    first(true),
    timerOn(false)
{
    first = true;

    ui->setupUi(this);
    ui->dockWidget->hide();
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

    connect(this, &Rip3d::operate, &control, &Rcontrol::doOperate);
    connect(&control, &Rcontrol::resultReady, this, &Rip3d::handleResults);
    connect(&control, &Rcontrol::logLine1, this, &Rip3d::logLine1);
    connect(&control, &Rcontrol::logLine2, this, &Rip3d::logLine2);
    connect(&control, &Rcontrol::progress, this, &Rip3d::progress);
    connect(&control, &Rcontrol::progressTimer, this, &Rip3d::progressTimer);
    connect(this,&Rip3d::Density,&control,&Rcontrol::doDensity);


    connect(this,&Rip3d::xValueChanged,&control,&Rcontrol::xValueChanged);
    connect(&control,&Rcontrol::setxValue,this,&Rip3d::xRotationChanged);
    connect(this,&Rip3d::yValueChanged,&control,&Rcontrol::yValueChanged);
    connect(&control,&Rcontrol::setyValue,this,&Rip3d::yRotationChanged);
    connect(this,&Rip3d::zValueChanged,&control,&Rcontrol::zValueChanged);
    connect(&control,&Rcontrol::setzValue,this,&Rip3d::zRotationChanged);

    connect(this,&Rip3d::ArgMinChanged,&control,&Rcontrol::ArgMinChanged);
    connect(&control,&Rcontrol::setArgMin,this,&Rip3d::setArgMin);
    connect(this,&Rip3d::ArgMaxChanged,&control,&Rcontrol::ArgMaxChanged);
    connect(&control,&Rcontrol::setArgMax,this,&Rip3d::setArgMax);
    connect(this,&Rip3d::PhMinChanged,&control,&Rcontrol::PhMinChanged);
    connect(&control,&Rcontrol::setPhMin,this,&Rip3d::setPhMin);

    connect(this,&Rip3d::verticalSliderChanged,&control,&Rcontrol::verticalSliderChanged);
    connect(&control,&Rcontrol::setVerticalSlider,this,&Rip3d::on_verticalSlider_valueChanged);

    connect(&control,&Rcontrol::MaxColorValue,this,&Rip3d::MaxColorValue);

    control.init();
    first = false;

}
void Rip3d::MaxColorValue(int val){
    ui->verticalSlider->setMaximum(val);
    ui->colorValue->setMaximum(val);
}
void Rip3d::setArgMin(int val){
    ui->ArgMin->setValue(val);
}
void Rip3d::setArgMax(int val){
    ui->ArgMax->setValue(val);
}
void Rip3d::setPhMin(int val){
    ui->PhMin->setValue(val);
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
void Rip3d::progressTimer(){
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(progressTimerNum()));
    timer->start(60);
    timerOn = true;
}
void Rip3d::progressTimerNum(){
    int pBar = ui->progressBar->value();
    if(pBar < 100)
        pBar++;
    ui->progressBar->setValue(pBar);
}
void Rip3d::progress(int pr){
    if(timerOn)
        timer->stop();
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
}

void Rip3d::on_loadFile_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Файл 3d объекта"), control.getString("lastFileDir"),
            tr("Бинарные файлы (*.dat);;Бинарные файлы (*.3d);;Все файлы (*.*)"));
    if (fileName != "") {
            logLine1("Загрузка файла");
            QFile file(fileName);
            control.setString("lastFileDir",fileName);
            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
                return;
            }
            ui->progressBar->setVisible(true);
            ui->progressBar->setValue(0);
            logLine1("Запись в массив");
            QByteArray data = file.readAll();
            emit operate(data);
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
    if(ui->verticalSlider->value()!=arg1){
        ui->verticalSlider->setValue(arg1);
        if(!first){
            sc->setMax(arg1);
            plotGl->updateSc();
        }
    }
}

void Rip3d::on_scale_valueChanged(int value)
{
    plotGl->setScale(ui->scale->maximum()-value);
}

void Rip3d::on_verticalSlider_valueChanged(int value)
{
    if(ui->colorValue->value()!=value){
        emit verticalSliderChanged(value);
        ui->colorValue->setValue(value);
        if(!first){
            sc->setMax(value);
            plotGl->updateSc();
        }
    }
}

void Rip3d::on_mmSettings_triggered()
{
    control.show("mmSettings");
}

void Rip3d::on_ArgMin_valueChanged(int value)
{
    emit ArgMinChanged(value);
}

void Rip3d::on_ArgMax_valueChanged(int value)
{
    emit ArgMaxChanged(value);
}

void Rip3d::on_PhMin_valueChanged(int value)
{
    emit PhMinChanged(value);
}

void Rip3d::on_mmplotXX_triggered()
{
    control.show("mmPlotXX");
}

void Rip3d::on_mmplotYY_triggered()
{
    control.show("mmPlotYY");
}

void Rip3d::on_mmShowDock_triggered()
{
    control.reInitDock();
}
