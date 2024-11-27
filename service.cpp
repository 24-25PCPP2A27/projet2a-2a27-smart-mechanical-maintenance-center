/*#include "service.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>

Service::Service() : id(0), cout(0), duree(0), type(""), etats("") {}

Service::Service(int id, double cout, QString type, double duree, QString etats)
    : id(id), cout(cout), type(type), duree(duree), etats(etats) {}

bool Service::ajouter(int idserv, const QString &type, double duree, const QString &etats, float cout) {
    QSqlQuery query;
    query.prepare("INSERT INTO services (id, type, duree, etats, cout) VALUES (:id, :type, :duree, :etats, :cout)");
    query.bindValue(":id", idserv);
    query.bindValue(":type", type);
    query.bindValue(":duree", duree);
    query.bindValue(":etats", etats);
    query.bindValue(":cout", cout);
    return query.exec();
}

bool Service::modifier(int id, const QString &type, double duree, double cout) {
    QSqlQuery query;
    query.prepare("UPDATE services SET type = :type, duree = :duree, cout = :cout WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":type", type);
    query.bindValue(":duree", duree);
    query.bindValue(":cout", cout);
    return query.exec();
}

bool Service::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM services WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

QSqlQueryModel* Service::rechercher(const QString &keyword) {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM services WHERE id LIKE :keyword OR type LIKE :keyword");
    query.bindValue(":keyword", "%" + keyword + "%");
    query.exec();
    model->setQuery(query);
    return model;
}

QSqlQueryModel* Service::trier(const QString &critere, const QString &ordre) {
    QSqlQueryModel *model = new QSqlQueryModel();
    QString queryStr = QString("SELECT * FROM services ORDER BY %1 %2").arg(critere).arg(ordre);
    QSqlQuery query;
    query.exec(queryStr);
    model->setQuery(query);
    return model;
}

QSqlQueryModel* Service::obtenirStatistiquesService() {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query("SELECT COUNT(*) AS total_services, AVG(cout) AS avg_cost, AVG(duree) AS avg_duration FROM services");
    query.exec();
    model->setQuery(query);
    return model;
}
*/
#include "service.h"
#include <QPdfWriter>
#include <QPainter>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QPainter>
#include <QPrinter>

Service::Service() : idserv(0), coutserv(0.0), dureeserv(0.0), etatsserv("en cours") {}

Service::Service(int idserv, double coutserv, QString typeserv, double dureeserv, QString etatsserv)
    : idserv(idserv), coutserv(coutserv), typeserv(typeserv), dureeserv(dureeserv), etatsserv(etatsserv) {}



int Service::getId() const { return idserv ; }
double Service::getCout() const { return coutserv ; }
QString Service::getType() const { return typeserv ; }
double Service::getDuree() const { return dureeserv ; }
QString Service::getEtats() const { return etatsserv ; }

void Service::setId(int idserv ) { this->idserv  = idserv ; }
void Service::setCout(double coutserv ) { this->coutserv = coutserv ; }
void Service::setType(QString typeserv) { this->typeserv  = typeserv; }
void Service::setDuree(double dureeserv ) { this->dureeserv = dureeserv ; }
void Service::setEtats(QString etatsserv ) { this->etatsserv = etatsserv ; }

bool Service::ajouter(int idserv, const QString &typeserv, const QString &dureeserv , const QString &etatsserv, float coutserv ) {
    QSqlQuery query;

    // Vérification des données en entrée (débogage)
    qDebug() << "Tentative d'ajout :"
             << "IDSERV:" << idserv
             << "TYPESERV:" << typeserv
             << "DUREESERV:" << dureeserv
             << "ETATSSERV:" << etatsserv
             << "COUTSERV:" << coutserv ;

    // Préparation de la requête
    query.prepare("INSERT INTO services (IDSERV, TYPE, DUREE, ETATS, COUT) "
                  "VALUES (:IDSERV, :TYPE, :DUREE, :ETATS, :COUT)");

    query.bindValue(":IDSERV", idserv);
    query.bindValue(":TYPE", typeserv.trimmed()); // Supprime les espaces inutiles
    query.bindValue(":DUREE", dureeserv.trimmed()); // Assure que la valeur est une chaîne valide
    query.bindValue(":ETATS", etatsserv.trimmed());
    query.bindValue(":COUT", coutserv );

    if (!query.exec()) {
        qDebug() << "Erreur SQL : " << query.lastError().text();
        return false; // Échec
    }

    return true; // Succès
}

QSqlQueryModel* Service::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM services"); // Ensure this query matches your table
    return model;
}

bool Service::supprimer(int idserv ) {
    QSqlQuery query;
    query.prepare("DELETE FROM services WHERE id = :id");
    query.bindValue(":id", idserv);
    return query.exec();
}

bool Service::modifier(int idserv ) {
    QSqlQuery query;
    query.prepare("UPDATE services SET cout = :cout, type = :type, duree = :duree, etats = :etats WHERE id = :id");
    query.bindValue(":id", idserv);
    query.bindValue(":cout", coutserv);
    query.bindValue(":type", typeserv );
    query.bindValue(":duree", dureeserv );
    query.bindValue(":etats", etatsserv );
    return query.exec();
}

QSqlQueryModel* Service::rechercher(int idrech) {
    QSqlQueryModel* model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare("SELECT * FROM services WHERE idserv = :idserv");
    query.bindValue(":idserv", idrech); // Correction ici

    if (query.exec()) {
        model->setQuery(query);
    } else {
        qDebug() << "Erreur lors de la recherche :" << query.lastError().text();
    }

    return model;
}






QSqlQueryModel* Service::trier(const QString& critere, const QString& ordre) {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM services ORDER BY " + critere + " " + ordre);
    return model;
}

void Service::genererPDF() {
   QPrinter printer(QPrinter::HighResolution);
   printer.setOutputFormat(QPrinter::PdfFormat);
   printer.setOutputFileName("Services.pdf");

   QPainter painter(&printer);
   int y = 0;

   QSqlQuery query("SELECT * FROM services");
   while (query.next()) {
       QString line = QString("ID: %1 | Type: %2 | Durée: %3 | État: %4 | Coût: %5")
           .arg(query.value("idserv").toString())
           .arg(query.value("type").toString())
           .arg(query.value("duree").toString())
           .arg(query.value("etats").toString())
           .arg(query.value("cout").toString());
       painter.drawText(100, y += 50, line);
   }

   painter.end();
}

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





