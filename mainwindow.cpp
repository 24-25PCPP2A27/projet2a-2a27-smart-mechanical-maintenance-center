/*#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQueryModel>
#include <QSqlRecord>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ajouterButton_clicked()
{
    int id = ui->idserv->text().toInt();             // Changed to match widget name
    QString type = ui->typeserv->text();             // Changed to match widget name
    double duree = ui->dureeserv->text().toDouble(); // Changed to match widget name
    QString etats = ui->etatsserv->currentText();    // Changed to match widget name
    float cout = ui->coutserv->text().toFloat();     // Changed to match widget name

    if (tmpService.ajouter(id, type, duree, etats, cout)) {
        ui->label->setText("Service ajouté avec succès.");
    } else {
        ui->label->setText("Erreur lors de l'ajout du service.");
    }
}

void MainWindow::on_modifierButton_clicked()
{
    int id = ui->idserv->text().toInt();             // Changed to match widget name
    QString type = ui->typeserv->text();             // Changed to match widget name
    double duree = ui->dureeserv->text().toDouble(); // Changed to match widget name
    double cout = ui->coutserv->text().toDouble();   // Changed to match widget name

    if (tmpService.modifier(id, type, duree, cout)) {
        ui->label->setText("Service modifié avec succès.");
    } else {
        ui->label->setText("Erreur lors de la modification du service.");
    }
}

void MainWindow::on_supprimerButton_clicked()
{
    int id = ui->idserv->text().toInt();             // Changed to match widget name

    if (tmpService.supprimer(id)) {
        ui->label->setText("Service supprimé avec succès.");
    } else {
        ui->label->setText("Erreur lors de la suppression du service.");
    }
}

void MainWindow::on_rechercherButton_clicked()
{
    QString keyword = ui->rechercheLineEdit->text(); // Changed to match widget name
    QSqlQueryModel *model = tmpService.rechercher(keyword);

    if (model->rowCount() > 0) {
        QString result;
        for (int i = 0; i < model->rowCount(); ++i) {
            QSqlRecord record = model->record(i);
            result += "ID: " + record.value("id").toString() + "\n";
            result += "Type: " + record.value("type").toString() + "\n";
            result += "Duree: " + record.value("duree").toString() + "\n";
            result += "Cout: " + record.value("cout").toString() + "\n";
            result += "Etat: " + record.value("etats").toString() + "\n\n";
        }
        ui->label->setText(result);
    } else {
        ui->label->setText("Aucun résultat trouvé.");
    }
}

void MainWindow::on_afficherButton_clicked()
{
    QString critere = ui->critereComboBox->currentText(); // Changed to match widget name
    QString ordre = ui->ordreComboBox->currentText();     // Changed to match widget name

    QSqlQueryModel *model = tmpService.trier(critere, ordre);

    if (model->rowCount() > 0) {
        QString result;
        for (int i = 0; i < model->rowCount(); ++i) {
            QSqlRecord record = model->record(i);
            result += "ID: " + record.value("id").toString() + "\n";
            result += "Type: " + record.value("type").toString() + "\n";
            result += "Duree: " + record.value("duree").toString() + "\n";
            result += "Cout: " + record.value("cout").toString() + "\n";
            result += "Etat: " + record.value("etats").toString() + "\n\n";
        }
        ui->label->setText(result);
    } else {
        ui->label->setText("Aucun service à afficher.");
    }
}

void MainWindow::on_trierButton_clicked()
{
    // This method is now handled by the on_afficherButton_clicked() as both are similar.
}

void MainWindow::on_pdfButton_clicked()
{
    // Implement PDF export functionality
    ui->label->setText("Fonction PDF non implémentée.");
}
*/
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
#include <QPainter>
#include <QPrinter>
#include <QSqlTableModel>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) , ui(new Ui::MainWindow) {
    ui->setupUi(this);
     ui->tableView->setModel(tmpService.afficher());
    // Initialiser la base de données
       QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE"); // Changez en fonction de votre type de base de données
       db.setDatabaseName("services.db"); // Chemin vers votre base de données
       if (!db.open()) {
           qDebug() << "Erreur lors de l'ouverture de la base de données.";
           return;
       }

       // Appeler la méthode pour charger les données au démarrage
       afficherServices();
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





void MainWindow::on_conversionenpdf_clicked() {
    qDebug() << "Génération du PDF...";

    // Connexion à la base de données
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DSN=CPP_Project"); // Remplacez par votre nom de source de données

    if (!db.open()) {
        qDebug() << "Erreur de connexion à la base de données:" << db.lastError().text();
        QMessageBox::critical(this, "Erreur", "Connexion à la base de données échouée.");
        return;
    }

    tmpService.genererPDF(); // Passez la base de données
    QMessageBox::information(this, "PDF", "Fichier PDF généré avec succès.");
}


/*

void MainWindow::on_conversionenpdf_clicked() {
    qDebug() << "Génération du PDF...";

    if (tmpService.genererPDF()) {
        QMessageBox::information(this, "PDF", "Le fichier PDF a été généré avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la génération du fichier PDF.");
    }
}

*/

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




/*void MainWindow::on_statistique_clicked() {
    QSqlQuery query("SELECT etats, COUNT(*) AS count FROM services GROUP BY etats");
    QVector<QPair<QString, int>> data;

    while (query.next()) {
        QString etatserv = query.value("etatsserv").toString();
        int count = query.value("count").toInt();
        data.append(qMakePair(etatserv, count));
    }

    if (data.isEmpty()) {
        QMessageBox::warning(this, "Statistiques", "Aucune donnée disponible pour les statistiques.");
        return;
    }

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
        int angleSpan = static_cast<int>(360.0 * data[i].second / total * 16);
        painter.setBrush(colors[i % colors.size()]);
        painter.drawPie(rect, startAngle, angleSpan);
        startAngle += angleSpan;
    }

    QLabel *label = new QLabel;
    label->setPixmap(QPixmap::fromImage(pieChartImage));
    label->show();
}
*/
void MainWindow::on_rechercher_clicked() {
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

void MainWindow::on_statistique_clicked() {
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






void MainWindow::on_updateButton_clicked() {
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
        query.prepare("UPDATE Services SET TYPESERV = :typeserv, DUREESERV = :dureeserv, ETATSSERV = :etatsserv, COUTSERV = :coutserv WHERE IDSERV = :idserv");
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

void MainWindow::on_deleteButton_clicked() {
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



void MainWindow::afficherServices()
{
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable("services"); // Assurez-vous que "services" est le nom de votre table SQL
    model->select();

    ui->tableView->setModel(model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}
void MainWindow::onSendMessage() {
 /*   QString userInput = ui->inputField->text();

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

*/
}
