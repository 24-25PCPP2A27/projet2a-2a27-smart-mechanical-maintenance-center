#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include "service.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
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

/*#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include "connection.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    connection conn;  // Instance unique de la classe connection
    if (conn.createconnection()) {
        qDebug() << "Connexion à la base de données réussie.";
        MainWindow w;
        w.show();
        QMessageBox::information(nullptr, QObject::tr("Connexion établie"),
                                 QObject::tr("Connexion à la base réussie.\n"
                                             "Cliquez sur Annuler pour continuer."),
                                 QMessageBox::Cancel);
    } else {
        qDebug() << "Erreur : Échec de la connexion à la base de données.";
        QMessageBox::critical(nullptr, QObject::tr("Erreur de connexion"),
                              QObject::tr("Impossible de se connecter à la base de données.\n"
                                          "Cliquez sur Annuler pour quitter."),
                              QMessageBox::Cancel);
        return -1;  // Arrêt de l'application en cas d'échec de connexion
    }

    int result = a.exec();
    conn.closeconnection();  // Fermeture explicite de la connexion
    return result;
}*/
