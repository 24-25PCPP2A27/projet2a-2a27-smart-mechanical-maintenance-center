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

    // Display a welcome message in the chat window
    QString welcomeMessage =
        "Bonjour ! Bienvenue au chatbot de gestion des services.\n"
        "Tapez 'help' pour voir la liste des commandes disponibles.\n"
        "Commandes disponibles:\n"
        "- Add Service, ID, Type, Cost, Duration, Status\n"
        "- Update Service\n"
        "- Delete Service, ID\n"
        "- List Services\n"
        "- Export PDF";
    ui->chatDisplay->append(welcomeMessage);
    ui->chatDisplay->setReadOnly(true); // Make the chat display read-only
}

ChatBotDialog::~ChatBotDialog()
{
    delete ui;
    delete chatbotInstance; // Ensure proper cleanup
}

void ChatBotDialog::onSendButtonClicked()
{
    QString userInput = ui->inputField->text().trimmed(); // Get and trim user input
    if (userInput.isEmpty()) {
        return; // Do nothing if input is empty
    }

    QString response = chatbotInstance->getResponse(userInput); // Get chatbot response

    // Display user input and chatbot response in the chat display
    ui->chatDisplay->append("User: " + userInput);
    ui->chatDisplay->append("Bot: " + response);

    // Clear the input field after sending
    ui->inputField->clear();
}
