#include "VehiMain.h"
#include "connexion.h"
#include "vehiculeui.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //VehiMain w;
    vehiculeUI vUI;
    connexion c;
    bool test =c.createconnect();
    if(test)
    {
        //w.show();
        vUI.exec();
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                              QObject::tr("connexion failed.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);

    return a.exec();
}
