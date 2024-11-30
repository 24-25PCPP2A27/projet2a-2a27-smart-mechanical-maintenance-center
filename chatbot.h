/*#ifndef CHATBOT_H
#define CHATBOT_H

#include <QObject>
#include "service.h"

class ChatBot : public QObject {
    Q_OBJECT

public:
    explicit ChatBot(employe *employeManager, QObject *parent = nullptr);
    QString processInput(const QString &input);

private:
    employe *employeManager; // Pointer to employe class instance
    QString handleEmployeeQuery(const QString &query);

    // Declare the functions for employee management
    QString handleAddEmployee(const QStringList &details);
    QString handleDeleteEmployee(int id);  // Add the delete function

    // Additional functions for other operations
    QString handleUpdateEmployee(const QStringList &details);
    QString handleExportToPDF(const QString &filePath);
    QString handleSearchEmployee(const QString &searchTerm);
    QString handleHelp();

};

#endif // CHATBOT_H
*/
#ifndef CHATBOT_H
#define CHATBOT_H

#include <QObject>
#include <QString>

class ChatBot : public QObject
{
    Q_OBJECT

public:
    explicit ChatBot(QObject *parent = nullptr);

    // Method to get a response from the chatbot
    QString getResponse(const QString &input);

private:
    // Helper methods for specific functionalities
    bool exportToPDF();
    QString listServices();
};

#endif // CHATBOT_H
