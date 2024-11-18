#include "mainwindow.h"
#include "ui_mainwindow.h"
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Initial setup: display the service table
    ui->tableView->setModel(tmpService.afficher());
    ui->chatDisplay->setReadOnly(true);
        connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::onSendMessage);
        connect(ui->inputField, &QLineEdit::returnPressed, ui->sendButton, &QPushButton::click);

}

MainWindow::~MainWindow() {
    delete ui;
}

/*void MainWindow::on_pushButton_clicked() {
    bool ok ;
    int id = ui->idserv->text().toInt(&ok);
    double cout = ui->coutserv->text().toDouble();
    QString type = ui->typeserv->text();
    double duree = ui->lineduree->text().toDouble();
    QString etats = ui->boxetats->currentText();


    // Validate inputs
    if (id <= 0 || cout <= 0 || type.isEmpty() || duree <= 0 || etats.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir correctement tous les champs.");
        return;
    }

    Service s(id, cout, type, duree, etats);
    if (s.ajouter()) {
        ui->tableView->setModel(tmpService.afficher());
        QMessageBox::information(this, "Ajout", "Service ajouté avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout.");
    }
}*/
/*void MainWindow::on_pushButton_clicked()
{
    bool ok;

    // Lecture et vérification de l'ID
    int id = ui->idserv->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("L'ID doit être un nombre entier."),
                             QMessageBox::Ok);
        return;
    }

    // Lecture du type
    QString type = ui->typeserv->text();
    if (type.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("Le champ Type ne peut pas être vide."),
                             QMessageBox::Ok);
        return;
    }

    // Lecture et vérification du coût
    double cout = ui->coutserv->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("Le coût doit être un nombre valide."),
                             QMessageBox::Ok);
        return;
    }

    // Lecture et vérification de la durée
    double duree = ui->dureeserv->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("La durée doit être un nombre valide."),
                             QMessageBox::Ok);
        return;
    }

    // Lecture de l'état
    QString etats = ui->etatsserv->currentText();
    if (etats.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur de saisie"),
                             tr("Veuillez sélectionner un état valide."),
                             QMessageBox::Ok);
        return;
    }

    // Création du service et ajout
    Service s(id, cout, type, duree, etats);
    bool test = s.ajouter();

    // Vérification du résultat de l'ajout
    if (test) {
        QMessageBox::information(this, tr("Succès"),
                                 tr("Ajout du service effectué avec succès."),
                                 QMessageBox::Ok);

        // Réinitialiser les champs
        ui->idserv->clear();
        ui->typeserv->clear();
        ui->coutserv->clear();
        ui->dureeserv->clear();
        ui->etatsserv->setCurrentIndex(0);
    } else {
        QMessageBox::critical(this, tr("Erreur"),
                              tr("Échec de l'ajout du service."),
                              QMessageBox::Ok);
    }
}
*/
void MainWindow::on_pushButton_clicked() {

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




void MainWindow::on_conversionenpdf_clicked() {
    qDebug() << "Génération du PDF...";
    tmpService.genererPDF();
    QMessageBox::information(this, "PDF", "Fichier PDF généré avec succès.");
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DSN=CPP_Project"); // Remplacez par votre nom de source de données
    if (!db.open()) {
        qDebug() << "Erreur de connexion à la base de données:" << db.lastError().text();
        return;
    }
}

void MainWindow::on_afficher_clicked() {
    ui->tableView->setModel(tmpService.afficher());
}

/*void MainWindow::on_trier_clicked() {


    ui->tableView->setModel(tmpService.trier("id", "ASC"));
}*/
void MainWindow::on_trier_clicked() {
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




void MainWindow::on_statistique_clicked() {
    // 1. Obtenez les données de statistiques pour les services
    QSqlQueryModel* model = serviceInstance.obtenirStatistiquesService();

    // Vérifiez que la requête a réussi
    if (model) {
        // Définir le modèle pour tableView_2
        ui->tableView_2->setModel(model);
        ui->tableView_2->resizeColumnsToContents();
        ui->tableView_2->resizeRowsToContents();

        // Créez l'image du graphique en secteurs (pie chart)
        QVector<QPair<QString, int>> chartData;
        for (int row = 0; row < model->rowCount(); ++row) {
            QString etat = model->data(model->index(row, 0)).toString();
            int count = model->data(model->index(row, 1)).toInt();
            chartData.append(qMakePair(etat, count));
        }

        // Créez l'image pour le graphique en secteurs (pie chart)
        const int pieChartWidth = 400;
        const int pieChartHeight = 400;
        QImage pieChartImage(pieChartWidth, pieChartHeight, QImage::Format_ARGB32);
        pieChartImage.fill(Qt::white);  // Remplir l'image avec un fond blanc

        QPainter pieChartPainter(&pieChartImage);
        pieChartPainter.setRenderHint(QPainter::Antialiasing);

        int total = 0;
        for (const auto &item : chartData) {
            total += item.second;
        }

        QVector<QColor> colorPalette = {
            QColor(255, 99, 71), // Tomato red
            QColor(70, 130, 180), // Steel blue
            QColor(34, 139, 34),  // Forest green
            QColor(255, 223, 0),  // Bright yellow
            QColor(255, 165, 0),  // Orange
            QColor(138, 43, 226), // Blue violet
            QColor(0, 255, 255)   // Cyan
        };

        int startAngle = 0;
        QRectF pieRect(50, 50, 300, 300);
        int colorIndex = 0;

        for (const auto &item : chartData) {
            int spanAngle = (item.second * 360) / total;

            QColor color = colorPalette[colorIndex % colorPalette.size()];
            pieChartPainter.setBrush(color);
            pieChartPainter.drawPie(pieRect, startAngle * 16, spanAngle * 16);

            int midAngle = startAngle + spanAngle / 2;
            QPointF textPosition = pieRect.center();
            textPosition.setX(textPosition.x() + 150 * qCos(midAngle * M_PI / 180));
            textPosition.setY(textPosition.y() + 150 * qSin(midAngle * M_PI / 180));

            pieChartPainter.setPen(Qt::black);
            pieChartPainter.drawText(textPosition, item.first);

            startAngle += spanAngle;
            colorIndex++;
        }

        QPixmap pieChartPixmap = QPixmap::fromImage(pieChartImage);

        QStandardItemModel* pieChartModel = new QStandardItemModel(1, 1, this);
        QStandardItem* pieChartItem = new QStandardItem();
        pieChartItem->setData(pieChartPixmap, Qt::DecorationRole);
        pieChartModel->setItem(0, 0, pieChartItem);
        ui->tableView_2->setModel(pieChartModel);

        ui->tableView_2->resizeColumnsToContents();
        ui->tableView_2->resizeRowsToContents();
    } else {
        QMessageBox::warning(this, tr("Erreur"), tr("Échec de la récupération des statistiques des services."));
    }
}


void MainWindow::on_rechercher_clicked() {
    QString keyword = ui->idrech->text();

    if (keyword.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer IDS pour la recherche.");
        return;
    }

    ui->tableView->setModel(tmpService.rechercher(keyword));
}


void MainWindow::on_updateButton_clicked() {
    int idserv = ui->idmaj->text().toInt();

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
        QString newType = ui->newtype->text();  // Get the selected value from the combo box
        QString newDuree = ui->newduree->text();
        QString newEtats = ui->comboBox_etats->currentText();
        double newCout = ui->newcout->text().toDouble();

        // Validate the other fields
        if (newType.isEmpty() || newDuree.isEmpty() || newEtats.isEmpty() || newCout <= 0.0) {
            QMessageBox::warning(this, "Error", "Please fill in all fields with valid data.");
            return;
        }

        // Prepare the query to update the service record
        QSqlQuery query;
        query.prepare("UPDATE Services SET TYPE = :type, DUREE = :duree, ETATS = :etats, COUT = :cout WHERE IDSERV = :idserv");
        query.bindValue(":type", newType);
        query.bindValue(":duree", newDuree);
        query.bindValue(":etats", newEtats);
        query.bindValue(":cout", newCout);
        query.bindValue(":idserv", idserv);

        // Execute the query and check the result
        if (query.exec()) {
            if (query.numRowsAffected() > 0) {
                QMessageBox::information(this, "Success", "Service updated successfully.");
                // Clear input fields after successful update
                ui->idmaj->clear();
                ui->newtype->clear();  // Reset combo box to first item
                ui->newduree->clear();
                ui->comboBox_etats->setCurrentIndex(0);
                ui->newcout->clear();
            } else {
                QMessageBox::warning(this, "Error", "No changes were made to the service data.");
            }
        } else {
            QMessageBox::warning(this, "Error", "Failed to update the service. Please try again.");
            qDebug() << "Database Error: " << query.lastError().text();
        }
}

void MainWindow::on_deleteButton_clicked() {
    int idserv = ui->lineEdit_7->text().toInt();

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
                ui->lineEdit_7->clear();
            } else {
                QMessageBox::warning(this, "Error", "No service found with the given ID.");
            }
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete the service. Please try again.");
            qDebug() << "Database Error: " << query.lastError().text();
        }
}
void MainWindow::onSendMessage() {
    QString userInput = ui->inputField->text();

    if (userInput.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter a message.");
        return;
    }

    // Append user message to chat display
    ui->chatDisplay->append("You: " + userInput);

    // Process the user's input
    QString response;
    if (userInput.toLower().contains("information")) {
        response = "Please provide your service ID.";
    } else if (userInput.toInt() > 0) { // Assume user entered an ID
        int idServ = userInput.toInt();
        QSqlQuery query;
        query.prepare("SELECT type, duree, etats, cout FROM Services WHERE idServ = :idServ");
        query.bindValue(":idServ", idServ);

        if (query.exec() && query.next()) {
            QString type = query.value(0).toString();
            QString duree = query.value(1).toString();
            QString etats = query.value(2).toString();
            double cout = query.value(3).toDouble();

            response = QString("Service Found:\nType: %1\nDuration: %2\nStatus: %3\nCost: %4")
                           .arg(type)
                           .arg(duree)
                           .arg(etats)
                           .arg(cout);
        } else {
            response = "No service found with the given ID.";
        }
    } else if (userInput.toLower().contains("cost")) {
        response = "Please provide the service ID to check the cost.";
    } else {
        response = "Sorry, I didn't understand that. You can ask for service information or cost.";
    }

    // Append bot response to chat display
    ui->chatDisplay->append("Bot: " + response);

    // Clear input field
    ui->inputField->clear();
}
