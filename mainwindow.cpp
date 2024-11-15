#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employe.h"
#include <QMessageBox>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QFileDialog>
#include <QSqlQueryModel>
#include <QTableView>
#include <QPixmap>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    refreshData();
    // Connect signals to slots
    connect(ui->lineEdit_search, &QLineEdit::textChanged, this, &MainWindow::searchEmployees);
    connect(ui->pushButton_refresh, &QPushButton::clicked, this, &MainWindow::refreshData);
    connect(ui->pushButton_sort, &QPushButton::clicked, this, &MainWindow::sortDataBySalaire);
    connect(ui->pushButton_statistics, &QPushButton::clicked, this, &MainWindow::displayDureeStatistics);
    connect(ui->pushButton_export_pdf, &QPushButton::clicked, this, &MainWindow::exportDataToPDF);
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
// Refresh data method
void MainWindow::refreshData() {
    // Create an instance of the employe class
    employe emp(0, "", "", "", 0, 0);

    // Fetch the latest data and update the table view
    ui->tableView->setModel(emp.afficher());

    // Optionally clear input fields after refresh
    ui->lineEdit_ID->clear();
    ui->lineEdit_nom->clear();
    ui->lineEdit_prenom->clear();
    ui->lineEdit_poste->clear();
    ui->lineEdit_salaire->clear();
    ui->lineEdit_date->clear();
}
// Slot to sort data by SALAIRE
void MainWindow::sortDataBySalaire() {
    ui->tableView->setModel(etmp.sortBySalaire());
    QMessageBox::information(this, QObject::tr("Tri"),
                             QObject::tr("Les employés ont été triés par salaire."));
}

// Slot to display statistics for DUREE
void MainWindow::displayDureeStatistics() {
    QMap<QString, QVariant> stats = etmp.getDureeStatistics();

    if (stats.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur"),
                             QObject::tr("Les statistiques sont vides. Vérifiez les données."));
        return;
    }

    QtCharts::QChart *chart = new QtCharts::QChart();
    QtCharts::QBarSet *set = new QtCharts::QBarSet("Duree Statistics");

    *set << stats["Min"].toInt() << stats["Average"].toInt() << stats["Max"].toInt();

    QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
    series->append(set);
    chart->addSeries(series);

    QStringList categories;
    categories << "Min" << "Average" << "Max";
    QtCharts::QBarCategoryAxis *axisX = new QtCharts::QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    int minDuration = stats["Min"].toInt();
    int maxDuration = stats["Max"].toInt();

    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
    axisY->setRange(minDuration - 1, maxDuration + 10);
    axisY->setTitleText("Duration (Years)");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400, 300);
    chartView->setWindowTitle("Statistiques de la durée");
    chartView->show();
}

// Slot to export data to PDF
void MainWindow::exportDataToPDF() {
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer en tant que PDF", "", "*.pdf");
    if (!filePath.isEmpty()) {
        if (etmp.exportToPDF(filePath)) {
            QMessageBox::information(this, QObject::tr("Export PDF"),
                                     QObject::tr("Données exportées avec succès en PDF."));
        } else {
            QMessageBox::critical(this, QObject::tr("Erreur d'export PDF"),
                                  QObject::tr("Échec de l'exportation des données en PDF."));
        }
    }
}

void MainWindow::searchEmployees(const QString &query) {
    ui->tableView->setModel(etmp.search(query));
}

