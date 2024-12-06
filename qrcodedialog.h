#ifndef QRCODEDIALOG_H
#define QRCODEDIALOG_H

#include <QDialog>
#include <QPixmap>
#include "qrcodegen.h"  // Include the QR code library

namespace Ui {
class QRCodeDialog;
}

class QRCodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QRCodeDialog(QWidget *parent = nullptr);
    ~QRCodeDialog();

private slots:
    void on_pushButton_generate_clicked();  // Slot for QR code generation

private:
    Ui::QRCodeDialog *ui;

    QPixmap generateQRCode(const QString &data);  // Helper to generate QR code
    QString getEmployeeDataByID(const QString &id);  // Fetch employee data by ID
};

#endif // QRCODEDIALOG_H
