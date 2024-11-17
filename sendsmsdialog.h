#ifndef SENDSMSDIALOG_H
#define SENDSMSDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class SendSmsDialog;
}

class SendSmsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SendSmsDialog(QWidget *parent = nullptr);
    ~SendSmsDialog();

private slots:
    void on_pushButton_sendSms_clicked();  // Slot for sending SMS

private:
    Ui::SendSmsDialog *ui;
};

#endif // SENDSMSDIALOG_H
