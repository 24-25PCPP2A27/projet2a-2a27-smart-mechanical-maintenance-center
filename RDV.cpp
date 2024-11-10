#include "RDV.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// Constructor
rdv::rdv(int id, int jour, int mois, int annee, int heure, const QString &adresse)
    : IDR(id), JOUR(jour), MOIS(mois), ANNEE(annee), HEURE(heure), ADRESSE(adresse) {}

// Getter methods
int rdv::getIDR() const { return IDR; }
int rdv::getJOUR() const { return JOUR; }
int rdv::getMOIS() const { return MOIS; }
int rdv::getANNEE() const { return ANNEE; }
int rdv::getHEURE() const { return HEURE; }
QString rdv::getADRESSE() const { return ADRESSE; }

// Method to add a new RDV
bool rdv::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO rdv (IDR, JOUR, MOIS, ANNEE, HEURE, ADRESSE) "
                  "VALUES (:idr, :jour, :mois, :annee, :heure, :adresse)");
    query.bindValue(":idr", IDR);
    query.bindValue(":jour", JOUR);
    query.bindValue(":mois", MOIS);
    query.bindValue(":annee", ANNEE);
    query.bindValue(":heure", HEURE);
    query.bindValue(":adresse", ADRESSE);

    if (!query.exec()) {
        qDebug() << "Error adding RDV:" << query.lastError();
        return false;
    }
    return true;
}

// Method to delete an RDV by ID
bool rdv::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM rdv WHERE IDR = :idr");
    query.bindValue(":idr", id);

    if (!query.exec()) {
        qDebug() << "Error deleting RDV:" << query.lastError();
        return false;
    }
    return true;
}

// Method to update an existing RDV
bool rdv::update(int id, int jour, int mois, int annee, int heure, const QString &adresse) {
    QSqlQuery query;
    query.prepare("UPDATE rdv SET JOUR = :jour, MOIS = :mois, ANNEE = :annee, HEURE = :heure, ADRESSE = :adresse "
                  "WHERE IDR = :idr");
    query.bindValue(":idr", id);
    query.bindValue(":jour", jour);
    query.bindValue(":mois", mois);
    query.bindValue(":annee", annee);
    query.bindValue(":heure", heure);
    query.bindValue(":adresse", adresse);

    if (!query.exec()) {
        qDebug() << "Error updating RDV:" << query.lastError();
        return false;
    }
    return true;
}

