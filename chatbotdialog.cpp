#include "chatbotdialog.h"
#include "ui_chatbotdialog.h"
#include "chatbot.h"
#include "service.h"

ChatBotDialog::ChatBotDialog(Service *serviceManager, QWidget *parent)
    : QDialog(parent), ui(new Ui::ChatBotDialog), chatbotInstance(new ChatBot(this))
{
    ui->setupUi(this);

    // Connect the send button to the chatbot logic
    connect(ui->sendButton, &QPushButton::clicked, this, &ChatBotDialog::onSendButtonClicked);

    // Display the welcome message in the chat window
    QString welcomeMessage = "Bonjour!\nTapez 'help' pour voir la liste des commandes disponibles.";
    ui->chatDisplay->append(welcomeMessage);
    ui->chatDisplay->setReadOnly(true);
}

ChatBotDialog::~ChatBotDialog()
{
    delete ui;
    delete chatbotInstance;  // Proper cleanup
}

void ChatBotDialog::onSendButtonClicked()
{
    QString userInput = ui->inputField->text();             // Get user input
    QString response = chatbotInstance->getResponse(userInput); // Process chatbot response

    // Display user input and chatbot response in the chat display
    ui->chatDisplay->append("User: " + userInput);
    ui->chatDisplay->append("Bot: " + response);

    // Clear the input field
    ui->inputField->clear();
}
