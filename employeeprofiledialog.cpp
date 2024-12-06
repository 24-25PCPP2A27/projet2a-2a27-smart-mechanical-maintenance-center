#include "employeeprofiledialog.h"
#include "ui_employeeprofiledialog.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

EmployeeProfileDialog::EmployeeProfileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmployeeProfileDialog)
{
    ui->setupUi(this);
}

EmployeeProfileDialog::~EmployeeProfileDialog()
{
    delete ui;
}

void EmployeeProfileDialog::setEmployeeDetails(const QString &nom, const QString &prenom, const QString &poste,
                                               int salaire, int duree, const QPixmap &profilePicture)
{
    // Set text for the employee details
    ui->labelName->setText("Name: " + nom + " " + prenom);
    ui->labelPoste->setText("Position: " + poste);
    ui->labelSalaire->setText("Salary: $" + QString::number(salaire));
    ui->labelDuree->setText("Date D'Embauche: " + QString::number(duree));

    // Scale the profile picture to fit inside the label (keep aspect ratio)
    QPixmap scaledPicture = profilePicture.scaled(180, 180, Qt::KeepAspectRatio);

    // Set the image on the label and center it
    ui->labelPicture->setPixmap(scaledPicture);
    ui->labelPicture->setAlignment(Qt::AlignCenter);  // Center the image within the label
}
void EmployeeProfileDialog::displayEmployeeLogs(const QString &RFID)
{
    QFile logFile("application.log");
    if (!logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Unable to open log file.";
        return;
    }

    QTextStream in(&logFile);
    ui->tableWidgetLogs->setRowCount(0);  // Clear any existing rows
    ui->tableWidgetLogs->setColumnCount(3);  // Set 3 columns for timestamp, category, and message
    ui->tableWidgetLogs->setHorizontalHeaderLabels({"Timestamp", "Category", "Message"}); // Set column headers
    QHeaderView *header = ui->tableWidgetLogs->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);

    // Resize rows to fit content
    ui->tableWidgetLogs->resizeRowsToContents();


    // Debug: Check RFID
    qDebug() << "Searching logs for RFID: " << RFID;

    bool logFound = false;
    int loginCount = 0;  // Counter to track how many times the user logged in

    while (!in.atEnd()) {
        QString line = in.readLine();
        qDebug() << "Log line: " << line;

        // Check if the line contains the RFID (and optionally a login)
        if (line.contains(RFID)) {
            // If this line contains the RFID, it's a match
            qDebug() << "Found matching log for RFID.";

            // Split the line into timestamp and message, set category to "login"
            QString timestamp = line.section(' ', 0, 1);  // Get the timestamp (first part)
            QString category = "login";  // Set category to "login"
            QString message = line.section(' ', 3, -1);   // Get the message (everything after the category)

            // Count the number of logins (based on RFID)
            loginCount++;

            int row = ui->tableWidgetLogs->rowCount();
            qDebug() << "Inserting row: " << row;
            ui->tableWidgetLogs->insertRow(row);

            // Set the text for each cell in the row
            ui->tableWidgetLogs->setItem(row, 0, new QTableWidgetItem(timestamp));
            ui->tableWidgetLogs->setItem(row, 1, new QTableWidgetItem(category));
            ui->tableWidgetLogs->setItem(row, 2, new QTableWidgetItem(message));

            logFound = true;
        } else {
            // Further debugging: print which lines are not matching
            qDebug() << "No match for RFID in this log line.";
        }
    }

    if (!logFound) {
        qDebug() << "No logs found for RFID: " << RFID;
    }

    // Display the login count in a label
    qDebug() << "User logged in " << loginCount << " times.";

    // Update label with the login count
    ui->labelLoginCount->setText(QString("Login count: %1").arg(loginCount));

    // Set the background color to red and text color to white for better visibility
    ui->labelLoginCount->setStyleSheet("QLabel { background-color: red; color: white; font-weight: bold; font-size: 16px; padding: 5px; }");

    logFile.close();
    ui->tableWidgetLogs->viewport()->update();  // Force update
}
