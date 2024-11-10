#include "employe.h"
#include <QDebug>
#include <QSqlError>
#include <QRegularExpression>

employe::employe(int ID, QString NOM, QString PRENOM, QString POSTE, int SALAIRE, int DUREE)
    : ID(ID), NOM(NOM), PRENOM(PRENOM), POSTE(POSTE), SALAIRE(SALAIRE), DUREE(DUREE) {}

// Helper function to validate input
bool employe::validateInputs() const {
    QRegularExpression numericRegex("^[0-9]+$");  // Only digits
    QRegularExpression alphaRegex("^[a-zA-Z]+$");  // Only letters

    if (!numericRegex.match(QString::number(ID)).hasMatch() || ID <= 0) {
        qDebug() << "Validation Error: ID doit etre un numero positive.";
        return false;
    }
    if (!alphaRegex.match(NOM).hasMatch() || NOM.length() > 20) {
        qDebug() << "Validation Error: Nom doit etre alphabetique et moins de 20 character.";
        return false;
    }
    if (!alphaRegex.match(PRENOM).hasMatch() || PRENOM.length() > 20) {
        qDebug() << "Validation Error: Prenom doit etre alphabetique et moins de 20 character.";
        return false;
    }
    if (!alphaRegex.match(POSTE).hasMatch() || POSTE.length() > 30) {
        qDebug() << "Validation Error: Poste doit etre alphabetique et moins de 30 character.";
        return false;
    }
    if (!numericRegex.match(QString::number(SALAIRE)).hasMatch() || SALAIRE < 0) {
        qDebug() << "Validation Error: Salaire doit etre un numero positive.";
        return false;
    }
    if (!numericRegex.match(QString::number(DUREE)).hasMatch() || DUREE < 0) {
        qDebug() << "Validation Error: Date_Embauche doit etre un numero positive.";
        return false;
    }
    return true;
}
QSqlQueryModel* employe::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM employee");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("POSTE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("SALAIRE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("DUREE"));

    return model;
}
bool employe::ajouter() {
    if (!validateInputs()) {
        return false;  // Stop if inputs are not valid
    }

    QSqlQuery query;
    query.prepare("INSERT INTO employee(id, NOM, PRENOM, POSTE, SALAIRE, DUREE) "
                  "VALUES (:id, :NOM, :PRENOM, :POSTE, :SALAIRE, :DUREE)");

    query.bindValue(":id", ID);
    query.bindValue(":NOM", NOM);
    query.bindValue(":PRENOM", PRENOM);
    query.bindValue(":POSTE", POSTE);
    query.bindValue(":SALAIRE", SALAIRE);
    query.bindValue(":DUREE", DUREE);

    if (!query.exec()) {
        qDebug() << "Add Employee Error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool employe::update(int id, QString NOM, QString PRENOM, QString POSTE, int SALAIRE, int DUREE) {
    if (id <= 0 || NOM.isEmpty() || PRENOM.isEmpty() || POSTE.isEmpty() || SALAIRE < 0 || DUREE < 0) {
        qDebug() << "Update Error: Invalid input data.";
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE employee SET NOM = :NOM, PRENOM = :PRENOM, POSTE = :POSTE, "
                  "SALAIRE = :SALAIRE, DUREE = :DUREE WHERE id = :id");

    query.bindValue(":id", id);
    query.bindValue(":NOM", NOM);
    query.bindValue(":PRENOM", PRENOM);
    query.bindValue(":POSTE", POSTE);
    query.bindValue(":SALAIRE", SALAIRE);
    query.bindValue(":DUREE", DUREE);

    if (!query.exec()) {
        qDebug() << "Update Employee Error:" << query.lastError().text();
        return false;
    }
    return true;
}
bool employe::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM employee WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Delete Employee Error:" << query.lastError().text();
        return false;
    }
    return true;
}
