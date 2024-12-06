#include "ServiceMain.h"
#include <QApplication>
#include "connection.h"
#include "arduino.h"
#include "service.h"
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    ServiceMain w;
    connection c;

    // Check database connection
    bool test = c.createconnection();
    if (!test) {
        QMessageBox::critical(nullptr,
                              QObject::tr("Erreur de connexion à la base de données"),
                              QObject::tr("Échec de la connexion à la base de données.\nCliquez sur Annuler pour quitter."),
                              QMessageBox::Cancel);
        return -1;
    }

    QMessageBox::information(nullptr,
                             QObject::tr("Connexion réussie"),
                             QObject::tr("Connexion à la base de données réussie .\nCliquez sur OK pour continuer."),
                             QMessageBox::Ok);

    w.show();
    return a.exec();
}
