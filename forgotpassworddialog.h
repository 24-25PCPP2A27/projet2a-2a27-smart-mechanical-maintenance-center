#ifndef FORGOTPASSWORDDIALOG_H
#define FORGOTPASSWORDDIALOG_H

#include <QDialog>

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
    void on_pushButton_verify_clicked();        // Slot for verify button
    void on_pushButton_resetPassword_clicked(); // Slot for reset password button

private:
    Ui::ForgotPasswordDialog *ui;

    // Helper methods
    bool verifyForgotPassword(const QString &id, const QString &salary); // Verify ID and salary
    void resetPassword(const QString &id, const QString &newPassword);   // Reset password
};

#endif // FORGOTPASSWORDDIALOG_H
