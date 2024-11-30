#ifndef CHATBOTDIALOG_H
#define CHATBOTDIALOG_H

#include <QDialog>
#include "chatbot.h"
#include "service.h"

namespace Ui {
class ChatBotDialog;
}

class ChatBotDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatBotDialog(Service *serviceManager, QWidget *parent = nullptr);
    ~ChatBotDialog();

private slots:
    void onSendButtonClicked();

private:
    Ui::ChatBotDialog *ui;
    ChatBot *chatbotInstance;  // Renamed to avoid conflict
};

#endif // CHATBOTDIALOG_H
