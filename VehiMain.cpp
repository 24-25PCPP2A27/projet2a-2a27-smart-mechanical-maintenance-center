#include "VehiMain.h"
#include "ui_VehiMain.h"


VehiMain::VehiMain(QWidget *parent)
    : QVehiMain(parent)
    , ui(new Ui::VehiMain)
{
    ui->setupUi(this);

}

VehiMain::~VehiMain()
{
    delete ui;
}


