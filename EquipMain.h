#ifndef EquipMain_H
#define EquipMain_H

#include <QMainWindow>
#include"equipements.h"

#include <QSqlDatabase>
#include <QByteArray>

#include "arduino.h"
namespace Ui {
class EquipMain;
}

class EquipMain : public QMainWindow
{
    Q_OBJECT


public:
    explicit EquipMain(QWidget *parent = nullptr);
    ~EquipMain();
protected:
    void showEvent(QShowEvent *event) override;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_modif_clicked();



    void on_pushButton_4_clicked();
    QString onBrowseButtonClicked();


    void on_rech_textChanged(const QString &arg1);

    void on_tri_currentTextChanged(const QString &arg1);

    void on_pushButton_6_clicked();

    void on_PDF_clicked();
     void on_ouvrirButton_clicked();
     void initializeArduinoConnection() ;

private:
    Ui::EquipMain *ui;
    Equipement E;
    Arduino A;
};

#endif // EquipMain_H
