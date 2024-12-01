
#include "ServiceMain.h"
#include <QApplication>
#include "connection.h"
#include "arduino.h"
#include "service.h"
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServiceMain w;
    connection c;

    // Vérifier la connexion à la base de données
    bool test = c.createconnection();
    if (!test) {
        QMessageBox::critical(nullptr,
                              QObject::tr("Erreur de connexion à la base de données"),
                              QObject::tr("Échec de la connexion à la base de données.\nCliquez sur Annuler pour quitter."),
                              QMessageBox::Cancel);
        return -1; // Quitter l'application si la connexion échoue
    }

    Arduino arduino;
    // Tentez de connecter l'Arduino
    if (arduino.connect_arduino() == 0) {
        qDebug() << "Arduino connecté avec succès !";
    } else {
        qDebug() << "Échec de la connexion Arduino.";
        QMessageBox::critical(nullptr,
                              QObject::tr("Erreur de connexion"),
                              QObject::tr("Échec de la connexion à Arduino.\nAssurez-vous que le périphérique est bien connecté."),
                              QMessageBox::Cancel);
        return -1; // Quitter l'application si la connexion échoue
    }

    // Exemple d'utilisation de la classe Service
    Service service;
    QString etatequip = "en panne"; // Exemple de valeur initiale pour l'état de l'équipement
    service.setEtatEquipement(etatequip);

    QMessageBox::information(nullptr,
                             QObject::tr("Connexion réussie"),
                             QObject::tr("Connexion à la base de données réussie et Arduino connecté.\nCliquez sur OK pour continuer."),
                             QMessageBox::Ok);

    w.show();
    return a.exec(); // Lancer l'application
}

