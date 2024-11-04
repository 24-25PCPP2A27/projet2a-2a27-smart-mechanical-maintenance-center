#include "service.h"
#include <QSqlError>
#include <QVariant>
#include <QMessageBox>

// Constructeur
Service::Service(int ids, QString type, QString duree, QString equipment, QString etats, QString cout)
    : IDS(ids), TYPE(type), DUREE(duree), EQUIPMENT(equipment), ETATS(etats), COUT(cout) {}

// ajouter un service
bool Service::Ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO services (IDS, TYPE, DUREE, EQUIPMENT, ETATS, COUT) VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(IDS);
    query.addBindValue(TYPE);
    query.addBindValue(DUREE);
    query.addBindValue(EQUIPMENT);
    query.addBindValue(ETATS);
    query.addBindValue(COUT);

   if (!query.exec()) {
        // Gérer l'erreur
        QMessageBox::warning(nullptr, "Erreur d'ajout", query.lastError().text());
        return false;
    }

    return true;
    return query.exec();
}


// Méthode pour afficher tous les services
QSqlQueryModel* Service::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query("SELECT * FROM services");
    query.exec();
    model->setQuery(query);
    return model; // Retourne le modèle de la requête
}

// Méthode pour supprimer un service par ID
bool Service::supprimer(int ids) {
    QSqlQuery query;
    query.prepare("DELETE FROM services WHERE IDS = ?");
    query.addBindValue(ids);

    if (!query.exec()) {
        // Gérer l'erreur
        QMessageBox::warning(nullptr, "Erreur de suppression", query.lastError().text());
        return false;
    }
    return true;
}

// Méthode pour mettre à jour un service
bool Service::update(int ids, QString type, QString duree, QString equipment, QString etats, QString cout) {
    QSqlQuery query;
    query.prepare("UPDATE services SET TYPE = ?, DUREE = ?, EQUIPMENT = ?, ETATS = ?, COUT = ? WHERE IDS = ?");
    query.addBindValue(type);
    query.addBindValue(duree);
    query.addBindValue(equipment);
    query.addBindValue(etats);
    query.addBindValue(cout);
    query.addBindValue(ids);

    if (!query.exec()) {
        // Gérer l'erreur
        QMessageBox::warning(nullptr, "Erreur de mise à jour", query.lastError().text());
        return false;
    }
    return true;
}

