#ifndef ARDUINO_H
#define ARDUINO_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

class Arduino
{
public:
    Arduino();  // Constructeur
    int connect_arduino();  // Connecte Arduino via le port série
    int close_arduino();  // Ferme la connexion
    void write_to_arduino(QByteArray data);  // Envoie des données à Arduino
    QByteArray read_from_arduino();  // Lit les données reçues d'Arduino
    QSerialPort* getserial();  // Retourne l'objet QSerialPort pour usage externe
    QString getarduino_port_name();  // Retourne le nom du port où Arduino est connecté

private:
    QSerialPort* serial;  // Objet pour la communication série
    static const quint16 arduino_uno_vendor_id = 9025;  // Identifiant du fabricant Arduino
    static const quint16 arduino_uno_product_id = 67;  // Identifiant du produit Arduino (Arduino Uno)
    QString arduino_port_name;  // Nom du port série où Arduino est connecté
    bool arduino_is_available;  // Indicateur de disponibilité d'Arduino
    QByteArray data;  // Données lues depuis Arduino
};

#endif // ARDUINO_H
