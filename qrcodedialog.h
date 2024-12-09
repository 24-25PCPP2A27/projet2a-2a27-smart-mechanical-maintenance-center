#ifndef QRCODEDIALOG_H
#define QRCODEDIALOG_H

#include <QDialog>
#include <QImage>
#include <QPixmap>
#include "vehicule.h"

namespace Ui {
class QRCodeDialog;
}

class QRCodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QRCodeDialog(QWidget *parent = nullptr);
    ~QRCodeDialog();

    void generateAndDisplayQRCode();

private slots:
    void on_generateButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::QRCodeDialog *ui;
    QImage generateQRCode(const QString &text);
    QString getmarqueBynums(int nums);
};

#endif // QRCODEDIALOG_H
