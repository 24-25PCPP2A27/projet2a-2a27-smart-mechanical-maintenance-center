#include "EmployeMain.h"
#include <QApplication>
#include "connection.h"
#include <QMessageBox>
#include "login.h"  // Include the new login header

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    connection c;
    bool test = c.createconnection();  // Check the database connection first

    // If the connection fails, show an error and exit
    if (!test) {
        QMessageBox::critical(nullptr,
                              QObject::tr("Database Connection"),
                              QObject::tr("Connection failed.\n Click Cancel to exit."),
                              QMessageBox::Cancel);
        return 0;  // Terminate the application if the connection fails
    }

    // If the connection is successful, show the login page
    Login loginPage;  // Create the Login object
    loginPage.setWindowTitle("Login");

    // Show the login page and wait for the result
    int result = loginPage.exec();  // Run the dialog's event loop

    // Check if login was successful (QDialog::Accepted)
    if (result == QDialog::Accepted) {
        MainWindow w;  // Main window
        w.show();  // Show the main window
        return a.exec();  // Start the event loop for the main window
    }

    // If login was not successful, terminate the application
    return 0;
}
