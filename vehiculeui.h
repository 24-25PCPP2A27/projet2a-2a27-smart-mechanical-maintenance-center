#ifndef VEHICULEUI_H
#define VEHICULEUI_H

#include <QDialog>

#include "vehicule.h"
#include "arduino.h"
namespace Ui {
class vehiculeUI;
}

class vehiculeUI : public QDialog
{
    Q_OBJECT

public:
    explicit vehiculeUI(QWidget *parent = nullptr);
    ~vehiculeUI();

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void on_ajouter_clicked();

    void on_editer_clicked();

    void on_supp_clicked();

    void on_pushButton_rech_clicked();

    void on_comboBox_tri_currentIndexChanged(int index);

    void on_pushButton_pdf_clicked();

    void on_pushButton_clicked();

    void on_pushButton_stat_clicked();

    void on_pushButton_qrcode_clicked();

    void on_Verif_button_clicked();
    void update_label();

private:
    Ui::vehiculeUI *ui;
    vehicule v;
    QByteArray data;
    Arduino A;
};

#endif // VEHICULEUI_H
