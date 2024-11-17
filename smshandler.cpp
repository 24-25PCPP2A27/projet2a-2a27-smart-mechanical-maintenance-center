#include "smshandler.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>
#include <QDebug>

SmsHandler::SmsHandler(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
}

SmsHandler::~SmsHandler()
{
    delete manager;  // Clean up the manager
}

bool SmsHandler::sendSmsNotification(const QString &phoneNumber, const QString &message)
{
    // Replace with your Twilio details
    const QString twilioSid = "AC11c53c8b4bd744ba967e73eda65feb6d";
    const QString twilioAuthToken = "b279f29ad1e50b8e893935cbd0e28d8d";
    const QString twilioPhoneNumber = "+12569524082";

    QUrl url("https://api.twilio.com/2010-04-01/Accounts/" + twilioSid + "/Messages.json");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray data;
    data.append("To=" + phoneNumber);
    data.append("&From=" + twilioPhoneNumber);
    data.append("&Body=" + message);

    // Send the POST request to Twilio API
    QString credentials = twilioSid + ":" + twilioAuthToken;
    QByteArray authHeader = "Basic " + credentials.toUtf8().toBase64();
    request.setRawHeader("Authorization", authHeader);

    QNetworkReply *reply = manager->post(request, data);

    // Wait for the request to finish
    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "SMS sent successfully!";
        } else {
            qDebug() << "Error sending SMS: " << reply->errorString();
        }
        reply->deleteLater();
    });

    return true;
}
