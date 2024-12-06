#ifndef MainRDV_H
#define MainRDV_H

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
#include "arduino.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainRDV; }
QT_END_NAMESPACE

class MainRDV : public QMainWindow {
    Q_OBJECT

public:
    explicit MainRDV(QWidget *parent = nullptr);
    ~MainRDV() override = default; // Explicit default destructor

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



     void on_statsButton_3_clicked();

     void on_statsButton_2_clicked();

private:
    QMap<QDate, QStringList> appointments; // Map to store appointments by date
    Ui::MainRDV *ui;
    QSqlTableModel *appointmentsModel = nullptr;
    QNetworkAccessManager *networkManager;

    // Widget declaration
    QCalendarWidget *calendarWidget; // Declare as a member of MainRDV

    void sendEmail(const QString &to, const QString &subject, const QString &body);
    Arduino A;
           QSerialPort* serialPort;
           QByteArray data;
           QString uid;
           void update_label();
};

#endif // MainRDV_H
