#include "forgotpassworddialog.h"
#include "ui_forgotpassworddialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

ForgotPasswordDialog::ForgotPasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ForgotPasswordDialog)
{
    ui->setupUi(this);

    // Initially hide password fields and their labels
    ui->lineEdit_newPassword->setVisible(false);
    ui->lineEdit_retypePassword->setVisible(false);
    ui->label_newPassword->setVisible(false);
    ui->label_retypePassword->setVisible(false);
    ui->label_pass->setVisible(false);
    ui->pushButton_resetPassword->setEnabled(false);
    ui->pushButton_verify->setEnabled(true);

    // Connect the verify button
    connect(ui->pushButton_verify, &QPushButton::clicked, this, &ForgotPasswordDialog::on_pushButton_verify_clicked);


    // Connect the reset password button
    connect(ui->pushButton_resetPassword, SIGNAL(clicked()), this, SLOT(on_pushButton_resetPassword_clicked()));
}

ForgotPasswordDialog::~ForgotPasswordDialog()
{
    delete ui;
}

void ForgotPasswordDialog::on_pushButton_verify_clicked()
{
    QString id = ui->lineEdit_id->text();        // Get ID
    QString salary = ui->lineEdit_salary->text(); // Get Salary

    if (id.isEmpty() || salary.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill both ID and Salary fields.");
        return;
    }

    // Verify ID and salary in the database
    if (verifyForgotPassword(id, salary)) {
        QMessageBox::information(this, "Verification", "ID and Salary verified successfully!");

        // Make password fields and labels visible
        ui->lineEdit_newPassword->setVisible(true);
        ui->lineEdit_retypePassword->setVisible(true);
        ui->label_newPassword->setVisible(true);
        ui->label_retypePassword->setVisible(true);
        ui->label_pass->setVisible(true);
        ui->pushButton_resetPassword->setEnabled(true);

        // Disable ID and salary fields after successful verification
        ui->lineEdit_id->setEnabled(false);
        ui->lineEdit_salary->setEnabled(false);

        // Hide and disable the verify button
        ui->pushButton_verify->setVisible(false);  // Hide the button
        ui->pushButton_verify->setEnabled(false); // Disable the button to prevent clicks
        ui->label_verify->setVisible(false);       // Optionally, hide the label as well
    } else {
        QMessageBox::warning(this, "Error", "Invalid ID or Salary. Please try again.");
    }
}


void ForgotPasswordDialog::on_pushButton_resetPassword_clicked()
{
    QString newPassword = ui->lineEdit_newPassword->text();       // Get New Password
    QString retypePassword = ui->lineEdit_retypePassword->text(); // Get Retype Password
    QString id = ui->lineEdit_id->text();                         // ID remains available after verification

    // Validate passwords
    if (newPassword.isEmpty() || retypePassword.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill all password fields.");
        return;
    }

    if (newPassword != retypePassword) {
        QMessageBox::warning(this, "Error", "Passwords do not match. Please try again.");
        return;
    }

    if (newPassword.length() < 6) {
        QMessageBox::warning(this, "Error", "Password must be at least 6 characters long.");
        return;
    }

    // Reset password in the database
    resetPassword(id, newPassword);
    QMessageBox::information(this, "Success", "Password reset successfully!");

    accept();  // Close dialog
}

bool ForgotPasswordDialog::verifyForgotPassword(const QString &id, const QString &salary)
{
    QSqlQuery query;
    query.prepare("SELECT 1 FROM EMPLOYEE WHERE ID = :id AND SALAIRE = :salary");
    query.bindValue(":id", id);
    query.bindValue(":salary", salary);

    if (!query.exec()) {
        qDebug() << "Query execution failed: " << query.lastError().text();
        return false;
    }

    return query.next();  // Return true if the ID and salary match a record
}

void ForgotPasswordDialog::resetPassword(const QString &id, const QString &newPassword)
{
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYEE SET MDP = :newPassword WHERE ID = :id");
    query.bindValue(":newPassword", newPassword);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Password reset query failed: " << query.lastError().text();
        QMessageBox::critical(this, "Error", "Failed to reset password: " + query.lastError().text());
    }
}
