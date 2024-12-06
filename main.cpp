#include <QApplication>
#include "connection.h"
#include <QMessageBox>
#include "login.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    connection c;
    bool test = c.createconnection();  // Check the database connection

    if (!test) {
        QMessageBox::critical(nullptr,
                              QObject::tr("Database Connection"),
                              QObject::tr("Connection failed.\nClick Cancel to exit."),
                              QMessageBox::Cancel);
        return 0;  // Terminate the application if the connection fails
    }

    Login loginPage;
    loginPage.setWindowTitle("Login");
    loginPage.show();  // Show the login window

    return a.exec();  // Start the main event loop
}
