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
}

ForgotPasswordDialog::~ForgotPasswordDialog()
{
    delete ui;
}

void ForgotPasswordDialog::on_pushButton_resetPassword_clicked()
{
    QString id = ui->lineEdit_id->text();  // Get ID from input
    QString salary = ui->lineEdit_salary->text();  // Get salary from input
    QString newPassword = ui->lineEdit_newPassword->text();  // Get new password

    if (id.isEmpty() || salary.isEmpty() || newPassword.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill all fields.");
        return;
    }

    // Check if ID and salary are valid in the database
    if (verifyForgotPassword(id, salary)) {
        if (newPassword.length() < 6) {
            QMessageBox::warning(this, "Error", "Password must be at least 6 characters long.");
            return;
        }

        // Reset password
        resetPassword(id, newPassword);
        QMessageBox::information(this, "Success", "Password reset successfully.");

        accept();  // Close the dialog
    } else {
        QMessageBox::warning(this, "Error", "Invalid ID or salary.");
    }
}

// Verify the ID and salary in the database
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

// Reset the password in the database
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
