#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "service.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {  // Hérite correctement de QMainWindow
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_statistique_clicked();
    void on_pushButton_clicked();
    void on_conversionenpdf_clicked();
    void on_afficher_clicked();
    void on_trier_clicked();
    void on_rechercher_clicked();
    void on_updateButton_clicked();
    void on_deleteButton_clicked();
    void onSendMessage();

private:
    Ui::MainWindow *ui;
    Service tmpService;
    Service serviceInstance;// Objet temporaire pour gérer les services
};

#endif // MAINWINDOW_H
