#include "mainwindow.h"
#include <QApplication>
#include "connection.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    connection c;
    bool test=c.createconnection();
    MainWindow w;

    if (test) {
        w.show();
        QMessageBox::information(nullptr,
                                 QObject::tr("Database Connection"),
                                 QObject::tr("Connection successful.\n Click Cancel to exit."),
                                 QMessageBox::Cancel);
    } else {
        QMessageBox::critical(nullptr,
                              QObject::tr("Database Connection"),
                              QObject::tr("Connection failed.\n Click Cancel to exit."),
                              QMessageBox::Cancel);

    }

    return a.exec();
}
