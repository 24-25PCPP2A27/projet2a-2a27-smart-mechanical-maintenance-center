#ifndef SMSHANDLER_H
#define SMSHANDLER_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class SmsHandler : public QObject
{
    Q_OBJECT

public:
    explicit SmsHandler(QObject *parent = nullptr);  // Constructor
    ~SmsHandler();  // Destructor

    // Function to send SMS using Twilio API
    bool sendSmsNotification(const QString &phoneNumber, const QString &message);

private:
    QNetworkAccessManager *manager;  // Network manager for sending requests
};

#endif // SMSHANDLER_H
