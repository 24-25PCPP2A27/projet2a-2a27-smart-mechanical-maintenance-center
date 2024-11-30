#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QSqlError> // Optional for error handling
#include <QMessageBox>
#include <QPdfWriter>
#include <QDebug>
#include <QTableWidget>
#include <QPainter>
#include <QFileDialog>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMessageBox>
#include <QtCore/QCoreApplication>
#include <QProgressBar>
#include <QHttpMultiPart>
#include <QPrinter>
#include <QCalendarWidget>
#include <QMap>
#include <QDate>
#include <QStringList>
#include <QByteArray>
#include <QTimer>
#include<QVBoxLayout>
#include<QtWidgets>
#include<QPaintEvent>
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
    connect(ui->calendarWidget, &QCalendarWidget::clicked, this, &MainWindow::onCalendarDateClicked);

    showAppointmentsOnCalendar();
}

void MainWindow::addAppointment() {
    int jour = ui->jourEdit->text().toInt();
    int mois = ui->moisEdit->text().toInt();
    int annee = ui->anneeEdit->text().toInt();
    int heure = ui->heureEdit->text().toInt();
    QString position = ui->adresseComboBox->currentText();
    int id = ui->idEdit->text().toInt();

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

    // Create a QDate object from the input date
    QDate appointmentDate(annee, mois, jour);

    // Validate that the appointment date is valid and greater than today's date (ignoring time)
    if (!appointmentDate.isValid() || appointmentDate < QDate::currentDate()) {
        QMessageBox::warning(this, "Erreur", "La date du rendez-vous doit être ultérieure à la date actuelle.");
        return;
    }

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

        // Add to the appointments map
        QString appointmentDetails = QString("Rendez-vous ID %1 à %2 heures - Adresse: %3")
                                    .arg(id)
                                    .arg(heure)
                                    .arg(position);

        if (!appointments.contains(appointmentDate)) {
            appointments[appointmentDate] = QStringList();
        }
        appointments[appointmentDate].append(appointmentDetails);

        // Refresh the calendar to show the updated appointments
        showAppointmentsOnCalendar();

        loadAppointments();  // Reload appointments if needed
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de l'ajout : " + query.lastError().text());
    }
    showAppointmentsOnCalendar();
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






// Function to calculate statistics for the most popular country


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

    // Create a QTextDocument to generate the content for the PDF
    QTextDocument document;

    // Create the HTML content with the table structure
    QString htmlContent;
    htmlContent.append("<html><body>");
    htmlContent.append("<h1 style='text-align: center;'>Liste des Rendez-vous</h1>");
    htmlContent.append("<table border='1' cellspacing='0' cellpadding='4' style='width: 100%; border-collapse: collapse;'>");
    htmlContent.append("<tr style='background-color: #f2f2f2;'>");
    htmlContent.append("<th>ID</th><th>Jour</th><th>Mois</th><th>Année</th><th>Heure</th><th>Adresse</th>");
    htmlContent.append("</tr>");

    // Query the database to get all the appointments
    QSqlQuery query("SELECT * FROM rdv");
    while (query.next()) {
        htmlContent.append("<tr>");
        htmlContent.append("<td>" + query.value(0).toString() + "</td>");  // ID
        htmlContent.append("<td>" + query.value(1).toString() + "</td>");  // Jour
        htmlContent.append("<td>" + query.value(2).toString() + "</td>");  // Mois
        htmlContent.append("<td>" + query.value(3).toString() + "</td>");  // Année
        htmlContent.append("<td>" + query.value(4).toString() + "</td>");  // Heure
        htmlContent.append("<td>" + query.value(5).toString() + "</td>");  // Adresse
        htmlContent.append("</tr>");
    }
    htmlContent.append("</table>");
    htmlContent.append("</body></html>");

    // Set the HTML content into the QTextDocument
    document.setHtml(htmlContent);

    // Set up the QPrinter to output the content to a PDF file
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageMargins(QMargins(20, 20, 20, 20));  // Set margins (left, top, right, bottom)

    // Print the document to the PDF
    document.print(&printer);

    // Show a success message
    QMessageBox::information(this, "Succès", "Exportation réussie.");
}



void MainWindow::generateCountryStatistics() {
    // Create a QMap to store the country count
    QMap<QString, int> countryCount;

    // Query the database to get all the addresses (or countries)
    QSqlQuery query("SELECT Adresse FROM rdv");
    while (query.next()) {
        QString country = query.value(0).toString();
        if (!country.isEmpty()) {
            countryCount[country]++;  // Count occurrences of each country
        }
    }

    // If no data exists, show a message box
    if (countryCount.isEmpty()) {
        QMessageBox::information(this, "Statistiques", "Aucune donnée sur les pays.");
        return;
    }

    // Create a dialog to show the chart
    QDialog *chartDialog = new QDialog(this);
    chartDialog->setWindowTitle("Statistiques des Pays");

    // Create a custom widget to draw the bar chart
    QWidget *chartWidget = new QWidget(chartDialog);
    chartWidget->setMinimumSize(600, 400);

    // Create a custom widget to handle the painting
    class ChartWidget : public QWidget {
    public:
        ChartWidget(QWidget *parent, const QMap<QString, int>& data)
            : QWidget(parent), countryCount(data) {}

    protected:
        void paintEvent(QPaintEvent *event) override {
            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing);

            int chartWidth = width() - 40;
            int chartHeight = height() - 40;
            int margin = 20;

            if (countryCount.isEmpty()) return;

            // Calculate maximum value to normalize the bars
            int maxCount = *std::max_element(countryCount.begin(), countryCount.end());

            int barWidth = chartWidth / countryCount.size();
            int xOffset = 20;  // Starting X position for the first bar

            painter.setPen(Qt::black);
            painter.setBrush(Qt::blue);

            // Draw bars for each country
            QMapIterator<QString, int> i(countryCount);
            while (i.hasNext()) {
                i.next();
                int barHeight = (i.value() * chartHeight) / maxCount;  // Normalize bar height
                painter.drawRect(xOffset, chartHeight - barHeight + margin, barWidth - 5, barHeight);  // Draw each bar

                // Draw country names at the bottom
                painter.setPen(Qt::black);
                painter.drawText(xOffset, chartHeight + 10, i.key());

                xOffset += barWidth;
            }
        }

    private:
        QMap<QString, int> countryCount;
    };

    // Create the chart widget instance
    ChartWidget *chart = new ChartWidget(chartWidget, countryCount);

    // Add the chart widget to the dialog layout
    QVBoxLayout *layout = new QVBoxLayout(chartDialog);
    layout->addWidget(chart);

    // Show the dialog
    chartDialog->resize(600, 400);
    chartDialog->exec();}

void MainWindow::showAppointmentsOnCalendar() {
    // Clear existing date text formats or decorations
    for (auto i = appointments.begin(); i != appointments.end(); ++i) {
        QDate date = i.key();
        QStringList details = i.value();
        // Set a format for dates that have appointments
        QTextCharFormat format;
        format.setFontWeight(QFont::Bold);
        format.setForeground(Qt::yellow); //
        ui->calendarWidget->setDateTextFormat(date, format);

        // Set a tooltip for each date with appointments
        QString tooltipText = "Rendez-vous:\n" + details.join("\n");
        ui->calendarWidget->setToolTip(tooltipText);
    }
}
void MainWindow::onCalendarDateClicked(const QDate &date) {
    if (appointments.contains(date)) {
        QString details = "Rendez-vous pour le " + date.toString() + ":\n" + appointments[date].join("\n");
        QMessageBox::information(this, "Détails des rendez-vous", details);
    }
}
