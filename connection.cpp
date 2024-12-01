#include "connection.h"
#include <QDebug>
#include <QSqlError>


connection::connection(){}

bool connection::createconnection() {
    db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName("CPP_Project");  // Nom de la source de données ODBC (DSN)
    db.setUserName("mechanic");         // Nom d'utilisateur de la base
    db.setPassword("oracle");           // Mot de passe de la base

    if (db.open()) {
        return true; // Connexion réussie
    } else {
        qDebug() << "Database Error: " << db.lastError().text();
        return false; // Connexion échouée
    }
}



void connection::closeconnection(){
    db.close();
}

