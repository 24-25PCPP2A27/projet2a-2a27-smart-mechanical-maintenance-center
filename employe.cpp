#include "employe.h"
#include <QDebug>
#include <QSqlError>

employe::employe(int ID, QString NOM, QString PRENOM, QString POSTE, int SALAIRE, int DUREE)
    : ID(ID), NOM(NOM), PRENOM(PRENOM), POSTE(POSTE), SALAIRE(SALAIRE), DUREE(DUREE) {}

bool employe::ajouter() {
    QSqlQuery query;
    QString res = QString::number(ID);

    query.prepare("INSERT INTO employee(id, NOM, PRENOM, POSTE, SALAIRE, DUREE) "
                  "VALUES (:id, :NOM, :PRENOM, :POSTE, :SALAIRE, :DUREE)");

    query.bindValue(":id", res);
    query.bindValue(":NOM", NOM);
    query.bindValue(":PRENOM", PRENOM);
    query.bindValue(":POSTE", POSTE);
    query.bindValue(":SALAIRE", SALAIRE);
    query.bindValue(":DUREE", DUREE);  // Bind DUREE as an int

    if (!query.exec()) {
        qDebug() << "Add Employee Error:" << query.lastError().text();
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
    return model;
}

bool employe::supprimer(int id) {
    QSqlQuery query;
    QString res = QString::number(id);
    query.prepare("DELETE FROM employee WHERE id = :id");
    query.bindValue(":id", res);

    if (!query.exec()) {
        qDebug() << "Delete employee Error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool employe::update(int id, QString NOM, QString PRENOM, QString POSTE, int SALAIRE, int DUREE) {
    QSqlQuery query;
    query.prepare("UPDATE employee SET NOM = :NOM, PRENOM = :PRENOM, POSTE = :POSTE, "
                  "SALAIRE = :SALAIRE, DUREE = :DUREE WHERE id = :id");

    query.bindValue(":id", id);
    query.bindValue(":NOM", NOM);
    query.bindValue(":PRENOM", PRENOM);
    query.bindValue(":POSTE", POSTE);
    query.bindValue(":SALAIRE", SALAIRE);
    query.bindValue(":DUREE", DUREE);  // Bind DUREE as an int

    if (!query.exec()) {
        qDebug() << "Update Employee Error:" << query.lastError().text();
        return false;
    }
    return true;
}
