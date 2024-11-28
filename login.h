#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "arduino.h"

namespace Ui {
class login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    void handleRFIDInput();

private slots:
    void on_pushButton_login_clicked();  // Slot for login button
    void on_pushButton_forgotPassword_clicked();

private:
    Ui::login *ui;
    Arduino *arduino;
    void writeLog(const QString &message, const QString &category = "general");
};

#endif // LOGIN_H
