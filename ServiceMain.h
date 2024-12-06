
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "service.h"
#include "arduino.h"


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

    void onArduinoDataReceived();


private:
    Ui::ServiceMain *ui;
    Service tmpService;
    Service service;
    Service serviceInstance;// Objet temporaire pour gérer les services
    void setupArduinoConnection();
    Arduino arduino;
    bool buttonPressedBuffer; // To act as a buffer
    QTimer *resetBufferTimer; // Timer to reset the buffer
    };

#endif // MAINWINDOW_H
