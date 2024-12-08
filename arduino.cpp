#include "arduino.h"

// Constructeur : initialisation des attributs
Arduino::Arduino()
{
    data = "";
    arduino_port_name = "";
    arduino_is_available = false;
    serial = new QSerialPort;
}

// Retourne le nom du port série où Arduino est connecté
QString Arduino::getarduino_port_name()
{
    return arduino_port_name;
}

// Retourne l'objet QSerialPort pour usage externe si nécessaire
QSerialPort* Arduino::getserial()
{
    return serial;
}

// Connecte l'application Qt à Arduino via le port série
int Arduino::connect_arduino()
{
    // Recherche le port sur lequel Arduino est connecté
    foreach (const QSerialPortInfo& serial_port_info, QSerialPortInfo::availablePorts()) {
        if (serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()) {
            if (serial_port_info.vendorIdentifier() == arduino_uno_vendor_id &&
                serial_port_info.productIdentifier() == arduino_uno_product_id) {
                arduino_is_available = true;
                arduino_port_name = serial_port_info.portName();
            }
        }
    }

    qDebug() << "Arduino port name is: " << arduino_port_name;

    // Si Arduino est disponible, configure le port série
    if (arduino_is_available) {
        serial->setPortName(arduino_port_name);
        if (serial->open(QSerialPort::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            return 0;  // Succès
        }
        return 1;  // Échec de l'ouverture du port
    }
    return -1;  // Arduino non trouvé
}

// Ferme la connexion avec Arduino
int Arduino::close_arduino()
{
    if (serial->isOpen()) {
        serial->close();
        return 0;  // Succès
    }
    return 1;  // Échec de la fermeture
}

// Envoie des données à Arduino
void Arduino::write_to_arduino(QByteArray d)
{
    if (serial->isWritable()) {
        serial->write(d);
    } else {
        qDebug() << "Erreur : Impossible d'écrire sur le port série.";
    }
}

// Lit les données reçues depuis Arduino
QByteArray Arduino::read_from_arduino()
{
    if (serial->isReadable()) {
        data = serial->readAll();
        return data;
    }
    return "";
}
