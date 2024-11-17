#include "employe.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>
#include <QDebug>
#include <QSqlQueryModel>
#include <QMap>
#include <QVariant>
#include <QPrinter>
#include <QTextDocument>

employe::employe(int ID, QString NOM, QString PRENOM, QString POSTE, int SALAIRE, int DUREE, QString MDP)
    : ID(ID), NOM(NOM), PRENOM(PRENOM), POSTE(POSTE), SALAIRE(SALAIRE), DUREE(DUREE), MDP(MDP) {}

bool employe::validateInputs() const {
    QRegularExpression numericRegex("^[0-9]+$");  // Only digits
    QRegularExpression alphaRegex("^[a-zA-Z]+$");  // Only letters

    if (!numericRegex.match(QString::number(ID)).hasMatch() || ID <= 0) {
        qDebug() << "Validation Error: ID doit etre un numero positif.";
        return false;
    }
    if (!alphaRegex.match(NOM).hasMatch() || NOM.length() > 20) {
        qDebug() << "Validation Error: Nom doit etre alphabetique et moins de 20 caractères.";
        return false;
    }
    if (!alphaRegex.match(PRENOM).hasMatch() || PRENOM.length() > 20) {
        qDebug() << "Validation Error: Prenom doit etre alphabetique et moins de 20 caractères.";
        return false;
    }
    if (!alphaRegex.match(POSTE).hasMatch() || POSTE.length() > 30) {
        qDebug() << "Validation Error: Poste doit etre alphabetique et moins de 30 caractères.";
        return false;
    }
    if (!numericRegex.match(QString::number(SALAIRE)).hasMatch() || SALAIRE < 0) {
        qDebug() << "Validation Error: Salaire doit etre un numero positif.";
        return false;
    }
    if (!numericRegex.match(QString::number(DUREE)).hasMatch() || DUREE < 0) {
        qDebug() << "Validation Error: Date_Embauche doit etre un numero positif.";
        return false;
    }
    return true;
}

QSqlQueryModel* employe::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();

    // Modify the query to include MDP
    model->setQuery("SELECT ID, NOM, PRENOM, POSTE, SALAIRE, DUREE, MDP FROM employee");

    // Set column headers
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("POSTE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("SALAIRE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("DUREE"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("MDP"));

    return model;
}

bool employe::ajouter() {
    if (!validateInputs()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO employee (id, NOM, PRENOM, POSTE, SALAIRE, DUREE, MDP) "
                  "VALUES (:id, :NOM, :PRENOM, :POSTE, :SALAIRE, :DUREE, :MDP)");

    query.bindValue(":id", ID);
    query.bindValue(":NOM", NOM);
    query.bindValue(":PRENOM", PRENOM);
    query.bindValue(":POSTE", POSTE);
    query.bindValue(":SALAIRE", SALAIRE);
    query.bindValue(":DUREE", DUREE);
    query.bindValue(":MDP", MDP);  // Bind the MDP value

    if (!query.exec()) {
        qDebug() << "Add Employee Error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool employe::update(int id, QString NOM, QString PRENOM, QString POSTE, int SALAIRE, int DUREE, QString MDP) {
    if (id <= 0 || NOM.isEmpty() || PRENOM.isEmpty() || POSTE.isEmpty() || SALAIRE < 0 || DUREE < 0) {
        qDebug() << "Update Error: Invalid input data.";
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE employee SET NOM = :NOM, PRENOM = :PRENOM, POSTE = :POSTE, "
                  "SALAIRE = :SALAIRE, DUREE = :DUREE, MDP = :MDP WHERE id = :id");

    query.bindValue(":id", id);
    query.bindValue(":NOM", NOM);
    query.bindValue(":PRENOM", PRENOM);
    query.bindValue(":POSTE", POSTE);
    query.bindValue(":SALAIRE", SALAIRE);
    query.bindValue(":DUREE", DUREE);
    query.bindValue(":MDP", MDP);  // Bind the MDP value

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

QSqlQueryModel* employe::sortBySalaire() {
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("SELECT ID, NOM, PRENOM, POSTE, SALAIRE, DUREE, MDP FROM employee ORDER BY SALAIRE ASC");

    // Set column headers
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("POSTE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("SALAIRE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("DUREE"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("MDP"));

    return model;
}

bool employe::exportToPDF(const QString &filePath) {
    QTextDocument document;
    QString htmlContent;

    // Prepare an HTML table to structure the data
    htmlContent.append("<h1>Employee Report</h1>");
    htmlContent.append("<table border='1' cellspacing='0' cellpadding='2'>");
    htmlContent.append("<tr><th>ID</th><th>Nom</th><th>Prenom</th><th>Post</th><th>Salaire</th><th>Date Embauche</th><th>MDP</th></tr>");

    QSqlQuery query("SELECT * FROM employee");
    while (query.next()) {
        htmlContent.append("<tr>");
        htmlContent.append("<td>" + query.value("id").toString() + "</td>");
        htmlContent.append("<td>" + query.value("NOM").toString() + "</td>");
        htmlContent.append("<td>" + query.value("PRENOM").toString() + "</td>");
        htmlContent.append("<td>" + query.value("POSTE").toString() + "</td>");
        htmlContent.append("<td>" + query.value("SALAIRE").toString() + "</td>");
        htmlContent.append("<td>" + query.value("DUREE").toString() + "</td>");
        htmlContent.append("<td>" + query.value("MDP").toString() + "</td>");
        htmlContent.append("</tr>");
    }
    htmlContent.append("</table>");

    document.setHtml(htmlContent);

    // Set up the printer for PDF output
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);

    // Print the document to the specified PDF file
    document.print(&printer);

    return true;  // Return true to indicate success
}

QMap<QString, QVariant> employe::getDureeStatistics() {
    QMap<QString, QVariant> stats;
    QSqlQuery query;

    // Query for the minimum duration
    query.exec("SELECT MIN(DUREE) FROM employee");
    if (query.next()) {
        stats["Min"] = query.value(0);
    } else {
        qDebug() << "Error fetching minimum duration";
    }

    // Query for the maximum duration
    query.exec("SELECT MAX(DUREE) FROM employee");
    if (query.next()) {
        stats["Max"] = query.value(0);
    } else {
        qDebug() << "Error fetching maximum duration";
    }

    // Query for the average duration
    query.exec("SELECT AVG(DUREE) FROM employee");
    if (query.next()) {
        stats["Average"] = query.value(0);
    } else {
        qDebug() << "Error fetching average duration";
    }

    return stats;
}

QSqlQueryModel* employe::search(const QString &searchQuery) {
    QSqlQueryModel *model = new QSqlQueryModel();

    // SQL query to search across NOM, PRENOM, and POSTE fields
    QString queryStr = "SELECT * FROM employee WHERE "
                       "NOM LIKE :query OR "
                       "PRENOM LIKE :query OR "
                       "POSTE LIKE :query";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":query", "%" + searchQuery + "%"); // Use wildcards for partial matching

    if (query.exec()) {
        model->setQuery(query);

        // Optionally set column headers for your table
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Poste"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Salaire"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Duree"));
    } else {
        qDebug() << "Error in query execution: " << query.lastError();
    }

    return model;
}
QString employe::getPhoneNumberForEmployee(int employeeId) {
    QSqlQuery query;
    query.prepare("SELECT PHONE FROM employee WHERE id = :id");
    query.bindValue(":id", employeeId);

    if (query.exec()) {
        if (query.next()) {
            return query.value(0).toString();  // Return the phone number
        } else {
            qDebug() << "Employee not found with ID:" << employeeId;
        }
    } else {
        qDebug() << "Query failed:" << query.lastError();
    }

    return "";  // Return an empty string if the employee is not found or an error occurred
}
