#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSqlTableModel>
#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "RDV.h"
#include <QDebug>

// Calendar
#include <QMap>
#include <QDate>
#include <QStringList>
#include <QCalendarWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default; // Explicit default destructor

private slots:
    // Slot for adding a new appointment
    void addAppointment();

    // Slot for updating an existing appointment
    void updateAppointment();

    // Slot for deleting an appointment
    void deleteAppointment();

    // Slot for loading appointments into the table
    void loadAppointments();

    // Slot for exporting the appointments table to a PDF file
    void exportToPDF();

    void generateCountryStatistics();

    void sortAppointmentsByID();

    void searchAppointments(const QString &searchQuery);

    void showAppointmentsOnCalendar();

     void onCalendarDateClicked(const QDate &date);

private:
    QMap<QDate, QStringList> appointments; // Map to store appointments by date
    Ui::MainWindow *ui;
    QSqlTableModel *appointmentsModel = nullptr;
    QNetworkAccessManager *networkManager;

    // Widget declaration
    QCalendarWidget *calendarWidget; // Declare as a member of MainWindow

    void sendEmail(const QString &to, const QString &subject, const QString &body);
};

#endif // MAINWINDOW_H
