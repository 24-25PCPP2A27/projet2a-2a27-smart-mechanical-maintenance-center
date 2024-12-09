/********************************************************************************
** Form generated from reading UI file 'qrcodedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.2.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QRCODEDIALOG_H
#define UI_QRCODEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_QRCodeDialog
{
public:
    QLabel *label;
    QLabel *qrCodeLabel;
    QPushButton *generateButton;
    QPushButton *pushButton;
    QLineEdit *idLineEdit;

    void setupUi(QDialog *QRCodeDialog)
    {
        if (QRCodeDialog->objectName().isEmpty())
            QRCodeDialog->setObjectName(QString::fromUtf8("QRCodeDialog"));
        QRCodeDialog->resize(518, 480);
        label = new QLabel(QRCodeDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 161, 16));
        qrCodeLabel = new QLabel(QRCodeDialog);
        qrCodeLabel->setObjectName(QString::fromUtf8("qrCodeLabel"));
        qrCodeLabel->setGeometry(QRect(110, 120, 361, 291));
        generateButton = new QPushButton(QRCodeDialog);
        generateButton->setObjectName(QString::fromUtf8("generateButton"));
        generateButton->setGeometry(QRect(200, 40, 80, 24));
        pushButton = new QPushButton(QRCodeDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(421, 445, 91, 24));
        idLineEdit = new QLineEdit(QRCodeDialog);
        idLineEdit->setObjectName(QString::fromUtf8("idLineEdit"));
        idLineEdit->setGeometry(QRect(10, 40, 181, 24));

        retranslateUi(QRCodeDialog);

        QMetaObject::connectSlotsByName(QRCodeDialog);
    } // setupUi

    void retranslateUi(QDialog *QRCodeDialog)
    {
        QRCodeDialog->setWindowTitle(QCoreApplication::translate("QRCodeDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("QRCodeDialog", "QR Code :", nullptr));
        qrCodeLabel->setText(QString());
        generateButton->setText(QCoreApplication::translate("QRCodeDialog", "Generate", nullptr));
        pushButton->setText(QCoreApplication::translate("QRCodeDialog", "Close", nullptr));
        idLineEdit->setText(QString());
        idLineEdit->setPlaceholderText(QCoreApplication::translate("QRCodeDialog", "donner le Num\303\251ro du v\303\251hicule", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QRCodeDialog: public Ui_QRCodeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QRCODEDIALOG_H
