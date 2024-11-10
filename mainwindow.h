#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTableWidget>
#include <QMessageBox>
#include <QComboBox>
#include "ui_mainwindow.h"
#include "RDV.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addAppointment();
    void updateAppointment();
    void deleteAppointment();
    void loadAppointments();

private:
    QLineEdit *idEdit;
    QLineEdit *jourEdit;
    QLineEdit *moisEdit;
    QLineEdit *anneeEdit;
    QLineEdit *heureEdit;
    QLineEdit *adresseEdit;
    QTableWidget *appointmentsTable;
};

#endif // MAINWINDOW_H
