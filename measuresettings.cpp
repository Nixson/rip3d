#include "measuresettings.h"
#include "ui_measuresettings.h"

MeasureSettings::MeasureSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MeasureSettings)
{
    ui->setupUi(this);
}

MeasureSettings::~MeasureSettings()
{
    delete ui;
}
