#include "arduino.h"

Arduino::Arduino()
{
    data = "";
    arduino_port_name = "";
    arduino_is_available = false;
    serial = new QSerialPort;
}

Arduino::~Arduino() {
    if (serial->isOpen()) {
        serial->close();
    }
    delete serial;
}

QString Arduino::getarduino_port_name() {
    return arduino_port_name;
}

QSerialPort* Arduino::getserial() {
    return serial;
}

int Arduino::connect_arduino() {
    if (serial->isOpen()) {
        qDebug() << "Port série déjà ouvert.";
        return 0; // Already connected
    }

    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) {
        if (serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()) {
            if (serial_port_info.vendorIdentifier() == arduino_uno_vendor_id &&
                serial_port_info.productIdentifier() == arduino_uno_product_id) {
                arduino_is_available = true;
                arduino_port_name = serial_port_info.portName();
                qDebug() << "Arduino port found: " << arduino_port_name;
                break; // Exit loop once Arduino is found
            }
        }
    }

    if (arduino_is_available) {
        serial->setPortName(arduino_port_name);
        if (serial->open(QSerialPort::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            qDebug() << "Arduino connected successfully.";
            return 0; // Connection successful
        } else {
            qDebug() << "Error opening serial port: " << serial->errorString();
            return 1; // Error opening port
        }
    } else {
        qDebug() << "Arduino not found on any available port.";
        return -1; // Arduino not found
    }
}

int Arduino::reconnect_arduino() {
    if (serial->isOpen()) {
        qDebug() << "Port série déjà ouvert.";
        return -2; // Port already open
    }

    qDebug() << "Tentative de reconnexion à l'Arduino...";
    if (serial->open(QSerialPort::ReadWrite)) {
        qDebug() << "Port série réouvert en lecture/écriture.";
        if (serial->setBaudRate(QSerialPort::Baud9600) &&
            serial->setDataBits(QSerialPort::Data8) &&
            serial->setParity(QSerialPort::NoParity) &&
            serial->setStopBits(QSerialPort::OneStop) &&
            serial->setFlowControl(QSerialPort::NoFlowControl)) {
            qDebug() << "Paramètres de communication série reconfigurés avec succès.";
            return 0; // Reconnection successful
        } else {
            qDebug() << "Erreur lors de la reconfiguration des paramètres de communication série.";
            return 2; // Error reconfiguring
        }
    } else {
        qDebug() << "Erreur lors de la réouverture du port série: " << serial->errorString();
        return 1; // Error opening port
    }
}

int Arduino::close_arduino() {
    if (serial->isOpen()) {
        serial->close();
        qDebug() << "Port série fermé.";
        return 0; // Successfully closed
    }
    qDebug() << "Le port série n'était pas ouvert.";
    return 1; // Port was not open
}

QByteArray Arduino::read_from_arduino() {
    if (serial->isReadable()) {
        data = serial->readLine(); // Read data received
        return data;
    } else {
        qDebug() << "Port série non lisible.";
        return QByteArray();
    }
}

int Arduino::write_to_arduino(QByteArray d) {
    if (serial->isWritable()) {
        serial->write(d); // Send data to Arduino
        qDebug() << "Données envoyées: " << d;
        return 0; // Successfully written
    } else {
        qDebug() << "Erreur : Impossible d'écrire sur le port série.";
        return 1; // Error writing
    }
}

bool Arduino::isConnected() const {
    return serial->isOpen();
}
