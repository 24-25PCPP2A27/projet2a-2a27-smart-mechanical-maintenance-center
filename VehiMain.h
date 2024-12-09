#ifndef VEHIMAIN_H
#define VEHIMAIN_H

#include <QVehiMain>

QT_BEGIN_NAMESPACE
namespace Ui {
class VehiMain;
}
QT_END_NAMESPACE

class VehiMain : public QVehiMain
{
    Q_OBJECT

public:
    explicit VehiMain(QWidget *parent = nullptr);
    ~VehiMain();

private:
    Ui::VehiMain *ui;
};
#endif // VEHIMAIN_H
