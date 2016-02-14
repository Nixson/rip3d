#include "plotter.h"
#include "ui_plotter.h"

Plotter::Plotter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Plotter)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    MaxColor = 255;
    Position = 0;
    AngleMin = 0;
    AngleMax = 360;
    OffsetMin = 0;
    OffsetMax = 1024;
    Lay = 0;
    Accum = 0;
    Size = 0;
    customPlot = ui->plot;
    customPlot->setInteractions(QCP::iRangeZoom); //QCP::iRangeDrag| this will also allow rescaling the color scale by dragging/zooming
    customPlot->axisRect()->setupFullAxesBox(true);
    customPlot->xAxis->setLabel("Градусы");
    customPlot->yAxis->setLabel("Отсчеты");
    colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(colorMap);
    plot();
}

Plotter::~Plotter()
{
    delete ui;
}
void Plotter::ShowDataVector(IntVector &amp,IntVector &ph){
    Amp = amp;
    Phase = ph;
    Size = Amp.size()/BLOCKLANGTH;
    plot();
}
void Plotter::MaxColorValue(int color){
    MaxColor = color;
    plot();
}
void Plotter::setMax(unsigned int b){
    Position = b;
    plot();
}
void Plotter::setAngle(int min,int max){
    AngleMin = min+180;
    AngleMax = max+180;
    plot();
}
void Plotter::setOffset(unsigned int min,unsigned int max){
    OffsetMin = min;
    OffsetMax = max;
    plot();
}
void Plotter::plot(){
    if(this->isHidden()) return;
    if(Size == 0) return;
    // configure axis rect:

    // set up the QCPColorMap:
    int nx = (int)AngleMax-AngleMin;
    int ny = OffsetMax-OffsetMin;
    colorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
    colorMap->data()->setRange(QCPRange((int)AngleMin-180, (int)AngleMax-180), QCPRange(OffsetMin, OffsetMax)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions
    int Acc = Accum;
    if(Acc == 0)
        Acc = 1;
    double x, y;
    for (int xIndex=0; xIndex<nx; ++xIndex)
    {
      for (int yIndex=0; yIndex<ny; ++yIndex)
      {
        colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);
        unsigned int rY = (unsigned int)(x+180);
        int rZ = (int)round(y);

        double A = 0;
        unsigned int P = 0;
        int Step = 0;
        for(P = Lay, Step = 0; Step <= Acc && P < Size; Step++, P++){
            if(Phase[P*BLOCKLANGTH+rZ] == rY){
                double AmpR = (double)Amp[P*BLOCKLANGTH+rZ];
                if(AmpR >= Position)
                    A+= sqrt(AmpR);
            }
        }
        colorMap->data()->setCell(xIndex, yIndex, A);
      }
    }
    colorMap->setGradient(QCPColorGradient::gpThermal);
//    colorMap->setGradient(QCPColorGradient::gpGrayscale);
    colorMap->rescaleDataRange();
    customPlot->rescaleAxes();
    customPlot->replot();
}

void Plotter::on_spinBox_valueChanged(int arg1)
{
    Lay = (unsigned int) arg1;
    plot();
}

void Plotter::on_spinBox_2_valueChanged(int arg1)
{
    Accum = (unsigned int) arg1;
    plot();
}
