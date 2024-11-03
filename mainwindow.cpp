#include "MainWindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle("Gestionnaire de Rendez-vous");

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Form for entering appointment details
    QFormLayout *formLayout = new QFormLayout;
    idEdit = new QLineEdit;
    jourEdit = new QLineEdit;
    moisEdit = new QLineEdit;
    anneeEdit = new QLineEdit;
    heureEdit = new QLineEdit;
    adresseEdit = new QLineEdit;

    formLayout->addRow("ID:", idEdit);
    formLayout->addRow("Jour:", jourEdit);
    formLayout->addRow("Mois:", moisEdit);
    formLayout->addRow("Année:", anneeEdit);
    formLayout->addRow("Heure:", heureEdit);
    formLayout->addRow("Adresse:", adresseEdit);
    layout->addLayout(formLayout);

    // Buttons for adding, updating, and deleting appointments
    QPushButton *addButton = new QPushButton("Ajouter un Rendez-vous");
    QPushButton *updateButton = new QPushButton("Mettre à Jour le Rendez-vous");
    QPushButton *deleteButton = new QPushButton("Supprimer le Rendez-vous");

    layout->addWidget(addButton);
    layout->addWidget(updateButton);
    layout->addWidget(deleteButton);

    // Table to display appointments
    appointmentsTable = new QTableWidget(this);
    appointmentsTable->setColumnCount(6);
    appointmentsTable->setHorizontalHeaderLabels(QStringList() << "ID" << "Jour" << "Mois" << "Année" << "Heure" << "Adresse");
    layout->addWidget(appointmentsTable);

    // Connect signals and slots
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addAppointment);
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::updateAppointment);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteAppointment);

    loadAppointments();
}

// Destructor
MainWindow::~MainWindow() {}

// Method to add an appointment
void MainWindow::addAppointment() {
    rdv appointment(idEdit->text().toInt(),
                    jourEdit->text().toInt(),
                    moisEdit->text().toInt(),
                    anneeEdit->text().toInt(),
                    heureEdit->text().toInt(),
                    adresseEdit->text());

    if (appointment.ajouter()) {
        QMessageBox::information(this, "Succès", "Rendez-vous ajouté avec succès.");
        loadAppointments();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de l'ajout du rendez-vous.");
    }
}

// Method to update an appointment
void MainWindow::updateAppointment() {
    rdv appointment(idEdit->text().toInt(),
                    jourEdit->text().toInt(),
                    moisEdit->text().toInt(),
                    anneeEdit->text().toInt(),
                    heureEdit->text().toInt(),
                    adresseEdit->text());

    if (appointment.update(appointment.getIDR(),
                           appointment.getJOUR(),
                           appointment.getMOIS(),
                           appointment.getANNEE(),
                           appointment.getHEURE(),
                           appointment.getADRESSE())) {
        QMessageBox::information(this, "Succès", "Rendez-vous mis à jour avec succès.");
        loadAppointments();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la mise à jour du rendez-vous.");
    }
}

// Method to delete an appointment
void MainWindow::deleteAppointment() {
    bool ok;
    int id = idEdit->text().toInt(&ok);  // Convert ID input to integer
    if (ok) {
        rdv appointment(id, 0, 0, 0, 0, "");  // Create an rdv object with a valid ID
        if (appointment.supprimer(id)) {      // Call the supprimer method
            QMessageBox::information(this, "Succès", "Rendez-vous supprimé avec succès.");
            loadAppointments();                // Reload the appointments after deletion
        } else {
            QMessageBox::warning(this, "Erreur", "Échec de la suppression du rendez-vous.");
        }
    } else {
        QMessageBox::warning(this, "Erreur", "ID invalide.");  // Handle invalid ID input
    }
}

// Method to load appointments into the table
void MainWindow::loadAppointments() {
    // Clear existing table data
    appointmentsTable->setRowCount(0);

    // Load appointments from the database and populate the table
    QSqlQuery query("SELECT * FROM rdv");
    while (query.next()) {
        int row = appointmentsTable->rowCount();
        appointmentsTable->insertRow(row);
        for (int i = 0; i < 6; ++i) {
            appointmentsTable->setItem(row, i, new QTableWidgetItem(query.value(i).toString()));
        }
    }
}
