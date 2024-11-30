
#include "ServiceMain.h"
#include "ui_ServiceMain.h"
#include "connection.h"
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
     ui->tableView->setModel(tmpService.afficher());

       // Appeler la méthode pour charger les données au démarrage
       afficherServices();
       connect(ui->chat, &QPushButton::clicked, this, &ServiceMain::on_chat_clicked);
}

ServiceMain::~ServiceMain() {
    delete ui;
}

/*void ServiceMain::on_pushButton_clicked() {
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
/*void ServiceMain::on_pushButton_clicked()
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


/*

void ServiceMain::on_conversionenpdf_clicked() {
    qDebug() << "Génération du PDF...";

    if (tmpService.genererPDF()) {
        QMessageBox::information(this, "PDF", "Le fichier PDF a été généré avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la génération du fichier PDF.");
    }
}

*/

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

/*void ServiceMain::on_statistique_clicked() {
    // Préparer la requête pour obtenir les données de statistiques
    QSqlQuery query("SELECT etats, COUNT(*) AS count FROM services GROUP BY etats");
    QVector<QPair<QString, int>> data;

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les données de statistiques.");
        return;
    }

    // Parcourir les résultats et les ajouter au vecteur
    while (query.next()) {
        QString etat = query.value("etats").toString(); // Nom correct du champ
        int count = query.value("count").toInt();
        data.append(qMakePair(etat, count));
    }

    // Vérification si des données sont disponibles
    if (data.isEmpty()) {
        QMessageBox::warning(this, "Statistiques", "Aucune donnée disponible pour les statistiques.");
        return;
    }

    // Dessiner le graphique en camembert
    const int width = 400, height = 400;
    QImage pieChartImage(width, height, QImage::Format_ARGB32);
    pieChartImage.fill(Qt::white);

    QPainter painter(&pieChartImage);
    painter.setRenderHint(QPainter::Antialiasing);

    int total = 0;
    for (const auto &item : data) total += item.second;

    QRectF rect(10, 10, width - 20, height - 20);
    int startAngle = 0;

    QVector<QColor> colors = {Qt::red, Qt::blue, Qt::green, Qt::yellow, Qt::cyan};
    for (int i = 0; i < data.size(); ++i) {
        int angleSpan = static_cast<int>(360.0 * data[i].second / total * 16); // Calcul de l'angle
        painter.setBrush(colors[i % colors.size()]);
        painter.drawPie(rect, startAngle, angleSpan);

        // Légende
        QString label = QString("%1: %2").arg(data[i].first).arg(data[i].second);
        painter.drawText(rect.center() + QPoint(0, i * 20 + 10), label);

        startAngle += angleSpan;
    }

    // Afficher le graphique
    QLabel *label = new QLabel;
    label->setPixmap(QPixmap::fromImage(pieChartImage));
    label->setWindowTitle("Statistiques des services");
    label->setAttribute(Qt::WA_DeleteOnClose);
    label->show();
}
*/
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



