#ifndef FORGOTPASSWORDDIALOG_H
#define FORGOTPASSWORDDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QMessageBox>
#include "smshandler.h"  // Include SmsHandler

namespace Ui {
class ForgotPasswordDialog;
}

class ForgotPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ForgotPasswordDialog(QWidget *parent = nullptr);
    ~ForgotPasswordDialog();

private slots:
    void on_pushButton_resetPassword_clicked();

private:
    Ui::ForgotPasswordDialog *ui;

    bool verifyForgotPassword(const QString &id, const QString &salary);  // Verify ID and salary
    void resetPassword(const QString &id, const QString &newPassword);  // Reset the password
    bool sendSmsNotification(const QString &phoneNumber, const QString &message);  // Send SMS
    QString getPhoneNumberForEmployee(const QString &id);  // Get phone number for employee
};

#endif // FORGOTPASSWORDDIALOG_H
