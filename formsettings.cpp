#include "formsettings.h"
#include "ui_formsettings.h"

formSettings::formSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formSettings)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);

}

formSettings::~formSettings()
{
    delete ui;
}

void formSettings::on_leSubBufNum_valueChanged(int arg1)
{
    emit leSubBufNum(arg1);
}

void formSettings::on_leBurstLen_valueChanged(int arg1)
{
    emit leBurstLen(arg1);
}
void formSettings::setleSubBufNum(int value){
    ui->leSubBufNum->setValue(value);
}
void formSettings::setleBurstLen(int value){
    ui->leBurstLen->setValue(value);
}

void formSettings::on_leGeterodin_valueChanged(int arg1)
{
    emit leGeterodin(arg1);
}

void formSettings::on_leFreq_valueChanged(int arg1)
{
    emit leFreq(arg1);
}

void formSettings::on_leAmp_valueChanged(int arg1)
{
    emit leAmp(arg1);
}

void formSettings::on_cbPulseMod_toggled(bool checked)
{
    emit cbPulseMod(checked);
}

void formSettings::on_cbUWB_toggled(bool checked)
{
    emit cbUWB(checked);
}

void formSettings::on_cbLFM_toggled(bool checked)
{
    emit cbLFM(checked);
}

void formSettings::on_lePeriod_valueChanged(double arg1)
{
    emit lePeriod(arg1);
}

void formSettings::on_leDuration_valueChanged(double arg1)
{
    emit leDuration(arg1);
}

void formSettings::on_leFreqRange_valueChanged(int arg1)
{
    emit leFreqRange(arg1);
}

void formSettings::on_cbGate_toggled(bool checked)
{
    emit cbGate(checked);
}

void formSettings::on_leGateDelay_valueChanged(int arg1)
{
    emit leGateDelay(arg1);
}

void formSettings::on_leGateDuration_valueChanged(double arg1)
{
    emit leGateDuration(arg1);
}

void formSettings::on_cbCont_toggled(bool checked)
{
    emit cbCont(checked);
}

void formSettings::on_cbCoherentAccum_toggled(bool checked)
{
    emit cbCoherentAccum(checked);
}

void formSettings::on_cbDDSReset_toggled(bool checked)
{
    emit cbDDSReset(checked);
}

void formSettings::on_cbLOGM_toggled(bool checked)
{
    emit cbLOGM(checked);
}

void formSettings::on_rbDdsRstBurst_toggled(bool checked)
{
    emit rbDdsRstBurst(checked);
}

void formSettings::on_rbDdsRstPulse_toggled(bool checked)
{
    emit rbDdsRstPulse(checked);
}

void formSettings::setleGeterodin(int val){
    ui->leGeterodin->setValue(val);
}
void formSettings::setleFreq(int val){
    ui->leFreq->setValue(val);
}
void formSettings::setleAmp(int val){
    ui->leAmp->setValue(val);
}
void formSettings::setcbPulseMod(bool val){
    ui->cbPulseMod->setChecked(val);
}
void formSettings::setcbUWB(bool val){
    ui->cbUWB->setChecked(val);
}
void formSettings::setcbLFM(bool val){
    ui->cbLFM->setChecked(val);
}
void formSettings::setlePeriod(double val){
    ui->lePeriod->setValue(val);
}
void formSettings::setleDuration(double val){
    ui->leDuration->setValue(val);
}
void formSettings::setleFreqRange(int val){
    ui->leFreqRange->setValue(val);
}
void formSettings::setcbGate(bool val){
    ui->cbGate->setChecked(val);
}
void formSettings::setleGateDelay(int val){
    ui->leGateDelay->setValue(val);
}
void formSettings::setleGateDuration(double val){
    ui->leGateDuration->setValue(val);
}
void formSettings::setcbCont(bool val){
    ui->cbCont->setChecked(val);
}
void formSettings::setcbCoherentAccum(bool val){
    ui->cbCoherentAccum->setChecked(val);
}
void formSettings::setcbDDSReset(bool val){
    ui->cbDDSReset->setChecked(val);
}
void formSettings::setcbLOGM(bool val){
    ui->cbLOGM->setChecked(val);
}
void formSettings::setrbDdsRstBurst(bool val){
    ui->rbDdsRstBurst->setChecked(val);
}
void formSettings::setrbDdsRstPulse(bool val){
    ui->rbDdsRstPulse->setChecked(val);
}



