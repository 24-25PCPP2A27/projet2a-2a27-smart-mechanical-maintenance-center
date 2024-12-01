/*
#include "chatbot.h"
#include "service.h"
#include <QDebug>
#include <QTextStream>
#include <QPdfWriter>
#include <QPainter>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlRecord>
ChatBot::ChatBot(QObject *parent)
    : QObject(parent)
{
}

QString ChatBot::getResponse(const QString &input)
{
    QString response;

    if (input.toLower().contains("add service")) {
        response = "To add a service, provide the following details: ID, Type, Cost, Duration, and Status.";
    } else if (input.toLower().contains("update service")) {
        response = "To update a service, provide the ID of the service you want to modify and the new details.";
    } else if (input.toLower().contains("delete service")) {
        response = "To delete a service, provide the ID of the service you want to remove.";
    } else if (input.toLower().contains("export pdf")) {
        response = "Exporting services to PDF. Please wait...";
        if (exportToPDF()) {
            response += "\nServices exported successfully to services.pdf.";
        } else {
            response += "\nFailed to export services to PDF.";
        }
    } else if (input.toLower().contains("list services")) {
        response = listServices();
    } else {
        response = "I didn't understand that. You can ask me about adding, updating, deleting services, or exporting them to PDF.";
    }

    return response;
}

bool ChatBot::exportToPDF()
{
    QPdfWriter pdfWriter("services.pdf");
    QPainter painter(&pdfWriter);

    if (!painter.isActive()) {
        qDebug() << "Failed to open PDF writer.";
        return false;
    }

    Service service;
    QSqlQueryModel *model = service.afficher();

    if (!model || model->rowCount() == 0) {
        qDebug() << "No services found to export.";
        return false;
    }

    int y = 100;
    painter.setFont(QFont("Arial", 12));
    painter.drawText(50, y, "ID");
    painter.drawText(150, y, "Type");
    painter.drawText(250, y, "Cost");
    painter.drawText(350, y, "Duration");
    painter.drawText(450, y, "Status");

    y += 50;

    for (int i = 0; i < model->rowCount(); ++i) {
        QSqlRecord record = model->record(i);
        painter.drawText(50, y, record.value("idserv").toString());
        painter.drawText(150, y, record.value("type").toString());
        painter.drawText(250, y, record.value("cout").toString());
        painter.drawText(350, y, record.value("duree").toString());
        painter.drawText(450, y, record.value("etats").toString());
        y += 50;
    }

    painter.end();
    return true;
}

QString ChatBot::listServices()
{
    Service service;
    QSqlQueryModel *model = service.afficher();

    if (!model || model->rowCount() == 0) {
        return "No services found.";
    }

    QString result;
    QTextStream stream(&result);

    stream << "ID\tType\tCost\tDuration\tStatus\n";

    for (int i = 0; i < model->rowCount(); ++i) {
        QSqlRecord record = model->record(i);
        stream << record.value("idserv").toString() << "\t"
               << record.value("type").toString() << "\t"
               << record.value("cout").toString() << "\t"
               << record.value("duree").toString() << "\t"
               << record.value("etats").toString() << "\n";
    }

    return result;
}
*/
/*#include "chatbot.h"
#include "service.h"
#include <QDebug>
#include <QTextStream>
#include <QPdfWriter>
#include <QPainter>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlRecord>

ChatBot::ChatBot(QObject *parent)
    : QObject(parent)
{
}

QString ChatBot::getResponse(const QString &input)
{
    QString response;

    if (input.toLower().contains("add service")) {
        response = "To add a service, provide the following details: ID, Type, Cost, Duration, and Status.";
        if (input.contains(",")) {
            QStringList details = input.split(",");
            if (details.size() == 5) {
                bool idOk;
                int id = details[0].split(" ").last().trimmed().toInt(&idOk);

                QString type = details[1].trimmed();
                bool costOk;
                double cost = details[2].trimmed().toDouble(&costOk);
                bool durationOk;
                double duration = details[3].trimmed().toDouble(&durationOk);
                QString status = details[4].trimmed();

                if (idOk && costOk && durationOk) {
                    // Create a Service instance with the provided details
                    Service service(id, cost, type, duration, status, "en panne"); // Default state for equipment

                    if (Service.ajouter(id, type, QString::number(duration), status, cost, "en panne")) {
                        response = "Service added successfully.";
                    } else {
                        response = "Failed to add service. Please check the details.";
                    }
                } else {
                    response = "Failed to convert some details to the correct types.";
                }
            } else {
                response += "\nIncorrect number of details provided. Format: ID, Type, Cost, Duration, Status.";
            }
        }
    } else if (input.toLower().contains("update service")) {
        response = "To update a service, provide the ID of the service you want to modify and the new details.";
    } else if (input.toLower().contains("delete service")) {
        response = "To delete a service, provide the ID of the service you want to remove.";
    } else if (input.toLower().contains("export pdf")) {
        response = "Exporting services to PDF. Please wait...";
        if (exportToPDF()) {
            response += "\nServices exported successfully to services.pdf.";
        } else {
            response += "\nFailed to export services to PDF.";
        }
    } else if (input.toLower().contains("list services")) {
        response = listServices();
    } else {
        response = "I didn't understand that. You can ask me about adding, updating, deleting services, or exporting them to PDF.";
    }

    return response;
}

bool ChatBot::exportToPDF()
{
    QPdfWriter pdfWriter("services.pdf");
    QPainter painter(&pdfWriter);

    if (!painter.isActive()) {
        qDebug() << "Failed to open PDF writer.";
        return false;
    }

    Service service;
    QSqlQueryModel *model = service.afficher();

    if (!model || model->rowCount() == 0) {
        qDebug() << "No services found to export.";
        return false;
    }

    int y = 100;
    painter.setFont(QFont("Arial", 12));
    painter.drawText(50, y, "ID");
    painter.drawText(150, y, "Type");
    painter.drawText(250, y, "Cost");
    painter.drawText(350, y, "Duration");
    painter.drawText(450, y, "Status");

    y += 50;

    for (int i = 0; i < model->rowCount(); ++i) {
        QSqlRecord record = model->record(i);
        painter.drawText(50, y, record.value("idserv").toString());
        painter.drawText(150, y, record.value("type").toString());
        painter.drawText(250, y, record.value("cout").toString());
        painter.drawText(350, y, record.value("duree").toString());
        painter.drawText(450, y, record.value("etats").toString());
        y += 50;
    }

    painter.end();
    return true;
}

QString ChatBot::listServices()
{
    Service service;
    QSqlQueryModel *model = service.afficher();

    if (!model || model->rowCount() == 0) {
        return "No services found.";
    }

    QString result;
    QTextStream stream(&result);

    stream << "ID\tType\tCost\tDuration\tStatus\n";

    for (int i = 0; i < model->rowCount(); ++i) {
        QSqlRecord record = model->record(i);
        stream << record.value("idserv").toString() << "\t"
               << record.value("type").toString() << "\t"
               << record.value("cout").toString() << "\t"
               << record.value("duree").toString() << "\t"
               << record.value("etats").toString() << "\n";
    }

    return result;
}
*/
#include "chatbot.h"
#include "service.h"
#include <QDebug>
#include <QTextStream>
#include <QPdfWriter>
#include <QPainter>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlRecord>

ChatBot::ChatBot(QObject *parent)
    : QObject(parent)
{
}

QString ChatBot::getResponse(const QString &input)
{
    QString response;

    if (input.toLower().contains("add service")) {
        response = "To add a service, provide the following details: ID, Type, Cost, Duration, and Status.";
        if (input.contains(",")) {
            QStringList details = input.split(",");
            if (details.size() == 5) {
                bool idOk, costOk, durationOk;
                int id = details[0].split(" ").last().trimmed().toInt(&idOk);
                QString type = details[1].trimmed();
                double cost = details[2].trimmed().toDouble(&costOk);
                double duration = details[3].trimmed().toDouble(&durationOk);
                QString status = details[4].trimmed();

                if (idOk && costOk && durationOk) {
                    Service service(id, cost, type, duration, status, "en panne"); // Default state for equipment
                    if (service.ajouter(id, type, QString::number(duration), status, cost)) {
                        response = "Service added successfully.";
                    } else {
                        response = "Failed to add service. Please check the details.";
                    }
                } else {
                    response = "Failed to convert some details to the correct types.";
                }
            } else {
                response += "\nIncorrect number of details provided. Format: ID, Type, Cost, Duration, Status.";
            }
        }
    } else if (input.toLower().contains("update service")) {
        response = "To update a service, provide the ID of the service you want to modify and the new details.";
    } else if (input.toLower().contains("delete service")) {
        response = "To delete a service, provide the ID of the service you want to remove.";
    } else if (input.toLower().contains("export pdf")) {
        response = "Exporting services to PDF. Please wait...";
        if (exportToPDF()) {
            response += "\nServices exported successfully to services.pdf.";
        } else {
            response += "\nFailed to export services to PDF.";
        }
    } else if (input.toLower().contains("list services")) {
        response = listServices();
    } else {
        response = "I didn't understand that. You can ask me about adding, updating, deleting services, or exporting them to PDF.";
    }

    return response;
}

bool ChatBot::exportToPDF()
{
    QPdfWriter pdfWriter("services.pdf");
    QPainter painter(&pdfWriter);

    if (!painter.isActive()) {
        qDebug() << "Failed to open PDF writer.";
        return false;
    }

    Service service;
    QSqlQueryModel *model = service.afficher();

    if (!model || model->rowCount() == 0) {
        qDebug() << "No services found to export.";
        return false;
    }

    int y = 100;
    painter.setFont(QFont("Arial", 12));
    painter.drawText(50, y, "ID");
    painter.drawText(150, y, "Type");
    painter.drawText(250, y, "Cost");
    painter.drawText(350, y, "Duration");
    painter.drawText(450, y, "Status");

    y += 50;

    for (int i = 0; i < model->rowCount(); ++i) {
        QSqlRecord record = model->record(i);
        painter.drawText(50, y, record.value("idserv").toString());
        painter.drawText(150, y, record.value("type").toString());
        painter.drawText(250, y, record.value("cout").toString());
        painter.drawText(350, y, record.value("duree").toString());
        painter.drawText(450, y, record.value("etats").toString());
        y += 50;
    }

    painter.end();
    return true;
}

QString ChatBot::listServices()
{
    Service service;
    QSqlQueryModel *model = service.afficher();

    if (!model || model->rowCount() == 0) {
        return "No services found.";
    }

    QString result;
    QTextStream stream(&result);

    stream << "ID\tType\tCost\tDuration\tStatus\n";

    for (int i = 0; i < model->rowCount(); ++i) {
        QSqlRecord record = model->record(i);
        stream << record.value("idserv").toString() << "\t"
               << record.value("type").toString() << "\t"
               << record.value("cout").toString() << "\t"
               << record.value("duree").toString() << "\t"
               << record.value("etats").toString() << "\n";
    }

    return result;
}
