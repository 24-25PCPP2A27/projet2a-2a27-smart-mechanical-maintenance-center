
#include "ServiceMain.h"
#include "ui_ServiceMain.h"
#include "connection.h"
#include "arduino.h"
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QPainter>
#include <QVector>
#include <QPair>
#include <QtMath>  // Pour qCos, qSin, M_PI
#include <QStandardItemModel>  // Pour QStandardItemModel
#include <QStandardItem>
#include <QSqlError>   // Pour QSqlError
#include <QDebug>
#include <QPainter>
#include <QPrinter>
#include <QSqlTableModel>
#include "qcustomplot.h"
#include "chatbotdialog.h"
#include <QSqlError>


ServiceMain::ServiceMain(QWidget *parent)
    : QMainWindow(parent) , ui(new Ui::ServiceMain) {
    ui->setupUi(this);
    setupArduinoConnection();
    connect(arduino.getserial(), &QSerialPort::readyRead, this, &ServiceMain::onArduinoDataReceived);

     afficherServices();  // Refresh the table view

       // Appeler la méthode pour charger les données au démarrage
       afficherServices();
       connect(ui->chat, &QPushButton::clicked, this, &ServiceMain::on_chat_clicked);
       // Connexion du bouton à la fonction de changement d'état
}

ServiceMain::~ServiceMain() {
    delete ui;
}


void ServiceMain::on_pushButton_clicked() {
    // Get input values from the UI
    int idserv = ui->idserv->text().toInt();  // ID input
    QString type = ui->typeserv->text();      // Type input
    QString duree = ui->dureeserv->text();    // Duration input
    QString etats = ui->etatsserv->currentText();  // Status input

    double cout = ui->coutserv->text().toDouble(); // Cost input

    // Validate the input
    if (idserv <= 0 || type.isEmpty() || duree.isEmpty() || etats.isEmpty() || cout <= 0.0) {
        QMessageBox::warning(this, "Error", "Please fill in all fields with valid data.");
        return;
    }

    // Ensure duree is a valid number
    bool dureeValid;
    double dureeValue = duree.toDouble(&dureeValid);
    if (!dureeValid || dureeValue <= 0) {
        QMessageBox::warning(this, "Error", "Please enter a valid duration.");
        return;
    }

    // Create a Service object with the input data
    Service service(idserv, cout, type, dureeValue, etats);

    // Use the Service class's ajouter() method to add the service to the database
    if (service.ajouter(idserv, type, duree, etats, cout)) {
        QMessageBox::information(this, "Success", "Service added successfully!");

        // Refresh the view with the updated data
        ui->tableView->setModel(service.afficher());  // Refresh the table view
        afficherServices();  // Refresh the table view
        // Clear fields after successful addition
        ui->idserv->clear();
        ui->typeserv->clear();
        ui->dureeserv->clear();
        ui->etatsserv->setCurrentIndex(0);

        ui->coutserv->clear();
    } else {
        QMessageBox::warning(this, "Error", "Failed to add service. Check for duplicate ID or database errors.");
    }
}







void ServiceMain::on_conversionenpdf_clicked() {
    qDebug() << "Génération du PDF...";

    // Call the PDF generation function
    tmpService.genererPDF();
    QMessageBox::information(this, "PDF", "Fichier PDF généré avec succès.");
}




void ServiceMain::on_trier_clicked() {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, tr("Erreur"), tr("La connexion à la base de données est fermée."));
        return;
    }

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    // Exemple de tri par 'idserv'
    query.prepare("SELECT * FROM services ORDER BY idserv ASC");

    if (query.exec()) {
        model->setQuery(query);
        ui->tableView->setModel(model);
        QMessageBox::information(this, tr("Succès"), tr("Les données ont été triées avec succès !"));
    } else {
        QMessageBox::critical(this, tr("Erreur SQL"), tr("Impossible d'exécuter la requête : %1").arg(query.lastError().text()));
        qDebug() << "Erreur SQL : " << query.lastError().text();
    }
}





void ServiceMain::on_rechercher_clicked() {
    int idrech = ui->idrech->text().toInt();
    if (idrech <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID valide.");
        return;
    }

    Service service;
    QSqlQueryModel* model = service.rechercher(idrech);

    if (model->rowCount() > 0) {
        ui->tableView->setModel(model); // Afficher les résultats dans le tableau
    } else {
        QMessageBox::warning(this, "Recherche", "Aucun service trouvé avec cet ID.");
        delete model; // Libérer la mémoire si aucun résultat
    }
}


void ServiceMain::on_statistique_clicked() {
    QMap<QString, int> stats = tmpService.statistiquesParEtats();
    if (stats.isEmpty()) {
        QMessageBox::warning(this, "Statistiques", "Aucune donnée trouvée pour les statistiques par états.");
        return;
    }

    // Create a QCustomPlot widget
    QCustomPlot *customPlot = new QCustomPlot();
    customPlot->setFixedSize(641, 441); // Set custom size

    // Create a QGraphicsScene and add the customPlot to it
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addWidget(customPlot);

    // Set the scene to the QGraphicsView
    ui->graphicsView_salaire->setScene(scene);

    // Prepare data for the chart
    QVector<double> xValues;
    QVector<double> yValues;
    QVector<QString> labels; // To hold the labels for the x-axis
    int index = 0;
    for (auto it = stats.begin(); it != stats.end(); ++it) {
        xValues.append(index++);
        yValues.append(it.value());
        labels.append(it.key()); // Store label for each data point
    }

    // Add a bar graph to the custom plot
    customPlot->addGraph();
    customPlot->graph(0)->setData(xValues, yValues);
    customPlot->graph(0)->setName("Statistiques des services");

    // Set the labels for the axes
    customPlot->xAxis->setLabel("État");
    customPlot->yAxis->setLabel("Nombre de services");

    // Set custom tick labels for the x-axis using a ticker
    QSharedPointer<QCPAxisTickerText> ticker(new QCPAxisTickerText());
    ticker->addTicks(xValues, labels);
    customPlot->xAxis->setTicker(ticker);

    // Adjust y-axis range to fit the data
    customPlot->yAxis->setRange(0, *std::max_element(yValues.begin(), yValues.end()) + 1);

    // Rotate x-axis labels for better readability (if needed)
    customPlot->xAxis->setTickLabelRotation(45);

    // Replot the chart to apply the changes
    customPlot->replot();
}



void ServiceMain::on_updateButton_clicked() {
    int idserv = ui->idserv->text().toInt();

        // Validate the ID (ensure it is greater than 0)
        if (idserv <= 0) {
            QMessageBox::warning(this, "Error", "Please enter a valid Service ID.");
            return;
        }

        // Check if the service ID exists in the database
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT COUNT(*) FROM Services WHERE IDSERV = :idserv");
        checkQuery.bindValue(":idserv", idserv);

        if (!checkQuery.exec()) {
            QMessageBox::warning(this, "Error", "Failed to check the Service ID. Please try again.");
            return;
        }

        checkQuery.next();
        if (checkQuery.value(0).toInt() == 0) {
            QMessageBox::warning(this, "Error", "No service found with the given ID.");
            return;
        }

        // Get new values for the other fields
        QString newType = ui->typeserv->text();  // Get the selected value from the combo box
        QString newDuree = ui->dureeserv->text();
        QString newEtats = ui->etatsserv->currentText();
        double newCout = ui->coutserv->text().toDouble();

        // Validate the other fields
        if (newType.isEmpty() || newDuree.isEmpty() || newEtats.isEmpty() || newCout <= 0.0) {
            QMessageBox::warning(this, "Error", "Please fill in all fields with valid data.");
            return;
        }

        // Prepare the query to update the service record
        QSqlQuery query;
        query.prepare("UPDATE Services SET TYPESERV = :typeserv, DUREESERV = :dureeserv, ETATSERV = :etatsserv, COUTSERV = :coutserv WHERE IDSERV = :idserv");
        query.bindValue(":typeserv", newType);
        query.bindValue(":dureeserv", newDuree);
        query.bindValue(":etatsserv", newEtats);
        query.bindValue(":coutserv", newCout);
        query.bindValue(":idserv", idserv);

        // Execute the query and check the result
        if (query.exec()) {
            if (query.numRowsAffected() > 0) {
                QMessageBox::information(this, "Success", "Service updated successfully.");
                ui->tableView->setModel(tmpService.afficher());
                // Clear input fields after successful update
                ui->idserv->clear();
                ui->typeserv->clear();  // Reset combo box to first item
                ui->dureeserv->clear();
                ui->etatsserv->setCurrentIndex(0);
                ui->coutserv->clear();
            } else {
                QMessageBox::warning(this, "Error", "No changes were made to the service data.");
            }
        } else {
            QMessageBox::warning(this, "Error", "Failed to update the service. Please try again.");
            qDebug() << "Database Error: " << query.lastError().text();
        }
}


void ServiceMain::on_deleteButton_clicked() {
    int idserv = ui->idserv->text().toInt();

        // Validate the input
        if (idserv <= 0) {
            QMessageBox::warning(this, "Error", "Please enter a valid service ID.");
            return;
        }

        // Prepare the query to delete the service
        QSqlQuery query;
        query.prepare("DELETE FROM Services WHERE IDSERV = :idserv");
        query.bindValue(":idserv", idserv);

        // Execute the query and check the result
        if (query.exec()) {
            if (query.numRowsAffected() > 0) {
                QMessageBox::information(this, "Success", "Service deleted successfully.");
                 ui->tableView->setModel(tmpService.afficher());
                ui->idserv->clear();
            } else {
                QMessageBox::warning(this, "Error", "No service found with the given ID.");
            }
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete the service. Please try again.");
            qDebug() << "Database Error: " << query.lastError().text();
        }
}



void ServiceMain::afficherServices() {
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable("Services");
    model->select();  // This ensures the latest data is fetched from the database

    ui->tableView->setModel(model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void ServiceMain::on_chat_clicked() {
    ChatBotDialog chatbotDialog(&tmpService, this); // Pass the Service object
        chatbotDialog.exec();
}

void ServiceMain::setupArduinoConnection()
{
    // Attempt to connect to Arduino
    int result = arduino.connect_arduino();
    if (result == 0) {
        qDebug() << "Arduino connected successfully on port:" << arduino.getarduino_port_name();

        // Connect Arduino data reception to slot
        connect(arduino.getserial(), &QSerialPort::readyRead, this, &ServiceMain::onArduinoDataReceived);
    } else if (result == 1) {
        qDebug() << "Failed to open Arduino serial port.";
    } else {
        qDebug() << "Arduino not available.";
    }
}
void ServiceMain::onArduinoDataReceived()
{
    // Read the data from Arduino
    QByteArray data = arduino.read_from_arduino().trimmed();

    // Debug log to verify received data
    qDebug() << "Received data from Arduino:" << data;

    // Check if the received data matches "1"
    if (data == "1") {
        // Get the ID from the input field
        int idserv = ui->idserv->text().toInt();

        // Validate the input ID
        if (idserv <= 0) {
            QMessageBox::warning(this, "Error", "Please enter a valid ID before using Arduino.");
            return;
        }

        // Query the current state of the equipment
        QSqlQuery query;
        query.prepare("SELECT ETAT FROM GESTION_EQUIPEMENTS WHERE ID = :idserv");
        query.bindValue(":idserv", idserv);

        if (query.exec()) {
            if (query.next()) {
                // Fetch the current state
                QString currentState = query.value("ETAT").toString();
                qDebug() << "Current state of equipment:" << currentState;

                // Determine the new state
                QString newState = (currentState == "en service") ? "en panne" : "en service";

                // Update the state in the database
                QSqlQuery updateQuery;
                updateQuery.prepare("UPDATE GESTION_EQUIPEMENTS SET ETAT = :newState WHERE ID = :idserv");
                updateQuery.bindValue(":newState", newState);
                updateQuery.bindValue(":idserv", idserv);

                if (updateQuery.exec()) {
                    QMessageBox::information(this, "Success",
                        QString("State updated successfully to '%1'.").arg(newState));
                } else {
                    QMessageBox::warning(this, "Database Error",
                        "Failed to update the state. Please try again.");
                    qDebug() << "Update error:" << updateQuery.lastError().text();
                }
            } else {
                QMessageBox::warning(this, "Error",
                    "No equipment found with the specified ID.");
            }
        } else {
            QMessageBox::warning(this, "Database Error",
                "Failed to query the database. Please check your connection.");
            qDebug() << "Query error:" << query.lastError().text();
        }
    }
}
