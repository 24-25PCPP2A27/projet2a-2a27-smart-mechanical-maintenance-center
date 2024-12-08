#include "EquipMain.h"
#include "ui_EquipMain.h"
#include "equipements.h"
#include <QApplication>
#include <QMessageBox>
#include <QLabel>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QByteArray>
#include <QBuffer>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts>
#include <QPainter>
#include "ImageDelegate.h"
#include <QSqlError>
#include "arduino.h"






EquipMain::EquipMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::EquipMain)
{
    ui->setupUi(this);
    initializeArduinoConnection();
}





EquipMain::~EquipMain()
{
    delete ui;
}
void EquipMain::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    QSqlQueryModel* model = E.afficher();
    ui->tab1->setModel(model);

    int rowHeight = 100;
    for (int i = 0; i < model->rowCount(); ++i) {
        ui->tab1->setRowHeight(i, rowHeight);
    }

    QString imageBaseDir = "C:/Users/MSI/Desktop/maintenance/Gestion equipements";
    ImageDelegate *imageDelegate = new ImageDelegate(imageBaseDir, this);
    ui->tab1->setItemDelegateForColumn(6, imageDelegate);

    ui->tab1->resizeColumnsToContents();
    ui->tab1->setAlternatingRowColors(true);

    ui->labelimage->setVisible(false);
}
void EquipMain::on_pushButton_clicked()
{
    int id = ui->id->text().toUInt();
    QString nom = ui->nom->text();
    QString modele = ui->modele->text();
    QString etat = ui->etat->currentText();
    QString date = ui->date->date().toString("yyyy-MM-dd");
    QString Emplacement = ui->Emplacement->currentText();

    QString imageName = ui->labelimage->text();
    if (!imageName.isEmpty()) {
        qDebug() << "Selected image name is: " << imageName;
    } else {
        qDebug() << "No image was selected or copy failed.";
    }
    // Validate input fields
    if (nom.isEmpty() || modele.isEmpty() || etat.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled out.");
        return;
    }

    if (id <= 0) {
        QMessageBox::warning(this, "Input Error", "ID must be a positive number.");
        return;
    }

    QDate dateInstallation = QDate::fromString(date, "yyyy-MM-dd"); // Convert date QString to QDate
    Equipement E(id, nom, modele, etat, dateInstallation, Emplacement, imageName);

    // Attempt to add the new service
    bool test = E.ajouter();
    if (test) {
        QMessageBox::information(this, QObject::tr("Ok"),
                                 QObject::tr("Service added successfully.\nClick Cancel to exit."), QMessageBox::Cancel);
        ui->id->clear();
        ui->nom->clear();
        ui->modele->clear();


        // Refresh the table view to show added service
        //ui->tableView->setModel(E.afficher());
        ui->tab1->setModel(E.afficher());
    } else {
        QMessageBox::critical(this, QObject::tr("Error"),
                              QObject::tr("Failed to add service.\nClick Cancel to exit."), QMessageBox::Cancel);
        //ui->tableView->setModel(E.afficher()); // Refresh the table view to show added service
        ui->tab1->setModel(E.afficher());
    }
}

void EquipMain::on_pushButton_2_clicked()
{
    int id = ui->supp->text().toInt();

    bool test = E.supprimer(id);

    if(test) {
        QMessageBox::information(nullptr, QObject::tr("Ok"),
                    QObject::tr("Suppression effectuée.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
        //ui->tableView->setModel(E.afficher()); // Refresh the table view after deletion
        ui->tab1->setModel(E.afficher());


    }
    else {
        QMessageBox::critical(nullptr, QObject::tr("Not ok"),
                    QObject::tr("Suppression non effectuée.\n"
                                "Click cancel to exit."), QMessageBox::Cancel);
    }
}

void EquipMain::on_modif_clicked()
{
    int id = ui->id->text().toUInt();
    QString nom = ui->nom->text();
    QString modele = ui->modele->text();
    QString etat = ui->etat->currentText();
    QString date = ui->date->date().toString("yyyy-MM-dd");
    QString emplacement = ui->Emplacement->currentText();

     // Get image as QByteArray

    QString imageName = ui->labelimage->text();
    if (!imageName.isEmpty()) {
        qDebug() << "Selected image name is: " << imageName;
    } else {
        qDebug() << "No image was selected or copy failed.";
    }
    // Validate input fields
    if (nom.isEmpty() || modele.isEmpty() || etat.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled out.");
        return;
    }

    Equipement E(id, nom, modele, etat, QDate::fromString(date, "yyyy-MM-dd"), emplacement, imageName);

    bool test = E.modifier(id, nom, modele, etat, QDate::fromString(date, "yyyy-MM-dd"), emplacement,imageName);
    if(test) {
        QMessageBox::information(nullptr, QObject::tr("Ok"),
                    QObject::tr("Modification effectuée.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
        //ui->tableView->setModel(E.afficher()); // Refresh the table view after modification
        ui->tab1->setModel(E.afficher());
        ui->id->clear();
        ui->nom->clear();
        ui->modele->clear();
    }
    else {
        QMessageBox::critical(nullptr, QObject::tr("Not ok"),
                    QObject::tr("Modification non effectuée.\n"
                                "Click cancel to exit."), QMessageBox::Cancel);
    }
}



void EquipMain::on_pushButton_4_clicked()
{
    onBrowseButtonClicked();
}

QString EquipMain::onBrowseButtonClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Select an Image",
                                                    "",
                                                    "Images (*.png *.jpg *.jpeg *.bmp *.gif)");
    if (filePath.isEmpty())
        return "";

    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();
    QString destinationPath = "C:/Users/MSI/Desktop/maintenance/Gestion equipements" + fileName;

    QDir dir("C:/Users/MSI/Desktop/maintenance/Gestion equipements");
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    if (QFile::exists(destinationPath)) {
        QFile::remove(destinationPath);
    }
    if (QFile::copy(filePath, destinationPath)) {
        qDebug() << "Image copied to: " << destinationPath;
        ui->labelimage->setText(destinationPath); // Save full path here
        return destinationPath; // Return the full path
    } else {
        QMessageBox::warning(this, "Error", "Failed to copy the image.");
        return "";
    }
}



void EquipMain::on_rech_textChanged(const QString &arg1)
{
    QString selectedOption = ui->rechercher->currentText(); // Get the selected search option

    if (arg1.isEmpty() && ui->rech->hasFocus() && !ui->rech->hasSelectedText()) {
        ui->tab1->setModel(E.afficher());
        ui->rech->setToolTip("Entrez un critère de recherche");
        return; // Exit early if there's no input
    }

    // Call the rechercher function with the selected option and input value
    ui->tab1->setModel(E.rechercher(selectedOption, arg1));
    ui->tab1->clearSelection();
}

void EquipMain::on_tri_currentTextChanged(const QString &arg1)
{
    // Create an instance of your produit class or access an existing one


    // Call the tri function with the selected sort column
    QSqlQueryModel *sortedModel = E.tri(arg1);

    // Assuming you have a QTableView to display the data
    ui->tab1->setModel(sortedModel);
}

void EquipMain::on_pushButton_6_clicked()
{
    QLayoutItem* item;
    while ((item = ui->stats_3->layout()->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Check the selected option from combo_3 and set up the appropriate chart

    QString selectedOption = ui->stat_cambo->currentText();
    QChartView *chartView = nullptr;

    if (selectedOption == "Emplacement") {
        // Create the pie chart based on 'etat'
        chartView = E.createPieChartModel();
    } else if (selectedOption == "etat") {
        // Create the pie chart based on 'poids'
        chartView = E.etat1();
    }

    // Add the chart view to the layout if it was successfully created
    if (chartView != nullptr) {
        ui->stats_3->layout()->addWidget(chartView);
    }
}

void EquipMain::on_PDF_clicked()
{
E.exportDataToPDF();

}
void EquipMain::on_ouvrirButton_clicked()
{

    // Get the vehicle ID from the line edit
    QString idVehicules = ui->IDE_VEHICULES->text();

    if (idVehicules.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a vehicle ID.");
        return;
    }

    QSqlQuery query;

    // Step 1: Check if the NUMS exists in the VEHICULES table
    query.prepare("SELECT NUMS FROM VEHICULES WHERE NUMS = :nums");
    query.bindValue(":nums", idVehicules);

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "Error", "Vehicle ID not found in VEHICULES table.");
        return;
    }

    // Step 2: Check if the same NUMS exists in the RDV table for an IDR
    query.prepare("SELECT IDR, JOUR, MOIS, ANNEE FROM RDV WHERE IDR = :idr");
    query.bindValue(":idr", idVehicules); // Assuming IDR is linked to NUMS
    if (!query.exec() || !query.next()) {
        QMessageBox::information(this, "Info", "No scheduled rendezvous (RDV) for this vehicle.");
        return;
    }

    // Fetch RDV date
    int jour = query.value("JOUR").toInt();
    int mois = query.value("MOIS").toInt();
    int annee = query.value("ANNEE").toInt();
    QDate rdvDate(annee, mois, jour);

    // Compare with system date
    QDate currentDate = QDate::currentDate();
    if (rdvDate == currentDate) {
        QMessageBox::information(this, "Info", "Rendezvous scheduled for today.");

        // Step 3: Send signal to Arduino to operate the motor
        QByteArray signal = "2";
        A.write_to_arduino(signal);
    } else {
        QMessageBox::information(this, "Info", "No rendezvous scheduled for today.");
    }
}
void EquipMain::initializeArduinoConnection() {
    int connectionStatus = A.connect_arduino();

    switch (connectionStatus) {
    case 0:
        QMessageBox::information(this, "Arduino Connection", "Arduino connected successfully!");
        break;
    case 1:
        QMessageBox::warning(this, "Arduino Connection", "Failed to configure Arduino connection.");
        break;
    case -1:
        QMessageBox::warning(this, "Arduino Connection", "Arduino not found. Please check the connection.");
        break;
    default:
        QMessageBox::critical(this, "Arduino Connection", "Unexpected error during Arduino connection.");
        break;
    }
}
