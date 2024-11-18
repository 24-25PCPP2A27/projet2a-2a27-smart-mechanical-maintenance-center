#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QSqlError> // Optional for error handling
#include <QMessageBox>
#include <QPdfWriter>
#include <QDebug>
#include <QPainter>
#include <QFileDialog>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QtCore/QCoreApplication>
#include <QSslSocket>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QByteArray>
#include <QUrl>


// Constructor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);  // Initialize UI components

    // Connect signals to their respective slots
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addAppointment);
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::updateAppointment);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::deleteAppointment);
    connect(ui->afficherButton, &QPushButton::clicked, this, &MainWindow::loadAppointments);
    connect(ui->pdfButton, &QPushButton::clicked,this, &MainWindow::exportToPDF);
    connect(ui->statsButton, &QPushButton::clicked, this, &MainWindow::generateCountryStatistics);
    connect(ui->sortButton, &QPushButton::clicked, this, &MainWindow::sortAppointmentsByID);
    connect(ui->lineEdit_search, &QLineEdit::textChanged, this, &MainWindow::searchAppointments);

}

void MainWindow::addAppointment() {
    int jour = ui->jourEdit->text().toInt();
    int mois = ui->moisEdit->text().toInt();
    int annee = ui->anneeEdit->text().toInt();
    int heure = ui->heureEdit->text().toInt();
    QString position = ui->adresseComboBox->currentText();
    int id = ui->idEdit->text().toInt();
    QString email = ui->emailEdit->text();  // Capture email input from the form

    // Validate email
    if (email.isEmpty() || !email.contains("@")) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une adresse e-mail valide.");
        return;
    }

    // Validate address selection
    if (position == "Selectionner...") {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une adresse valide.");
        return;
    }

    // Validate date and time inputs
    if (jour < 1 || jour > 31 || mois < 1 || mois > 12 || heure < 0 || heure > 23) {
        QMessageBox::warning(this, "Erreur", "Vérifiez les champs Jour, Mois ou Heure.");
        return;
    }

    // Send confirmation email to the client (without storing the email in the DB)
    QString subject = "Confirmation de votre rendez-vous";
    QString body = QString("Bonjour,\n\nVotre rendez-vous est confirmé pour le %1/%2/%3 à %4 heures. "
                           "Adresse: %5.\n\nCordialement,\nVotre service.")
                           .arg(jour).arg(mois).arg(annee).arg(heure).arg(position);
    sendEmail(email, subject, body);  // Send email to client

    // Insert the appointment into the database
    QSqlQuery query;
    query.prepare("INSERT INTO rdv (IDR, Jour, Mois, Annee, Heure, Adresse) "
                  "VALUES (:id, :jour, :mois, :annee, :heure, :adresse)");
    query.bindValue(":id", id);
    query.bindValue(":jour", jour);
    query.bindValue(":mois", mois);
    query.bindValue(":annee", annee);
    query.bindValue(":heure", heure);
    query.bindValue(":adresse", position);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Rendez-vous ajouté avec succès.");
        loadAppointments();  // Reload appointments if needed
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de l'ajout : " + query.lastError().text());
    }
}


// Update Appointment
void MainWindow::updateAppointment() {
    int id = ui->idEdit->text().toInt();
    int jour = ui->jourEdit->text().toInt();
    int mois = ui->moisEdit->text().toInt();
    int annee = ui->anneeEdit->text().toInt();
    int heure = ui->heureEdit->text().toInt();
    QString position = ui->adresseComboBox->currentText();

    // Check if the address is valid
    if (position == "Selectionner...") {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une adresse valide.");
        return;
    }

    // Prepare the SQL query for updating the appointment
    QSqlQuery query;
    query.prepare("UPDATE rdv SET Jour = :jour, Mois = :mois, Annee = :annee, "
                  "Heure = :heure, Adresse = :adresse WHERE IDR = :id");
    query.bindValue(":id", id);
    query.bindValue(":jour", jour);
    query.bindValue(":mois", mois);
    query.bindValue(":annee", annee);
    query.bindValue(":heure", heure);
    query.bindValue(":adresse", position);

    // Execute the query and check for success
    if (query.exec()) {
        // Show success message and reload appointments to refresh the view
        QMessageBox::information(this, "Succès", "Rendez-vous mis à jour.");
        loadAppointments();  // Refresh the appointments table
    } else {
        // Show error message in case of failure
        QMessageBox::warning(this, "Erreur", "Échec de la mise à jour : " + query.lastError().text());
    }
}


// Delete Appointment
void MainWindow::deleteAppointment() {
    int id = ui->idEdit->text().toInt();

    // Check if the ID is valid
    if (id == 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID valide.");
        return;
    }

    // Prepare the SQL query for deleting the appointment
    QSqlQuery query;
    query.prepare("DELETE FROM rdv WHERE IDR = :id");
    query.bindValue(":id", id);

    // Execute the query and check for success
    if (query.exec()) {
        // Show success message and reload appointments to refresh the view
        QMessageBox::information(this, "Succès", "Rendez-vous supprimé.");
        loadAppointments();  // Refresh the appointments table
    } else {
        // Show error message in case of failure
        QMessageBox::warning(this, "Erreur", "Échec de la suppression : " + query.lastError().text());
    }
}



void MainWindow::loadAppointments() {
    // Initialize the model if it's not already initialized
    if (!appointmentsModel) {
        appointmentsModel = new QSqlTableModel(this);
        appointmentsModel->setTable("rdv"); // Set the table name
        appointmentsModel->setEditStrategy(QSqlTableModel::OnManualSubmit); // Allow manual submission of changes
    }

    // Reload the data from the database (this will refresh the view)
    appointmentsModel->select();  // Fetch the latest data from the database

    // Set the model to the QTableView
    ui->appointmentsView->setModel(appointmentsModel);

    // Customize headers if needed
    appointmentsModel->setHeaderData(0, Qt::Horizontal, "ID");
    appointmentsModel->setHeaderData(1, Qt::Horizontal, "Jour");
    appointmentsModel->setHeaderData(2, Qt::Horizontal, "Mois");
    appointmentsModel->setHeaderData(3, Qt::Horizontal, "Année");
    appointmentsModel->setHeaderData(4, Qt::Horizontal, "Heure");
    appointmentsModel->setHeaderData(5, Qt::Horizontal, "Adresse");

    // Adjust the view to make the columns stretch to fit the available space
    ui->appointmentsView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}




void MainWindow::exportToPDF() {
    // Get the file path from the user
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter au format PDF", "", "*.pdf");

    // If the user cancels the dialog, return
    if (filePath.isEmpty()) {
        return;
    }

    // Ensure the file has a .pdf extension
    if (!filePath.endsWith(".pdf")) {
        filePath += ".pdf";
    }

    // Initialize the PDF writer with the chosen file path
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));  // Set the page size to A4
    QPainter painter(&pdfWriter);  // Create a QPainter to draw on the PDF

    int y = 50;  // Starting y position for text

    // Set title font
    painter.setFont(QFont("Arial", 16));
    painter.drawText(200, y, "Liste des Rendez-vous");
    y += 50;  // Adjust for spacing after title

    // Set header font
    painter.setFont(QFont("Arial", 10));
    painter.drawText(50, y, "ID");
    painter.drawText(100, y, "Jour");
    painter.drawText(150, y, "Mois");
    painter.drawText(200, y, "Année");
    painter.drawText(250, y, "Heure");
    painter.drawText(300, y, "Adresse");
    y += 20;  // Adjust for spacing after headers

    // Query the database to get all the appointments
    QSqlQuery query("SELECT * FROM rdv");
    while (query.next()) {
        // Write each record in the PDF
        painter.drawText(50, y, query.value(0).toString());  // ID
        painter.drawText(100, y, query.value(1).toString()); // Jour
        painter.drawText(150, y, query.value(2).toString()); // Mois
        painter.drawText(200, y, query.value(3).toString()); // Année
        painter.drawText(250, y, query.value(4).toString()); // Heure
        painter.drawText(300, y, query.value(5).toString()); // Adresse
        y += 20;  // Adjust for spacing between records

        // If the page is too full, create a new page
        if (y > 800) {
            pdfWriter.newPage();
            y = 50;  // Reset y to the top for the new page
        }
    }

    painter.end();  // End the painter session

    // Show a message to indicate the export was successful
    QMessageBox::information(this, "Succès", "Exportation réussie.");
}
// Function to calculate statistics for the most popular country
void MainWindow::generateCountryStatistics() {
    // Create a QMap to store the country count
    QMap<QString, int> countryCount;

    // Query the database to get all the addresses (or countries)
    QSqlQuery query("SELECT Adresse FROM rdv");
    while (query.next()) {
        QString country = query.value(0).toString();

        // If the country is not empty, increment its count
        if (!country.isEmpty()) {
            countryCount[country]++;
        }
    }

    // Find the country with the highest count
    QString mostPopularCountry;
    int highestCount = 0;

    QMapIterator<QString, int> i(countryCount);
    while (i.hasNext()) {
        i.next();
        if (i.value() > highestCount) {
            mostPopularCountry = i.key();
            highestCount = i.value();
        }
    }

    // Show the result in a message box
    if (!mostPopularCountry.isEmpty()) {
        QString message = QString("Le pays le plus populaire est : %1 avec %2 rendez-vous.")
                          .arg(mostPopularCountry)
                          .arg(highestCount);
        QMessageBox::information(this, "Statistiques", message);
    } else {
        QMessageBox::information(this, "Statistiques", "Aucune donnée sur les pays.");
    }
}
void MainWindow::sortAppointmentsByID() {
    if (!appointmentsModel) {
        // Initialize the model if it's not already initialized
        appointmentsModel = new QSqlTableModel(this);
        appointmentsModel->setTable("rdv"); // Set the table name
        appointmentsModel->setEditStrategy(QSqlTableModel::OnManualSubmit); // Allow manual submission of changes
    }

    // Set the sorting order for the table
    appointmentsModel->setSort(0, Qt::AscendingOrder); // Sort by column 0 (ID) in ascending order
    appointmentsModel->select(); // Refresh the model to apply the sorting

    // Optional: Show a message indicating the sorting is done
    QMessageBox::information(this, "Tri", "Les rendez-vous ont été triés par ID.");
}
void MainWindow::searchAppointments(const QString &searchQuery) {
    // Create a new QSqlQueryModel to hold the query results
    QSqlQueryModel* model = new QSqlQueryModel();

    // SQL query to search across IDR and Adresse fields
    QString queryStr = "SELECT * FROM rdv WHERE "
                       "IDR LIKE :query OR "
                       "Adresse LIKE :query";

    // Prepare and execute the query
    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":query", "%" + searchQuery + "%"); // Use wildcards for partial matching

    if (query.exec()) {
        // Populate the model with the query results
        model->setQuery(query);

        // Set column headers for the table view
        model->setHeaderData(0, Qt::Horizontal, "ID");
        model->setHeaderData(1, Qt::Horizontal, "Jour");
        model->setHeaderData(2, Qt::Horizontal, "Mois");
        model->setHeaderData(3, Qt::Horizontal, "Année");
        model->setHeaderData(4, Qt::Horizontal, "Heure");
        model->setHeaderData(5, Qt::Horizontal, "Adresse");

        // Set the model to the table view
        ui->appointmentsView->setModel(model);
    } else {
        // Log query errors for debugging
        qDebug() << "Error in search query execution: " << query.lastError();
    }
}
void MainWindow::sendEmail(const QString &recipient, const QString &subject, const QString &body) {
    // Set up the email content
    QByteArray data;
    data.append("To: " + recipient + "\r\n");
    data.append("Subject: " + subject + "\r\n");
    data.append("Content-Type: text/plain; charset=utf-8\r\n");
    data.append("\r\n");  // End of headers
    data.append(body);  // Email body

    // Set up the SMTP server connection (you can replace this with your own SMTP server)
    QSslSocket smtpSocket;
    smtpSocket.connectToHostEncrypted("smtp.yourmailserver.com", 465);  // SMTP server (replace with actual SMTP server)

    if (!smtpSocket.waitForConnected(5000)) {
        QMessageBox::warning(this, "Erreur", "Connection au serveur de messagerie échouée.");
        return;
    }

    // Send email commands via SMTP protocol
    smtpSocket.write("HELO localhost\r\n");
    smtpSocket.waitForBytesWritten();

    smtpSocket.write("MAIL FROM:<your-email@example.com>\r\n");  // Your email address here
    smtpSocket.waitForBytesWritten();

    smtpSocket.write("RCPT TO:<" + recipient.toUtf8() + ">\r\n");
    smtpSocket.waitForBytesWritten();

    smtpSocket.write("DATA\r\n");
    smtpSocket.waitForBytesWritten();

    smtpSocket.write(data);  // Write the email content
    smtpSocket.write("\r\n.\r\n");  // End of email body
    smtpSocket.waitForBytesWritten();

    smtpSocket.write("QUIT\r\n");
    smtpSocket.waitForBytesWritten();

    // Close connection
    smtpSocket.disconnectFromHost();
}

