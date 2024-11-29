#include "service.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPrinter>
#include <QTextDocument>
#include <QDesktopServices>
#include <QUrl>
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
    QString pdfFileName = "Services.pdf";
    printer.setOutputFileName(pdfFileName);

    QTextDocument document;
    QString html = "<html><head><style>"
                   "table { border-collapse: collapse; width: 100%; }"
                   "th, td { border: 1px solid black; padding: 8px; text-align: left; }"
                   "th { background-color: #f2f2f2; }"
                   "</style></head><body>";
    html += "<h1 style='text-align: center;'>Services List</h1>";
    html += "<table>";
    html += "<tr>"
            "<th>ID</th>"
            "<th>Type</th>"
            "<th>Durée</th>"
            "<th>État</th>"
            "<th>Coût</th>"
            "</tr>";

    QSqlQuery query;
    if (!query.exec("SELECT idserv, type, duree, etats, cout FROM services")) {
        qDebug() << "Query execution failed:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        html += QString("<tr>"
                        "<td>%1</td>"
                        "<td>%2</td>"
                        "<td>%3</td>"
                        "<td>%4</td>"
                        "<td>%5</td>"
                        "</tr>")
                    .arg(query.value("idserv").toString())
                    .arg(query.value("type").toString())
                    .arg(query.value("duree").toString())
                    .arg(query.value("etats").toString())
                    .arg(query.value("cout").toString());
    }

    html += "</table></body></html>";
    document.setHtml(html);
    document.print(&printer);

    qDebug() << "PDF generated successfully: " << pdfFileName;

    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(pdfFileName))) {
        qDebug() << "Failed to open the PDF automatically.";
    }
}

QMap<QString, int> Service::statistiquesParEtats() {
    QMap<QString, int> stats;
    QSqlQuery query;
    if (!query.exec("SELECT etats, COUNT(*) as count FROM services GROUP BY etats")) {
        qDebug() << "Failed to execute statistics query:" << query.lastError().text();
        return stats;
    }

    while (query.next()) {
        QString etat = query.value("etats").toString();
        int count = query.value("count").toInt();
        stats.insert(etat, count);
    }

    return stats;
}
