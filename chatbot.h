#ifndef CHATBOT_H
#define CHATBOT_H

#include <QObject>
#include <QString>

class ChatBot : public QObject
{
    Q_OBJECT
public:
    explicit ChatBot(QObject *parent = nullptr);

    // Processes user input and returns an appropriate response
    QString getResponse(const QString &input);

private:
    // Handlers for specific commands
    QString handleAddService(const QString &input);
    QString handleDeleteService(const QString &input);
    QString handleExportToPDF();
    QString handleListServices();

    // Helper function to export services to a PDF file
    bool exportToPDF();
};

#endif // CHATBOT_H
