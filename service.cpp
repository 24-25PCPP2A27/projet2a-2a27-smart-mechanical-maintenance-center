#include "service.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPrinter>
#include <QTextDocument>
#include <QDesktopServices>
#include <QUrl>

Service::Service() : idserv(0), coutserv(0), dureeserv(0), etatsserv(""), typeserv("") {}

Service::Service(int idserv, double coutserv, QString typeserv, double dureeserv, QString etatsserv)
    : idserv(idserv), coutserv(coutserv), typeserv(typeserv), dureeserv(dureeserv), etatsserv(etatsserv) {}

bool Service::ajouter(int idserv, const QString &typeserv, const QString &dureeserv, const QString &etatsserv, float coutserv) {
    QSqlQuery query;
    query.prepare("INSERT INTO services (IDSERV, TYPESERV, DUREESERV, ETATSERV, COUTSERV) "
                  "VALUES (:idserv, :typeserv, :dureeserv, :etatsserv, :coutserv)");

    // Bind values to the query
    query.bindValue(":idserv", idserv);  // Integer
    query.bindValue(":typeserv", typeserv);  // String (VARCHAR2)
    query.bindValue(":dureeserv", dureeserv);  // String (VARCHAR2) - No need to convert if it's stored as VARCHAR2
    query.bindValue(":etatsserv", etatsserv);  // String (VARCHAR2)
    query.bindValue(":coutserv", QString::number(coutserv));  // String (VARCHAR2) - Convert float to string

    // Execute the query and check for errors
    if (!query.exec()) {
        qDebug() << "Error inserting service: " << query.lastError().text();
        return false;
    }
    return true;
}





QSqlQueryModel* Service::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery query("SELECT * FROM services");
    model->setQuery(query);
    return model;
}

bool Service::supprimer(int idserv)
{
    QSqlQuery query;
    query.prepare("DELETE FROM services WHERE idserv = :idserv");
    query.bindValue(":idserv", idserv);
    return query.exec();
}

bool Service::modifier(int idserv)
{
    QSqlQuery query;
    query.prepare("UPDATE services SET typeserv = :typeserv, dureeserv = :dureeserv, etatsserv = :etatsserv, coutserv = :coutserv WHERE idserv = :idserv");
    query.bindValue(":idserv", idserv);
    query.bindValue(":typeserv", typeserv);
    query.bindValue(":dureeserv", dureeserv);
    query.bindValue(":etatsserv", etatsserv);
    query.bindValue(":coutserv", coutserv);

    return query.exec();
}


QSqlQueryModel* Service::rechercher(int idrech) {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM services WHERE IDSERV = :idserv");
    query.bindValue(":idserv", idrech);

    if (query.exec()) {
        model->setQuery(query);
    } else {
        qDebug() << "Erreur lors de la recherche :" << query.lastError().text();
    }
    return model;
}

QSqlQueryModel* Service::trier(const QString &critere, const QString &ordre) {
    QSqlQueryModel *model = new QSqlQueryModel();
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
    html += "<h1 style='text-align: center;'>Liste des Services</h1>";
    html += "<table>";
    html += "<tr><th>ID</th><th>Type</th><th>Durée</th><th>État</th><th>Coût</th></tr>";

    QSqlQuery query;
    if (!query.exec("SELECT IDSERV, TYPE, DUREE, ETATS, COUT FROM services")) {
        qDebug() << "Erreur lors de l'extraction des données :" << query.lastError().text();
        return;
    }

    while (query.next()) {
        html += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td></tr>")
                    .arg(query.value("IDSERV").toString())
                    .arg(query.value("TYPE").toString())
                    .arg(query.value("DUREE").toString())
                    .arg(query.value("ETATS").toString())
                    .arg(query.value("COUT").toString());
    }

    html += "</table></body></html>";
    document.setHtml(html);
    document.print(&printer);

    qDebug() << "PDF généré avec succès :" << pdfFileName;

    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(pdfFileName))) {
        qDebug() << "Impossible d'ouvrir le PDF.";
    }
}

QMap<QString, int> Service::statistiquesParEtats() {
    QMap<QString, int> stats;
    QSqlQuery query;
    if (query.exec("SELECT ETATSERV, COUNT(*) AS count FROM services GROUP BY ETATSERV")) {
        while (query.next()) {
            stats.insert(query.value("ETATSERV").toString(), query.value("count").toInt());
        }
    } else {
        qDebug() << "Erreur lors de l'extraction des statistiques :" << query.lastError().text();
    }
    return stats;
}
