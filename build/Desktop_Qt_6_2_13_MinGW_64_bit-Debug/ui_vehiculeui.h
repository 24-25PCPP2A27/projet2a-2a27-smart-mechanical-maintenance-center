/********************************************************************************
** Form generated from reading UI file 'vehiculeui.ui'
**
** Created by: Qt User Interface Compiler version 6.2.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VEHICULEUI_H
#define UI_VEHICULEUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_vehiculeUI
{
public:
    QTableView *tableView;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *nums;
    QLineEdit *numc;
    QDateEdit *dateEdit;
    QLineEdit *km;
    QLineEdit *tel;
    QLineEdit *modele;
    QLineEdit *marque;
    QLineEdit *nom;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout;
    QPushButton *ajouter;
    QPushButton *editer;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *lineEdit_supp;
    QPushButton *supp;
    QLabel *label;
    QLineEdit *lineEdit_rech;
    QComboBox *comboBox_rech;
    QPushButton *pushButton_rech;
    QLabel *label_2;
    QComboBox *comboBox_tri;
    QPushButton *pushButton_stat;
    QPushButton *pushButton_qrcode;
    QLabel *label_3;
    QLineEdit *lineEdit_pdf;
    QPushButton *pushButton_pdf;
    QPushButton *pushButton;

    void setupUi(QDialog *vehiculeUI)
    {
        if (vehiculeUI->objectName().isEmpty())
            vehiculeUI->setObjectName(QString::fromUtf8("vehiculeUI"));
        vehiculeUI->resize(1039, 472);
        tableView = new QTableView(vehiculeUI);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setGeometry(QRect(210, 130, 811, 291));
        layoutWidget = new QWidget(vehiculeUI);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 130, 171, 291));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        nums = new QLineEdit(layoutWidget);
        nums->setObjectName(QString::fromUtf8("nums"));

        verticalLayout->addWidget(nums);

        numc = new QLineEdit(layoutWidget);
        numc->setObjectName(QString::fromUtf8("numc"));

        verticalLayout->addWidget(numc);

        dateEdit = new QDateEdit(layoutWidget);
        dateEdit->setObjectName(QString::fromUtf8("dateEdit"));

        verticalLayout->addWidget(dateEdit);

        km = new QLineEdit(layoutWidget);
        km->setObjectName(QString::fromUtf8("km"));

        verticalLayout->addWidget(km);

        tel = new QLineEdit(layoutWidget);
        tel->setObjectName(QString::fromUtf8("tel"));

        verticalLayout->addWidget(tel);

        modele = new QLineEdit(layoutWidget);
        modele->setObjectName(QString::fromUtf8("modele"));

        verticalLayout->addWidget(modele);

        marque = new QLineEdit(layoutWidget);
        marque->setObjectName(QString::fromUtf8("marque"));

        verticalLayout->addWidget(marque);

        nom = new QLineEdit(layoutWidget);
        nom->setObjectName(QString::fromUtf8("nom"));

        verticalLayout->addWidget(nom);

        layoutWidget1 = new QWidget(vehiculeUI);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(20, 430, 171, 26));
        horizontalLayout = new QHBoxLayout(layoutWidget1);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        ajouter = new QPushButton(layoutWidget1);
        ajouter->setObjectName(QString::fromUtf8("ajouter"));

        horizontalLayout->addWidget(ajouter);

        editer = new QPushButton(layoutWidget1);
        editer->setObjectName(QString::fromUtf8("editer"));

        horizontalLayout->addWidget(editer);

        layoutWidget2 = new QWidget(vehiculeUI);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(770, 430, 241, 26));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        lineEdit_supp = new QLineEdit(layoutWidget2);
        lineEdit_supp->setObjectName(QString::fromUtf8("lineEdit_supp"));

        horizontalLayout_2->addWidget(lineEdit_supp);

        supp = new QPushButton(layoutWidget2);
        supp->setObjectName(QString::fromUtf8("supp"));

        horizontalLayout_2->addWidget(supp);

        label = new QLabel(vehiculeUI);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(70, 20, 111, 31));
        lineEdit_rech = new QLineEdit(vehiculeUI);
        lineEdit_rech->setObjectName(QString::fromUtf8("lineEdit_rech"));
        lineEdit_rech->setGeometry(QRect(230, 50, 113, 24));
        comboBox_rech = new QComboBox(vehiculeUI);
        comboBox_rech->addItem(QString());
        comboBox_rech->addItem(QString());
        comboBox_rech->addItem(QString());
        comboBox_rech->setObjectName(QString::fromUtf8("comboBox_rech"));
        comboBox_rech->setGeometry(QRect(71, 50, 151, 24));
        pushButton_rech = new QPushButton(vehiculeUI);
        pushButton_rech->setObjectName(QString::fromUtf8("pushButton_rech"));
        pushButton_rech->setGeometry(QRect(350, 50, 80, 24));
        label_2 = new QLabel(vehiculeUI);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(480, 28, 70, 21));
        comboBox_tri = new QComboBox(vehiculeUI);
        comboBox_tri->addItem(QString());
        comboBox_tri->addItem(QString());
        comboBox_tri->addItem(QString());
        comboBox_tri->addItem(QString());
        comboBox_tri->addItem(QString());
        comboBox_tri->addItem(QString());
        comboBox_tri->setObjectName(QString::fromUtf8("comboBox_tri"));
        comboBox_tri->setGeometry(QRect(480, 50, 211, 24));
        pushButton_stat = new QPushButton(vehiculeUI);
        pushButton_stat->setObjectName(QString::fromUtf8("pushButton_stat"));
        pushButton_stat->setGeometry(QRect(250, 84, 131, 31));
        pushButton_qrcode = new QPushButton(vehiculeUI);
        pushButton_qrcode->setObjectName(QString::fromUtf8("pushButton_qrcode"));
        pushButton_qrcode->setGeometry(QRect(460, 84, 131, 31));
        label_3 = new QLabel(vehiculeUI);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(750, 30, 101, 21));
        lineEdit_pdf = new QLineEdit(vehiculeUI);
        lineEdit_pdf->setObjectName(QString::fromUtf8("lineEdit_pdf"));
        lineEdit_pdf->setGeometry(QRect(750, 50, 113, 24));
        pushButton_pdf = new QPushButton(vehiculeUI);
        pushButton_pdf->setObjectName(QString::fromUtf8("pushButton_pdf"));
        pushButton_pdf->setGeometry(QRect(870, 50, 80, 24));
        pushButton = new QPushButton(vehiculeUI);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(940, 100, 80, 24));

        retranslateUi(vehiculeUI);

        QMetaObject::connectSlotsByName(vehiculeUI);
    } // setupUi

    void retranslateUi(QDialog *vehiculeUI)
    {
        vehiculeUI->setWindowTitle(QCoreApplication::translate("vehiculeUI", "Dialog", nullptr));
        nums->setPlaceholderText(QCoreApplication::translate("vehiculeUI", "nums", nullptr));
        numc->setPlaceholderText(QCoreApplication::translate("vehiculeUI", "numc", nullptr));
        km->setPlaceholderText(QCoreApplication::translate("vehiculeUI", "km", nullptr));
        tel->setPlaceholderText(QCoreApplication::translate("vehiculeUI", "tel", nullptr));
        modele->setPlaceholderText(QCoreApplication::translate("vehiculeUI", "modele", nullptr));
        marque->setPlaceholderText(QCoreApplication::translate("vehiculeUI", "marque", nullptr));
        nom->setPlaceholderText(QCoreApplication::translate("vehiculeUI", "nom", nullptr));
        ajouter->setText(QCoreApplication::translate("vehiculeUI", "Ajouter", nullptr));
        editer->setText(QCoreApplication::translate("vehiculeUI", "Editer", nullptr));
        lineEdit_supp->setPlaceholderText(QCoreApplication::translate("vehiculeUI", "Supprimer", nullptr));
        supp->setText(QCoreApplication::translate("vehiculeUI", "Supprimer", nullptr));
        label->setText(QCoreApplication::translate("vehiculeUI", "Recherche", nullptr));
        comboBox_rech->setItemText(0, QCoreApplication::translate("vehiculeUI", "Numero de chassis", nullptr));
        comboBox_rech->setItemText(1, QCoreApplication::translate("vehiculeUI", "Num\303\251ro du v\303\251hicule", nullptr));
        comboBox_rech->setItemText(2, QCoreApplication::translate("vehiculeUI", "Model", nullptr));

        pushButton_rech->setText(QCoreApplication::translate("vehiculeUI", "Recherche", nullptr));
        label_2->setText(QCoreApplication::translate("vehiculeUI", "Tri", nullptr));
        comboBox_tri->setItemText(0, QCoreApplication::translate("vehiculeUI", "Kilom\303\251trage croissants", nullptr));
        comboBox_tri->setItemText(1, QCoreApplication::translate("vehiculeUI", "Kilom\303\251trage decroissants", nullptr));
        comboBox_tri->setItemText(2, QCoreApplication::translate("vehiculeUI", "Num\303\251ro du v\303\251hicule decroissants", nullptr));
        comboBox_tri->setItemText(3, QCoreApplication::translate("vehiculeUI", "Num\303\251ro du v\303\251hicule croissants", nullptr));
        comboBox_tri->setItemText(4, QCoreApplication::translate("vehiculeUI", "Date d'entr\303\251e decroissants", nullptr));
        comboBox_tri->setItemText(5, QCoreApplication::translate("vehiculeUI", "Date d'entr\303\251e croissants", nullptr));

        pushButton_stat->setText(QCoreApplication::translate("vehiculeUI", "Statistique", nullptr));
        pushButton_qrcode->setText(QCoreApplication::translate("vehiculeUI", "QrCode", nullptr));
        label_3->setText(QCoreApplication::translate("vehiculeUI", "PDF", nullptr));
        pushButton_pdf->setText(QCoreApplication::translate("vehiculeUI", "Export", nullptr));
        pushButton->setText(QCoreApplication::translate("vehiculeUI", "Refresh", nullptr));
    } // retranslateUi

};

namespace Ui {
    class vehiculeUI: public Ui_vehiculeUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VEHICULEUI_H
