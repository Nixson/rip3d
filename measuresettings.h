#ifndef MEASURESETTINGS_H
#define MEASURESETTINGS_H

#include <QDialog>

namespace Ui {
class MeasureSettings;
}

class MeasureSettings : public QDialog
{
    Q_OBJECT

public:
    explicit MeasureSettings(QWidget *parent = 0);
    ~MeasureSettings();

private:
    Ui::MeasureSettings *ui;
};

#endif // MEASURESETTINGS_H
