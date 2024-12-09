#include "vehiculeui.h"
#include "ui_vehiculeui.h"
#include "statistique.h"
#include "qrcodedialog.h"

#include <QMessageBox>
#include <QFile>
#include <QPixmap>
#include <QPainter>
#include <QPrinter>
#include "arduino.h"

vehiculeUI::vehiculeUI(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::vehiculeUI)
{
    ui->setupUi(this);
    int ret=A.connect_arduino(); // lancer la connexion à arduino
        switch(ret){
        case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
            break;
        case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
           break;
        case(-1):qDebug() << "arduino is not available";
        }
         QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer
         //le slot update_label suite à la reception du signal readyRead (reception des données).
}

vehiculeUI::~vehiculeUI()
{
    delete ui;
}

void vehiculeUI::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    ui->tableView->setModel(v.afficher());
}


void vehiculeUI::on_ajouter_clicked()
{
    int nums = ui->nums->text().toInt();
    int numc = ui->numc->text().toInt();
    QDate datea = ui->dateEdit->date();
    float km = ui->km->text().toFloat();
    int tel = ui->tel->text().toInt();
    QString modele = ui->modele->text();
    QString marque = ui->marque->text();
    QString nom = ui->nom->text();

    vehicule v(nums, numc, tel,datea, km, modele, marque, nom);

    bool test = v.Ajouter();
    if(test) {
        ui->tableView->setModel(v.afficher());
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Error !!"),
                              QObject::tr("Cannot add vehicle."), QMessageBox::Cancel);
    }

}


void vehiculeUI::on_editer_clicked()
{
    int nums = ui->nums->text().toInt();
    int numc = ui->numc->text().toInt();
    QDate datea = ui->dateEdit->date();
    float km = ui->km->text().toFloat();
    int tel = ui->tel->text().toInt();
    QString modele = ui->modele->text();
    QString marque = ui->marque->text();
    QString nom = ui->nom->text();

    vehicule v(nums, numc, tel,datea, km, modele, marque, nom);

    bool test = v.modifier();
    if(test) {
        ui->tableView->setModel(v.afficher());
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Error !!"),
                              QObject::tr("Cannot edit vehicle."), QMessageBox::Cancel);
    }
}


void vehiculeUI::on_supp_clicked()
{
    vehicule v1;
    v1.setNumc(ui->lineEdit_supp->text().toInt());
    v1.Supprimer(v1.getNumc());
    ui->tableView->setModel(v.afficher());
}


void vehiculeUI::on_pushButton_rech_clicked()
{
    QString val=ui->lineEdit_rech->text();
    QString option=ui->comboBox_rech->currentText();

    if((val!="")&&(option=="Numero de chassis"))
    {        ui->tableView->setModel(v.afficher_numc(val));}
    else if((val!="")&&(option=="Numéro du véhicule"))
    {
        ui->tableView->setModel(v.afficher_nums(val));
    }
    else if((val!="")&&(option=="Model"))
    {
        ui->tableView->setModel(v.afficher_model(val));
    }
}


void vehiculeUI::on_comboBox_tri_currentIndexChanged(int index)
{
    QString choix=ui->comboBox_tri->currentText();
    ui->tableView->setModel(v.afficher_choix(choix));
}


void vehiculeUI::on_pushButton_pdf_clicked()
{
        int nums = ui->lineEdit_pdf->text().toInt();
        vehicule *Veh;
        Veh = v.readvehicule(nums);

        QString nums_string = QString::number(Veh->getNums());
        QString model_string = Veh->getModel();
        QString marque_string = Veh->getMarque();
        QString datea_string = Veh->getDate().toString("dd-MM-yyyy");
        QString km_string = QString::number(Veh->getKm());
        QString nom_string = Veh->getNom();
        QString numc_string = QString::number(Veh->getNumc());
        QString numtel_string = QString::number(Veh->getNumTel());

        QString pdfFilePath = "C:/Users/yassm/Desktop/vehicule (1)/vehicule/pdf/" + model_string + "_" + nums_string + ".pdf";

        QPrinter printer;
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(pdfFilePath);

        QPainter painter;
        if (!painter.begin(&printer))
        {
            qWarning() << "Failed to open PDF file for writing";
            return;
        }

        painter.setFont(QFont("Bahnschrift Light", 25));
        painter.setPen(Qt::blue);
        painter.drawText(130, 230, "Vehicule Information");
        painter.setPen(Qt::black);
        painter.drawText(120, 150, "Vehicule Management System");

        painter.drawText(150, 300, "Vehicle ID (nums): " + nums_string);
        painter.drawText(150, 400, "Model: " + model_string);
        painter.drawText(150, 500, "Brand: " + marque_string);
        painter.drawText(150, 600, "Date of Acquisition: " + datea_string);
        painter.drawText(150, 700, "Mileage (km): " + km_string);
        painter.drawText(150, 800, "Owner Name: " + nom_string);
        painter.drawText(150, 900, "Category Number (numc): " + numc_string);
        painter.drawText(150, 1000, "Phone Number: " + numtel_string);

        painter.end();
    }


    void vehiculeUI::on_pushButton_clicked()
    {
        ui->tableView->setModel(v.afficher());
    }


    void vehiculeUI::on_pushButton_stat_clicked()
    {
        statistique S;
        S.exec();
    }


    void vehiculeUI::on_pushButton_qrcode_clicked()
    {
        QRCodeDialog QRcode;
        QRcode.exec();
    }

void vehiculeUI::update_label()
{
    QByteArray data1=A.read_from_arduino();
    data1 = QByteArray(1, data1[0]);
    data=data+data1;
ui->V_ID->setText(data);


}

void vehiculeUI::on_Verif_button_clicked()
{


    QString id = ui->V_ID->text();
    QSqlQuery q ;
    q.prepare("SELECT * From Vehicule where NUMS = :id");
    q.bindValue(":id",id);
    q.exec();
    q.first();
    if (q.value(0).toString()!= "")
    {
            QMessageBox::information(nullptr, QObject::tr("Ok"),
                        QObject::tr("Vehicule trouvé.\n"
                                    "Click cancel to exit."), QMessageBox::Cancel);
}
            else
    {
            QMessageBox::critical(nullptr, QObject::tr("Ok"),
                        QObject::tr("Vehicule non trouvé.\n"
                                    "Click cancel to exit."), QMessageBox::Cancel);
}



}
