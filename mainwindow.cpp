#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "employe.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Display the initial data
    ui->tableView->setModel(etmp.afficher());

    // Connect the refresh button to a new refresh slot
    connect(ui->pushButton_refresh, &QPushButton::clicked, this, &MainWindow::refreshData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_ajouter_clicked()
{
    // Retrieve the data
    int id = ui->lineEdit_ID->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString poste = ui->lineEdit_poste->text();
    int salaire = ui->lineEdit_salaire->text().toInt();
    int duree = ui->lineEdit_date->text().toInt(); // Assuming input is only the year (e.g., 2023)

    employe e(id, nom, prenom, poste, salaire, duree);

    // Validate inputs before adding
    if (!e.validateInputs()) {
        QMessageBox::critical(this, QObject::tr("Erreur de validation"),
                              QObject::tr("Les données saisies ne sont pas valides.\nVeuillez vérifier les champs et réessayer."));
        return;
    }

    bool test = e.ajouter();
    if (test) {
        ui->tableView->setModel(etmp.afficher());  // Refresh the table
        QMessageBox::information(nullptr, QObject::tr("ok"),
                                 QObject::tr("Ajout effectué\nClick to exit."));
    } else {
        QMessageBox::critical(nullptr, QObject::tr("not ok"),
                              QObject::tr("Ajout non effectué.\nClick cancel to exit."),
                              QMessageBox::Cancel);
    }
}

void MainWindow::on_pushButton_supprimer_clicked()
{
    int id = ui->lineEdit_ID->text().toInt();
    bool test = etmp.supprimer(id);
    if (test) {
        ui->tableView->setModel(etmp.afficher());  // Refresh the table
        QMessageBox::information(nullptr, QObject::tr("ok"),
                                 QObject::tr("Suppression effectuée\nClick to exit."));
    } else {
        QMessageBox::critical(nullptr, QObject::tr("not ok"),
                              QObject::tr("Suppression non effectuée.\nClick cancel to exit."),
                              QMessageBox::Cancel);
    }
}

void MainWindow::on_pushButton_update_clicked() {
    // Retrieve the data
    int id = ui->lineEdit_ID->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString poste = ui->lineEdit_poste->text();
    int salaire = ui->lineEdit_salaire->text().toInt();
    int duree = ui->lineEdit_date->text().toInt(); // Assuming input is only the year

    employe e(id, nom, prenom, poste, salaire, duree);

    // Validate inputs before updating
    if (!e.validateInputs()) {
        QMessageBox::critical(this, QObject::tr("Erreur de validation"),
                              QObject::tr("Les données saisies ne sont pas valides.\nVeuillez vérifier les champs et réessayer."));
        return;
    }

    bool test = etmp.update(id, nom, prenom, poste, salaire, duree);
    if (test) {
        ui->tableView->setModel(etmp.afficher());  // Refresh the table
        QMessageBox::information(nullptr, QObject::tr("ok"),
                                 QObject::tr("Mise à jour effectuée\nClick to exit."));
    } else {
        QMessageBox::critical(nullptr, QObject::tr("not ok"),
                              QObject::tr("Mise à jour non effectuée.\nClick cancel to exit."),
                              QMessageBox::Cancel);
    }
}

// Slot to handle the refresh button click
void MainWindow::refreshData() {
    ui->tableView->setModel(etmp.afficher());  // Refresh the data in the table view
    QMessageBox::information(nullptr, QObject::tr("Refresh"),
                             QObject::tr("Données rafraîchies avec succès."));
}
