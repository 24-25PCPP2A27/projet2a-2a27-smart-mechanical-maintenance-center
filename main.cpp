#include "ServiceMain.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include "service.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServiceMain w;
    connection connection;//une seule instance de la classe connection
    bool test=connection.createconnection();//etablir la connexion
    if(test)
    {w.show();
        QMessageBox::information(nullptr, QObject::tr("database is open"),
                    QObject::tr("connection successful.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}
    else
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                    QObject::tr("connection failed.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);



    return a.exec();
}

