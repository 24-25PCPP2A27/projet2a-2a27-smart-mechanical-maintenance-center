#include "service.h"
#include <QPdfWriter>
#include <QPainter>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlQuery>

Service::Service() : id(0), cout(0.0), duree(0.0), etats("en cours") {}

Service::Service(int id, double cout, QString type, double duree, QString etats)
    : id(id), cout(cout), type(type), duree(duree), etats(etats) {}


QSqlQueryModel* Service::obtenirStatistiquesService() {
    QSqlQueryModel* model = new QSqlQueryModel();

    // Requête SQL pour obtenir l'état des services et le nombre de services pour chaque état
    QSqlQuery query;
    query.prepare("SELECT ETATS, COUNT(*) as nombre FROM SERVICES GROUP BY ETATS");

    // Exécution de la requête et vérification du succès
    if (!query.exec()) {
        qWarning() << "Query failed:" << query.lastError().text();  // Journaliser en cas d'erreur
        return model;  // Retourner un modèle vide en cas d'erreur
    }

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nombre"));

    return model;
}
int Service::getId() const { return id; }
double Service::getCout() const { return cout; }
QString Service::getType() const { return type; }
double Service::getDuree() const { return duree; }
QString Service::getEtats() const { return etats; }

void Service::setId(int id) { this->id = id; }
void Service::setCout(double cout) { this->cout = cout; }
void Service::setType(QString type) { this->type = type; }
void Service::setDuree(double duree) { this->duree = duree; }
void Service::setEtats(QString etats) { this->etats = etats; }

bool Service::ajouter(int idserv, const QString &type, const QString &duree, const QString &etats, float cout) {
    QSqlQuery query;

    // Vérification des données en entrée (débogage)
    qDebug() << "Tentative d'ajout :"
             << "IDSERV:" << idserv
             << "TYPE:" << type
             << "DUREE:" << duree
             << "ETATS:" << etats
             << "COUT:" << cout;

    // Préparation de la requête
    query.prepare("INSERT INTO services (IDSERV, TYPE, DUREE, ETATS, COUT) "
                  "VALUES (:IDSERV, :TYPE, :DUREE, :ETATS, :COUT)");

    query.bindValue(":IDSERV", idserv);
    query.bindValue(":TYPE", type.trimmed()); // Supprime les espaces inutiles
    query.bindValue(":DUREE", duree.trimmed()); // Assure que la valeur est une chaîne valide
    query.bindValue(":ETATS", etats.trimmed());
    query.bindValue(":COUT", cout);

    if (!query.exec()) {
        qDebug() << "Erreur SQL : " << query.lastError().text();
        return false; // Échec
    }

    return true; // Succès
}

QSqlQueryModel* Service::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM services");
    return model;
}

bool Service::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM services WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Service::modifier(int id) {
    QSqlQuery query;
    query.prepare("UPDATE services SET cout = :cout, type = :type, duree = :duree, etats = :etats WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":cout", cout);
    query.bindValue(":type", type);
    query.bindValue(":duree", duree);
    query.bindValue(":etats", etats);
    return query.exec();
}

QSqlQueryModel* Service::rechercher(const QString& keyword) {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM services WHERE type LIKE '%" + keyword + "%' OR etats LIKE '%" + keyword + "%'");
    return model;
}

QSqlQueryModel* Service::trier(const QString& critere, const QString& ordre) {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM services ORDER BY " + critere + " " + ordre);
    return model;
}

/*void Service::genererPDF() {
    QPdfWriter pdf("services.pdf");
    QPainter painter(&pdf);

    QSqlQuery query("SELECT * FROM services");
    int y = 400;

    painter.drawText(200, 200, "Liste des services");
    while (query.next()) {
        QString line = QString("ID: %1 | Type: %2 | Coût: %3 | Durée: %4 | État: %5")
                           .arg(query.value(0).toString())
                           .arg(query.value(2).toString())
                           .arg(query.value(1).toDouble())
                           .arg(query.value(3).toDouble())
                           .arg(query.value(4).toString());
        painter.drawText(200, y, line);
        y += 200;
    }
}*/
void Service::genererPDF() {
    QPdfWriter pdf("services.pdf");
    QPainter painter(&pdf);

    // Vérifier si la création du fichier PDF a échoué
    if (!painter.isActive()) {
        qWarning() << "Échec de l'ouverture du PDF";
        return;
    }

    // Effectuer la requête SQL
    QSqlQuery query("SELECT * FROM services");
    if (!query.exec()) {
        qWarning() << "Échec de la requête SQL:" << query.lastError().text();
        return;
    }

    int y = 400;
    painter.drawText(200, 200, "Liste des services");
    while (query.next()) {
        QString line = QString("ID: %1 | Type: %2 | Coût: %3 | Durée: %4 | État: %5")
                           .arg(query.value(0).toString())
                           .arg(query.value(2).toString())
                           .arg(query.value(1).toDouble())
                           .arg(query.value(3).toDouble())
                           .arg(query.value(4).toString());
        painter.drawText(200, y, line);
        y += 200;
    }
}

