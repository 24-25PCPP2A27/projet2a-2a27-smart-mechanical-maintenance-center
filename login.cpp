#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "forgotpassworddialog.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    connect(ui->pushButton_forgotPassword, SIGNAL(clicked()), this, SLOT(on_pushButton_forgotPassword_clicked()));

}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_username->text();  // Get username from the input
    QString password = ui->lineEdit_password->text();  // Get password from the input

    // Check if the username is "admin" and the password is the predefined admin password
    if (username == "admin" && password == "admin") {
        // If the admin credentials are correct, log in successfully
        QMessageBox::information(this, "Login", "Admin login successful!");
        accept();  // Close the login dialog and accept the login
        return;  // Skip database authentication
    }

    // Query the database to check if the NAME and MDP match
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYEE WHERE NOM = :username AND MDP = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec()) {
        if (query.next()) {
            // If the username and password match, proceed
            QMessageBox::information(this, "Login", "Login successful!");
            accept();  // Close the login dialog and accept the login
        } else {
            // If no matching user is found
            QMessageBox::warning(this, "Login", "Incorrect username or password.");
        }
    } else {
        qDebug() << "Database query failed: " << query.lastError().text();
        QMessageBox::critical(this, "Error", "Failed to execute query.");
    }
}
void Login::on_pushButton_forgotPassword_clicked()
{
    ForgotPasswordDialog forgotPasswordDialog(this);  // Create and show the ForgotPasswordDialog
    forgotPasswordDialog.exec();  // Show the dialog and wait for it to close
}
