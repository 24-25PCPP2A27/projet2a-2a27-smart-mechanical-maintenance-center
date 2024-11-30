#ifndef EMPLOYE_MAIN_H
#define EMPLOYE_MAIN_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include "employe.h"
#include "qrcodedialog.h"
#include <QSystemTrayIcon>
#include "login.h"
#include "ui_EmployeMain.h"


namespace Ui {
class EmployeMain;
}
class EmployeMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit EmployeMain(QWidget *parent = nullptr);
    ~EmployeMain();

private slots:
    void on_pushButton_ajouter_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_update_clicked();
    void refreshData();
    void sortDataBySalaire();
    void exportDataToPDF();
    void searchEmployees(const QString &query);
    void on_pushButton_openQRCodeDialog_clicked();
    void toggleTheme();
    void openLogViewer();


private:
    Ui::EmployeMain *ui;
    employe etmp;  // Object of the 'employe' class for employee operations
    bool isDarkMode = false;
    QSystemTrayIcon *trayIcon;
    Login *loginWindow;
    QTimer *refreshTimer;
    void refreshDureeStatistics();


};

#endif // MAINWINDOW_H
