
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

Arduino arduino;
ServiceMain::ServiceMain(QWidget *parent)
    : QMainWindow(parent) , ui(new Ui::ServiceMain) {
    ui->setupUi(this);

     ui->tableView->setModel(tmpService.afficher());

       // Appeler la méthode pour charger les données au démarrage
       afficherServices();
       connect(ui->chat, &QPushButton::clicked, this, &ServiceMain::on_chat_clicked);
       // Connexion du bouton à la fonction de changement d'état
       connect(ui->pushButton_2, &QPushButton::clicked, this, &ServiceMain::changerEtatEquipement);
}

ServiceMain::~ServiceMain() {
    delete ui;
}


void ServiceMain::on_pushButton_clicked() {

        // Get input values from the UI
        int idserv = ui->idserv->text().toInt(); // Assuming this is used for ID input
        QString type = ui->typeserv->text();     // Assuming this is repurposed for TYPE input
        QString duree = ui->dureeserv->text(); // Assuming this is repurposed for DUREE input
        QString etats = ui->etatsserv->currentText(); // Assuming this is repurposed for ETATS input
        double cout = ui->coutserv->text().toDouble(); // Assuming this is repurposed for COUT input

        // Validate the input
        if (idserv <= 0 || type.isEmpty() || duree.isEmpty() || etats.isEmpty() || cout <= 0.0) {
            QMessageBox::warning(this, "Error", "Please fill in all fields with valid data.");
            return;
        }

        // Insert the data into the database
        QSqlQuery query;
        query.prepare("INSERT INTO Services (IDSERV, TYPE, DUREE, ETATS, COUT) "
                      "VALUES (:idserv, :type, :duree, :etats, :cout)");
        query.bindValue(":idserv", idserv);
        query.bindValue(":type", type);
        query.bindValue(":duree", duree);
        query.bindValue(":etats", etats);
        query.bindValue(":cout", cout);

        if (query.exec()) {
            QMessageBox::information(this, "Success", "Service added successfully!");
             ui->tableView->setModel(tmpService.afficher());
            // Clear fields after successful addition
            ui->idserv->clear();
            ui->typeserv->clear();
            ui->dureeserv->clear();
            ui->etatsserv->setCurrentIndex(0);
            ui->coutserv->clear();
        } else {
            QMessageBox::warning(this, "Error", "Failed to add service. Check for duplicate ID or database errors.");
            qDebug() << "Database Error: " << query.lastError().text();
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
        query.prepare("UPDATE Services SET TYPE = :typeserv, DUREE = :dureeserv, ETATS = :etatsserv, COUT = :coutserv WHERE IDSERV = :idserv");
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



void ServiceMain::afficherServices()
{
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable("Services");
    model->select();

    ui->tableView->setModel(model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}
void ServiceMain::on_chat_clicked() {
    ChatBotDialog chatbotDialog(&tmpService, this); // Pass the Service object
        chatbotDialog.exec();
}

/*void ServiceMain::changerEtatEquipement()
{
    QString etatActuel = ui->etatsserv_2->currentText();
    QString nouvelEtat;

    if (etatActuel == "en panne") {
        nouvelEtat = "en service";
    } else {
        nouvelEtat = "en panne";
    }

    // Changement de l'état dans l'interface utilisateur
    ui->etatsserv_2->setCurrentText(nouvelEtat);

    // Envoi de l'information à l'Arduino
    QByteArray commande = nouvelEtat == "en service" ? "1" : "0"; // Exemple de commande (1 pour "en service", 0 pour "en panne")
    arduino.write_to_arduino(commande);
}
*/
void ServiceMain::changerEtatEquipement()
{
    QString etatActuel = ui->etatsserv_2->currentText();
    QString nouvelEtat;

    // Détermination du nouvel état
    if (etatActuel == "en panne") {
        nouvelEtat = "en service";
    } else {
        nouvelEtat = "en panne";
    }

    // Mise à jour de l'état dans l'interface
    ui->etatsserv_2->setCurrentText(nouvelEtat);

    // Mise à jour de l'état dans l'objet Service
    service.setEtatEquipement(nouvelEtat);

    // Envoi de la commande à Arduino
    QByteArray commande = nouvelEtat == "en service" ? "1" : "0";
    arduino.write_to_arduino(commande);
}
