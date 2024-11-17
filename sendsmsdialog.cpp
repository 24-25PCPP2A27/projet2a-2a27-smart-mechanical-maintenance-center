#include "sendsmsdialog.h"
#include "ui_sendsmsdialog.h"
#include "smshandler.h"  // Assuming SmsHandler is a separate class to handle SMS logic
#include "employe.h"     // Include the employee class to access its methods
#include <QMessageBox>

SendSmsDialog::SendSmsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendSmsDialog)
{
    ui->setupUi(this);
}

SendSmsDialog::~SendSmsDialog()
{
    delete ui;
}

void SendSmsDialog::on_pushButton_sendSms_clicked()
{
    bool ok;
    int employeeId = ui->lineEdit_employeeId->text().toInt(&ok);  // Convert the text to int

    QString message = ui->textEdit_message->toPlainText();

    if (!ok || message.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please provide a valid Employee ID and a message.");
        return;
    }

    // Create an employe object to retrieve the phone number
    employe emp;

    // Get the employee's phone number using the ID
    QString phoneNumber = emp.getPhoneNumberForEmployee(employeeId);

    if (phoneNumber.isEmpty()) {
        QMessageBox::warning(this, "Error", "Employee not found or phone number not available.");
        return;
    }

    // Send the SMS
    SmsHandler smsHandler;
    bool smsSent = smsHandler.sendSmsNotification(phoneNumber, message);

    if (smsSent) {
        QMessageBox::information(this, "Success", "SMS sent successfully!");
        accept();  // Close the dialog after sending the SMS
    } else {
        QMessageBox::critical(this, "Error", "Failed to send SMS.");
    }
}
