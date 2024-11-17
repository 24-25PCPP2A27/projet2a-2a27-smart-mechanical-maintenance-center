#include "EmployeMain.h"
#include "ui_EmployeMain.h"
#include "employe.h"
#include "sendsmsdialog.h"
#include <QMessageBox>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include <QFileDialog>
#include <QSqlQueryModel>
#include <QTableView>
#include <QPixmap>
#include <QDebug>
#include <QSqlError>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Initialize tray icon
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/resources/icon.png")); // Replace with your app's icon
    trayIcon->show();


    // Add context menu to tray icon
    QMenu *trayMenu = new QMenu(this);
    trayMenu->addAction("Ouvrir l'application", this, &MainWindow::show);
    trayMenu->addAction("Quitter", this, &MainWindow::close);
    trayIcon->setContextMenu(trayMenu);
    refreshData();
    // Connect signals to slots
    connect(ui->lineEdit_search, &QLineEdit::textChanged, this, &MainWindow::searchEmployees);
    connect(ui->pushButton_refresh, &QPushButton::clicked, this, &MainWindow::refreshData);
    connect(ui->pushButton_sort, &QPushButton::clicked, this, &MainWindow::sortDataBySalaire);
    connect(ui->pushButton_statistics, &QPushButton::clicked, this, &MainWindow::displayDureeStatistics);
    connect(ui->pushButton_export_pdf, &QPushButton::clicked, this, &MainWindow::exportDataToPDF);
    connect(ui->pushButton_toggleTheme, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    connect(ui->pushButton_sendSms, &QPushButton::clicked, this, &MainWindow::openSendSmsDialog);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_ajouter_clicked()
{
    int id = ui->lineEdit_ID->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString poste = ui->lineEdit_poste->text();
    int salaire = ui->lineEdit_salaire->text().toInt();
    int duree = ui->lineEdit_date->text().toInt();
    QString mdp = ui->lineEdit_mdp->text();

    employe e(id, nom, prenom, poste, salaire, duree, mdp);

    if (!e.validateInputs()) {
        QMessageBox::critical(this, QObject::tr("Erreur de validation"),
                              QObject::tr("Les données saisies ne sont pas valides."));
        return;
    }

    bool test = e.ajouter();
    if (test) {
        ui->tableView->setModel(e.afficher());
        QMessageBox::information(this, QObject::tr("Succès"), QObject::tr("Ajout effectué."));

        trayIcon->showMessage("Succès",
                              "Employé ajouté avec succès.",
                              QSystemTrayIcon::Information,
                              3000);
    } else {
        QMessageBox::critical(this, QObject::tr("Erreur"), QObject::tr("Échec de l'ajout."));
    }
}

void MainWindow::on_pushButton_supprimer_clicked()
{
    int id = ui->lineEdit_ID->text().toInt();
    bool test = etmp.supprimer(id);

    if (test) {
        ui->tableView->setModel(etmp.afficher());
        QMessageBox::information(this, QObject::tr("Succès"), QObject::tr("Suppression effectuée."));

        trayIcon->showMessage("Succès",
                              "Employé supprimé avec succès.",
                              QSystemTrayIcon::Information,
                              3000);
    } else {
        QMessageBox::critical(this, QObject::tr("Erreur"), QObject::tr("Échec de la suppression."));
    }
}
void MainWindow::on_pushButton_update_clicked()
{
    int id = ui->lineEdit_ID->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString poste = ui->lineEdit_poste->text();
    int salaire = ui->lineEdit_salaire->text().toInt();
    int duree = ui->lineEdit_date->text().toInt();
    QString mdp = ui->lineEdit_mdp->text();

    employe e(id, nom, prenom, poste, salaire, duree, mdp);

    if (!e.validateInputs()) {
        QMessageBox::critical(this, QObject::tr("Erreur de validation"),
                              QObject::tr("Les données saisies ne sont pas valides."));
        return;
    }

    bool test = etmp.update(id, nom, prenom, poste, salaire, duree, mdp);

    if (test) {
        ui->tableView->setModel(etmp.afficher());
        QMessageBox::information(this, QObject::tr("Succès"), QObject::tr("Mise à jour effectuée."));

        trayIcon->showMessage("Succès",
                              "Employé mis à jour avec succès.",
                              QSystemTrayIcon::Information,
                              3000);
    } else {
        QMessageBox::critical(this, QObject::tr("Erreur"), QObject::tr("Échec de la mise à jour."));
    }
}
// Refresh data method
void MainWindow::refreshData()
{
    ui->tableView->setModel(etmp.afficher());
    ui->lineEdit_ID->clear();
    ui->lineEdit_nom->clear();
    ui->lineEdit_prenom->clear();
    ui->lineEdit_poste->clear();
    ui->lineEdit_salaire->clear();
    ui->lineEdit_date->clear();
    ui->lineEdit_mdp->clear();
}

// Slot to sort data by SALAIRE
void MainWindow::sortDataBySalaire()
{
    ui->tableView->setModel(etmp.sortBySalaire());
    trayIcon->showMessage("Information",
                          "Tri des employés par salaire terminé.",
                          QSystemTrayIcon::Information,
                          3000);
}
// Slot to display statistics for DUREE

void MainWindow::displayDureeStatistics() {
    // Query to get all "duree" values from the EMPLOYEE table
    QSqlQuery query;
    query.prepare("SELECT duree FROM EMPLOYEE");

    if (!query.exec()) {
        QMessageBox::critical(this, QObject::tr("Erreur"),
                              QObject::tr("Échec de la requête : ") + query.lastError().text());
        return;
    }

    // Map to store the count of each distinct "duree" value
    QMap<int, int> dureeCounts;

    // Read all duree values and count their occurrences
    while (query.next()) {
        int duree = query.value(0).toInt();
        dureeCounts[duree]++;
    }

    // Check if we have any data
    if (dureeCounts.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur"),
                             QObject::tr("Aucune donnée disponible pour afficher les statistiques."));
        return;
    }

    // Create the pie chart series
    QtCharts::QPieSeries *series = new QtCharts::QPieSeries();

    // Add slices to the pie chart, each representing a distinct "duree" value
    for (auto it = dureeCounts.begin(); it != dureeCounts.end(); ++it) {
        int dureeValue = it.key();
        int count = it.value();
        QString label = QString("%1 ans (%2)").arg(dureeValue).arg(count); // Label with value and count
        series->append(label, count);
    }

    // Optional: Customize slices (e.g., setting colors or labels)
    for (QtCharts::QPieSlice *slice : series->slices()) {
        slice->setLabelVisible();
        slice->setPen(QPen(Qt::black)); // Add border to each slice
        slice->setBrush(QColor::fromRgb(rand() % 255, rand() % 255, rand() % 255)); // Random color for each slice
    }

    // Create the chart and add the series to it
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques de la durée (Par Employé)");

    // Display the chart in a chart view
    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400, 300);
    chartView->setWindowTitle("Durée Statistiques");
    chartView->show();
}

// Slot to export data to PDF
void MainWindow::exportDataToPDF()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer en tant que PDF", "", "*.pdf");
    if (!filePath.isEmpty()) {
        if (etmp.exportToPDF(filePath)) {
            trayIcon->showMessage("Succès",
                                  "Données exportées avec succès en PDF.",
                                  QSystemTrayIcon::Information,
                                  3000);
        } else {
            QMessageBox::critical(this, QObject::tr("Erreur"), QObject::tr("Échec de l'exportation en PDF."));
        }
    }
}

void MainWindow::searchEmployees(const QString &query) {
    ui->tableView->setModel(etmp.search(query));
}
void MainWindow::on_pushButton_openQRCodeDialog_clicked()
{
    QRCodeDialog dialog(this);  // Create an instance of the dialog
    dialog.setWindowTitle("Generate Employee QR Code");
    dialog.exec();  // Show the dialog
}
void MainWindow::toggleTheme() {
    if (isDarkMode) {
        // Switch to light mode
        QApplication::setPalette(QApplication::style()->standardPalette());
        isDarkMode = false;
        QMessageBox::information(this, "Mode Clair", "Le mode clair est activé.");
    } else {
        // Switch to dark mode
        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(42, 42, 42));
        darkPalette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::Highlight, QColor(142, 45, 197).lighter());
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);

        QApplication::setPalette(darkPalette);
        isDarkMode = true;
        QMessageBox::information(this, "Mode Sombre", "Le mode sombre est activé.");
    }
}
void MainWindow::openSendSmsDialog() {
    SendSmsDialog dialog(this);
    dialog.setWindowTitle("Send SMS to Employee");
    dialog.exec();  // Show the dialog
}
QString MainWindow::getPhoneNumberForEmployee(const QString &id)
{
    QSqlQuery query;
    query.prepare("SELECT PHONE FROM EMPLOYEE WHERE ID = :ID");
    query.bindValue(":ID", id);

    if (!query.exec()) {
        qDebug() << "Query execution failed: " << query.lastError().text();
        return "";
    }

    if (query.next()) {
        return query.value("PHONE").toString();  // Return the phone number from the query result
    }

    return "";  // Return empty string if phone number not found
}

