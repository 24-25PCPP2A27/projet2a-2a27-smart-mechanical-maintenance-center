#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFormLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::on_addButton_clicked()
{
    // Récupérer les données des champs de texte
    int ids = ui->idLineEdit->text().toInt();
    QString type = ui->typeLineEdit->text();
    QString duree = ui->dureeLineEdit->text();
    QString equipment = ui->equipmentLineEdit->text();
    QString etats = ui->etatsLineEdit->text();
    QString cout = ui->coutLineEdit->text();

    // Créer un nouvel objet Service
    Service newService(ids, type, duree, equipment, etats, cout);

    // Ajouter le service dans la base de données
    if (newService.ajouter()) {
        QMessageBox::information(this, "Ajout", "Service ajouté avec succès.");
    } else {
        QMessageBox::warning(this, "Erreur", "Erreur lors de l'ajout du service.");
    }
}*/

void MainWindow::on_deleteButton_clicked()
{
    // Récupérer l'ID du service à supprimer
    int id = ui->idLineEdit->text().toInt();

    // Supprimer le service par ID
    Service service;  // Créer un objet Service pour utiliser la méthode supprimer
    if (service.supprimer(id)) {
        QMessageBox::information(this, "Suppression", "Service supprimé avec succès.");
    } else {
        QMessageBox::warning(this, "Erreur", "Erreur lors de la suppression du service.");
    }
}

void MainWindow::on_updateButton_clicked()
{
    // Récupérer les données des champs de texte
    int id = ui->idLineEdit->text().toInt();
    QString type = ui->typeLineEdit->text();
    QString duree = ui->dureeLineEdit->text();
    QString equipment = ui->equipmentLineEdit->text();
    QString etats = ui->etatsLineEdit->text();
    QString cout = ui->coutLineEdit->text();

    // Créer un objet Service pour utiliser la méthode update
    Service service;  // Créer un objet Service pour mettre à jour
    if (service.update(id, type, duree, equipment, etats, cout)) {
        QMessageBox::information(this, "Mise à jour", "Service mis à jour avec succès.");
    } else {
        QMessageBox::warning(this, "Erreur", "Erreur lors de la mise à jour du service.");
    }
}

void MainWindow::on_showButton_clicked()
{

    // Cela pourrait être amélioré pour afficher des données dans une table par exemple
    QMessageBox::information(this, "Affichage", "Afficher");
}

void MainWindow::on_pushButton_clicked()
{
    // Récupérer les données des champs de texte
    int ids = ui->idLineEdit->text().toInt();
    QString type = ui->typeLineEdit->text();
    QString duree = ui->dureeLineEdit->text();
    QString equipment = ui->equipmentLineEdit->text();
    QString etats = ui->etatsLineEdit->text();
    QString cout = ui->coutLineEdit->text();

    // Créer un nouvel objet Service
    Service newService(ids, type, duree, equipment, etats, cout);

    // Ajouter le service dans la base de données
    if (newService.Ajouter()) {
        QMessageBox::information(this, "Ajout", "Service ajouté avec succès.");
    } else {
        QMessageBox::warning(this, "Erreur", "Erreur lors de l'ajout du service.");
    }
}
