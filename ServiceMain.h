/*#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "service.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ajouter_clicked();       // ajouter button
    void on_afficher_clicked();      // afficher button
    void on_updateServ_clicked();    // update serv button
    void on_deleteServ_clicked();    // delete serv button
    void on_recherche_clicked();     // recherche button
    void on_trier_clicked();         // trier button
    void on_pdf_clicked();           // pdf button
    void on_stat_clicked();          // stat button
    void on_sendMsg_clicked();       // send msg button

private:
    Ui::MainWindow *ui;
    Service tmpService;
};

#endif // MAINWINDOW_H
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "service.h"



QT_BEGIN_NAMESPACE
namespace Ui { class ServiceMain; }
QT_END_NAMESPACE

class ServiceMain : public QMainWindow {  // Hérite correctement de QMainWindow
    Q_OBJECT

public:
    explicit ServiceMain(QWidget *parent = nullptr);
    ~ServiceMain();

      void afficherServices();

private slots:
    void on_statistique_clicked();
    void on_pushButton_clicked();
    void on_conversionenpdf_clicked();
    void on_trier_clicked();
    void on_rechercher_clicked();
    void on_updateButton_clicked();
    void on_deleteButton_clicked();
    void on_chat_clicked();
    void changerEtatEquipement();


private:
    Ui::ServiceMain *ui;
    Service tmpService;
    Service service;
    Service serviceInstance;// Objet temporaire pour gérer les services

    };

#endif // MAINWINDOW_H
