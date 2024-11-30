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
#include "employeeprofiledialog.h"
#include "EmployeMain.h"

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
    QString username = ui->lineEdit_username->text();  // Get username
    QString password = ui->lineEdit_password->text();  // Get password

    // Check for admin login (for now)
    if (username == "admin" && password == "admin") {
        QMessageBox::information(this, "Login", "Admin login successful!");

        // Show the EmployeMain window for admin (for now all users go here)
        EmployeMain *employeMain = new EmployeMain();
        employeMain->setWindowTitle("Employee Management");
        employeMain->show();  // Show the main window
        this->close();  // Close the login dialog
        return;
    }

    // Database query for username/password login
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYEE WHERE NOM = :username AND MDP = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        QMessageBox::information(this, "Login", "Login successful!");

        // Retrieve the role (POSTE) for future checks
        QString poste = query.value("POSTE").toString();

        // Commented Role-Based Navigation
        /*
        if (poste == "ResponsableRDV") {
            AdminMain *adminMain = new AdminMain();
            adminMain->setWindowTitle("Admin Panel");
            adminMain->show();
        } else if (poste == "ResponsableService") {
            TechnicianMain *technicianMain = new TechnicianMain();
            technicianMain->setWindowTitle("Technician Panel");
            technicianMain->show();
        } else {
            EmployeMain *employeMain = new EmployeMain();
            employeMain->setWindowTitle("Employee Panel");
            employeMain->show();
        }
        */

        // Temporary: All users go to EmployeMain
        EmployeMain *employeMain = new EmployeMain();
        employeMain->setWindowTitle("Employee Management");
        employeMain->show();

        this->close();  // Close the login dialog
    } else {
        QMessageBox::warning(this, "Login", "Incorrect username or password.");
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

    // Update label to show "Scanning card..."
    ui->labelStatus->setText("Scanning card...");

    if (rfidData.isEmpty()) {
        qDebug() << "No data received from Arduino.";
        return;
    }

    // Convert incoming data to QString and append it to the buffer
    buffer += QString(rfidData).trimmed();
    qDebug() << "Current buffer content:" << buffer;

    // Define the length of the UID (8 characters for MIFARE Classic)
    const int uidLength = 8;

    // Process the buffer in chunks of the UID length
    while (buffer.length() >= uidLength) {
        QString completeRFID = buffer.left(uidLength); // Extract the first UID
        buffer.remove(0, uidLength); // Remove the processed UID from the buffer

        // Ensure the UID is of the correct length
        if (completeRFID.length() == uidLength) {
            writeLog("RFID scanned: " + completeRFID, "RFID");
            qDebug() << "RFID data processed:" << completeRFID;

            // Query the database for employee details using the RFID
            QSqlQuery query;
            query.prepare("SELECT NOM, PRENOM, POSTE, SALAIRE, DUREE, IMAGE FROM EMPLOYEE WHERE RFID = :rfid");
            query.bindValue(":rfid", completeRFID);

            if (query.exec()) {
                if (query.next()) {
                    // Retrieve employee details from the database
                    QString nom = query.value("NOM").toString();
                    QString prenom = query.value("PRENOM").toString();
                    QString poste = query.value("POSTE").toString();
                    int salaire = query.value("SALAIRE").toInt();
                    int duree = query.value("DUREE").toInt();
                    QString imagePath = query.value("IMAGE").toString();

                    // Load the profile picture from the file path
                    QPixmap profilePicture;
                    if (!profilePicture.load(imagePath)) {
                        qDebug() << "Failed to load profile picture from path:" << imagePath;
                        QMessageBox::warning(this, "Image Error", "Failed to load profile picture for user: " + nom);
                    }

                    // Display employee profile in a custom dialog
                    EmployeeProfileDialog *profileDialog = new EmployeeProfileDialog(this);
                    profileDialog->setEmployeeDetails(nom, prenom, poste, salaire, duree, profilePicture);
                    profileDialog->displayEmployeeLogs(completeRFID);
                    profileDialog->exec();

                    writeLog("RFID login successful for user: " + nom + " " + prenom, "RFID");
                    ui->labelStatus->setText("Scan successful for: " + nom + " " + prenom);

                    // Commented Role-Based Navigation
                    /*
                    if (poste == "ResponsableService") {
                        AdminMain *adminMain = new AdminMain();
                        adminMain->setWindowTitle("Admin Panel");
                        adminMain->show();
                    } else if (poste == "ResponsableRDV") {
                        TechnicianMain *technicianMain = new TechnicianMain();
                        technicianMain->setWindowTitle("Technician Panel");
                        technicianMain->show();
                    } else if (poste == "ResponsableRDV") {
                        TechnicianMain *technicianMain = new TechnicianMain();
                        technicianMain->setWindowTitle("Technician Panel");
                        technicianMain->show();
                    } else if (poste == "ResponsableEmploye" {
                        EmployeMain *employeMain = new EmployeMain();
                        employeMain->setWindowTitle("Employee Panel");
                        employeMain->show();
                    }
                    */

                    // Temporary: All users go to EmployeMain
                    EmployeMain *employeMain = new EmployeMain();
                    employeMain->setWindowTitle("Employee Management");
                    employeMain->show();

                    this->close();  // Close the login dialog
                    return; // Exit the function after handling
                } else {
                    QMessageBox::warning(this, "Login", "Unknown RFID tag.");
                    writeLog("Unknown RFID scanned: " + completeRFID, "RFID");
                    ui->labelStatus->setText("Unknown RFID tag.");
                }
            } else {
                qDebug() << "Database query failed: " << query.lastError().text();
                QMessageBox::critical(this, "Error", "Failed to execute query.");
                writeLog("Database query error: " + query.lastError().text(), "RFID");
                ui->labelStatus->setText("Error during scan.");
            }
        }
    }

    // Reset the label to default if the buffer is empty
    if (buffer.isEmpty()) {
        ui->labelStatus->setText("Please scan your card...");
    }
}
