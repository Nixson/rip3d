#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include "cloudtypelist.h"
#include "qcustomplot.h"

namespace Ui {
class Plotter;
}

class Plotter : public QWidget
{
    Q_OBJECT

public:
    explicit Plotter(QWidget *parent = 0);
    ~Plotter();
public slots:
    void ShowDataVector(IntVector &amp,IntVector &ph);
    void MaxColorValue(int color);
    void setMax(unsigned int b);
    void setAngle(int min,int max);
    void setOffset(unsigned int min,unsigned int max);
    void plot();

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::Plotter *ui;
    IntVector Amp;
    IntVector Phase;
    int MaxColor;
    unsigned int Position;
    int AngleMin;
    int AngleMax;
    unsigned int OffsetMin;
    unsigned int OffsetMax;
    unsigned int Lay;
    unsigned int Accum;
    unsigned int Size;
    QCustomPlot *customPlot;
    QCPColorMap *colorMap;
};

#endif // PLOTTER_H
