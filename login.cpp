#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "forgotpassworddialog.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "arduino.h"

// Constructor
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login),
    arduino(new Arduino) // Initialize Arduino instance
{
    ui->setupUi(this);

    // Connect the forgot password button
    connect(ui->pushButton_forgotPassword, SIGNAL(clicked()), this, SLOT(on_pushButton_forgotPassword_clicked()));

    // Attempt to connect to the Arduino
    int connectionResult = arduino->connect_arduino();
    if (connectionResult == 0) {
        qDebug() << "Arduino connected on port:" << arduino->getarduino_port_name();
        // Connect the serial port's readyRead signal to handle RFID input
        connect(arduino->getserial(), &QSerialPort::readyRead, this, &Login::handleRFIDInput);
    } else if (connectionResult == 1) {
        QMessageBox::warning(this, "Arduino Connection", "Failed to open Arduino port.");
        writeLog("Failed to open Arduino port.", "RFID");
    } else {
        QMessageBox::critical(this, "Arduino Connection", "Arduino not found.");
        writeLog("Arduino not found.", "RFID");
    }
}

// Destructor
Login::~Login()
{
    delete ui;
    if (arduino) {
        arduino->close_arduino(); // Close the Arduino connection
        delete arduino;
    }
}

// Login button clicked handler
void Login::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_username->text();  // Get username from the input
    QString password = ui->lineEdit_password->text();  // Get password from the input

    writeLog("Login attempt by user: " + username, "login");

    // Check for admin login
    if (username == "admin" && password == "admin") {
        QMessageBox::information(this, "Login", "Admin login successful!");
        writeLog("Admin login successful for user: " + username, "login");
        accept();
        return;
    }

    // Database query for username/password login
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYEE WHERE NOM = :username AND MDP = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec()) {
        if (query.next()) {
            QMessageBox::information(this, "Login", "Login successful!");
            writeLog("Login successful for user: " + username, "login");
            accept();
        } else {
            QMessageBox::warning(this, "Login", "Incorrect username or password.");
            writeLog("Failed login attempt for user: " + username, "login");
        }
    } else {
        qDebug() << "Database query failed: " << query.lastError().text();
        QMessageBox::critical(this, "Error", "Failed to execute query.");
    }
}

// Forgot password button clicked handler
void Login::on_pushButton_forgotPassword_clicked()
{
    ForgotPasswordDialog forgotPasswordDialog(this);
    forgotPasswordDialog.exec();
}

// Log messages to a file
void Login::writeLog(const QString &message, const QString &category)
{
    QFile logFile("application.log");
    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&logFile);
        out << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
            << " - [" << category << "] - " << message << "\n";
        logFile.close();
    } else {
        qDebug() << "Failed to open log file for writing.";
    }
}

// Handle RFID input
void Login::handleRFIDInput() {
    static QString buffer;  // Buffer to store incoming RFID data
    QByteArray rfidData = arduino->read_from_arduino(); // Read data from Arduino

    if (rfidData.isEmpty()) {
        qDebug() << "No data received from Arduino.";
        return;
    }

    // Convert incoming data to QString and append to the buffer
    buffer += QString(rfidData).trimmed();
    qDebug() << "Current buffer content:" << buffer;

    // Define the length of the UID (8 characters for MIFARE Classic)
    const int uidLength = 8; // Update if necessary for different card types

    // Process the buffer in chunks of the UID length
    while (buffer.length() >= uidLength) {
        QString completeRFID = buffer.left(uidLength); // Extract the first UID
        buffer.remove(0, uidLength); // Remove the processed UID from the buffer

        // Ensure we have a valid RFID (UID)
        if (completeRFID.length() == uidLength) {
            writeLog("RFID scanned: " + completeRFID, "RFID");
            qDebug() << "RFID data processed:" << completeRFID;

            // Query the database for the RFID
            QSqlQuery query;
            query.prepare("SELECT * FROM EMPLOYEE WHERE RFID = :rfid");
            query.bindValue(":rfid", completeRFID);

            if (query.exec()) {
                if (query.next()) {
                    QString username = query.value("NOM").toString();
                    QMessageBox::information(this, "Login", "RFID login successful for user: " + username);
                    writeLog("RFID login successful for user: " + username, "RFID");
                    accept();  // Close the login dialog
                } else {
                    QMessageBox::warning(this, "Login", "Unknown RFID tag.");
                    writeLog("Unknown RFID scanned: " + completeRFID, "RFID");
                }
            } else {
                qDebug() << "Database query failed: " << query.lastError().text();
                QMessageBox::critical(this, "Error", "Failed to execute query.");
            }
        }
    }
}
