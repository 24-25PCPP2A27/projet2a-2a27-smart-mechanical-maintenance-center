#include "chatbot.h"
#include "service.h"
#include <QDebug>
#include <QTextStream>
#include <QPdfWriter>
#include <QPainter>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QStringList>
#include <QFileDialog>
#include <QTextDocument>
ChatBot::ChatBot(QObject *parent)
    : QObject(parent)
{
}

QString ChatBot::getResponse(const QString &input)
{
    QString response;

    if (input.toLower().contains("add service")) {
        response = handleAddService(input);
    } else if (input.toLower().contains("delete service")) {
        response = handleDeleteService(input);
    } else if (input.toLower().contains("export pdf")) {
        response = handleExportToPDF();
    } else if (input.toLower().contains("list services")) {
        response = handleListServices();
    } else {
        response = "Command not recognized. Try:\n"
                   "- Add Service, ID, Type, Cost, Duration, Status\n"
                   "- Delete Service, ID\n"
                   "- List Services\n"
                   "- Export PDF";
    }

    return response;
}

QString ChatBot::handleAddService(const QString &input)
{
    // Check if the input starts with "Add Service" followed by a comma
    if (!input.toLower().startsWith("add service,")) {
        return "Invalid format. Use: Add Service, ID, Type, Cost, Duration, Status.";
    }

    // Remove the "Add Service," part from the input string (including the comma)
    QString cleanedInput = input.mid(12).trimmed(); // Remove "Add Service, " part (length 12)

    // Now split the rest of the input by commas and skip empty parts
    QStringList details = cleanedInput.split(",", QString::SkipEmptyParts);
    for (auto &detail : details) {
        detail = detail.trimmed(); // Trim extra spaces around each parameter
    }

    if (details.size() != 5) { // Expect exactly 5 parameters (ID, Type, Cost, Duration, Status)
        return QString("Incorrect number of parameters. Provided: %1. Expected: 5.")
                .arg(details.size());
    }

    bool idOk, costOk, durationOk;
    int id = details[0].toInt(&idOk);
    QString type = details[1];
    double cost = details[2].toDouble(&costOk);
    double duration = details[3].toDouble(&durationOk);
    QString status = details[4].toLower();

    if (!idOk || !costOk || !durationOk) {
        return "Failed to parse ID, Cost, or Duration. Please ensure they are valid numbers.";
    }

    // Validate status
    QStringList validStatuses = {"en cours", "terminée", "annulé"};
    if (!validStatuses.contains(status)) {
        return "Invalid status. Please choose from: en cours, terminée, annulé.";
    }

    // Create service object
    Service service(id, cost, type, duration, status);

    // Try to add the service
    if (service.ajouter(id, type, QString::number(duration), status, cost)) {
        return "Service added successfully.";
    } else {
        return "Failed to add service. Please verify the details.";
    }
}





QString ChatBot::handleDeleteService(const QString &input)
{
    if (!input.contains(",")) {
        return "Invalid format. Use: Delete Service, ID.";
    }

    QStringList details = input.split(",");
    if (details.size() != 2) {
        return "Incorrect format. Provide only: Delete Service, ID.";
    }

    bool idOk;
    int id = details[1].trimmed().toInt(&idOk);

    if (!idOk) {
        return "Invalid ID. Please ensure the ID is a number.";
    }

    Service service;
    if (service.supprimer(id)) {
        return "Service deleted successfully.";
    } else {
        return "Failed to delete service. Please ensure the ID exists.";
    }
}

QString ChatBot::handleExportToPDF()
{
    // Open a file dialog to choose location and filename
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save PDF", "", "PDF Files (*.pdf)");

    // If the user cancels the dialog, return without saving
    if (fileName.isEmpty()) {
        return "Export canceled.";
    }

    // Ensure the file has a .pdf extension
    if (!fileName.endsWith(".pdf")) {
        fileName += ".pdf";
    }

    // Create HTML table structure
    QString htmlContent = "<html><head><style>"
                          "table {border-collapse: collapse; width: 100%;}"
                          "th, td {border: 1px solid black; padding: 8px; text-align: left;}"
                          "</style></head><body>"
                          "<h1>Service List</h1>"
                          "<table>"
                          "<tr><th>ID</th><th>Type</th><th>Cost</th><th>Duration</th><th>Status</th><th>Equipment State</th></tr>";

    // Fetch the data
    Service service;
    QSqlQueryModel *model = service.afficher();

    if (!model || model->rowCount() == 0) {
        return "No services found to export.";
    }

    // Loop through the rows and add them to the HTML table
    for (int i = 0; i < model->rowCount(); ++i) {
        QSqlRecord record = model->record(i);
        htmlContent += "<tr>"
                       "<td>" + record.value("idserv").toString() + "</td>"
                       "<td>" + record.value("typeserv").toString() + "</td>"
                       "<td>" + record.value("coutserv").toString() + "</td>"
                       "<td>" + record.value("dureeserv").toString() + "</td>"
                       "<td>" + record.value("etatsserv").toString() + "</td>"
                       "<td>" + record.value("etat").toString() + "</td>"
                       "</tr>";
    }

    // Close the table and HTML tags
    htmlContent += "</table></body></html>";

    // Create a QTextDocument to convert HTML to PDF
    QTextDocument document;
    document.setHtml(htmlContent);

    // Create PDF writer
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPagedPaintDevice::A4);

    // Now, pass QPdfWriter directly to QTextDocument::print()
    document.print(&pdfWriter);

    return "Services exported successfully to " + fileName;
}

QString ChatBot::handleListServices()
{
    Service service;
    QSqlQueryModel *model = service.afficher();

    if (!model || model->rowCount() == 0) {
        return "No services found.";
    }

    QString result;
    QTextStream stream(&result);

    stream << "ID\tType\tCost\tDuration\tStatus\tEquipment State\n";

    for (int i = 0; i < model->rowCount(); ++i) {
        QSqlRecord record = model->record(i);
        stream << record.value("idserv").toString() << "\t"
               << record.value("typeserv").toString() << "\t"
               << record.value("coutserv").toString() << "\t"
               << record.value("dureeserv").toString() << "\t"
               << record.value("etatsserv").toString() << "\t"
               << record.value("etat").toString() << "\n";
    }

    return result;
}
