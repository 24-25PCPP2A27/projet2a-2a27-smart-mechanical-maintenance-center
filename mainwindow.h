#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_ajouter_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_update_clicked();
    void refreshData();
    void sortDataBySalaire();
    void displayDureeStatistics();
    void exportDataToPDF();
    void searchEmployees(const QString &query);

private:
    Ui::MainWindow *ui;
    employe etmp;  // Object of the 'employe' class for employee operations
};

#endif // MAINWINDOW_H
