#include "qrcodedialog.h"
#include "ui_qrcodedialog.h"

#include <QMessageBox>

QRCodeDialog::QRCodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QRCodeDialog)
{
    ui->setupUi(this);
}

QRCodeDialog::~QRCodeDialog()
{
    delete ui;
}

void QRCodeDialog::generateAndDisplayQRCode()
{
    bool ok;
    int nums = ui->idLineEdit->text().toInt(&ok);

    if (!ok || nums <= 0) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid vehicule number.");
        return;
    }

    QString marque = getmarqueBynums(nums);
    if (marque.isEmpty()) {
        QMessageBox::warning(this, "Not Found", "No vehicule number found with this ID.");
        return;
    }

    QImage qrCodeImage = generateQRCode(marque);

    ui->qrCodeLabel->setPixmap(QPixmap::fromImage(qrCodeImage));
}

void QRCodeDialog::on_generateButton_clicked()
{
    generateAndDisplayQRCode();
}

QString QRCodeDialog::getmarqueBynums(int nums)
{
    vehicule veh;
    QSqlQuery query;
    query.prepare("SELECT marque FROM vehicule WHERE nums = :nums");
    query.bindValue(":nums", nums);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }

    return QString();
}

QImage QRCodeDialog::generateQRCode(const QString &marqueName)
{
    QString googleImagesUrl = QString("https://www.google.com/search?tbm=isch&q=%1")
    .arg(marqueName);

    const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(googleImagesUrl.toUtf8().constData(), qrcodegen::QrCode::Ecc::LOW);

    const int size = qr.getSize();
    const int scale = 10;

    QImage image(size * scale, size * scale, QImage::Format_RGB888);
    image.fill(Qt::white);

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            QColor color = qr.getModule(x, y) ? Qt::black : Qt::white;
            for (int dy = 0; dy < scale; ++dy) {
                for (int dx = 0; dx < scale; ++dx) {
                    image.setPixel((x * scale) + dx, (y * scale) + dy, color.rgb());
                }
            }
        }
    }

    return image;
}



void QRCodeDialog::on_pushButton_clicked()
{
    close();
}

