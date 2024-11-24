#include "logviewer.h"
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QPushButton>
#include <QDateTime>

QString LogViewer::logFilePath = "application.log"; // Path to the log file

LogViewer::LogViewer(QWidget *parent)
    : QDialog(parent), logViewer(new QTextEdit(this))
{
    setWindowTitle("Application Logs");

    // Layout for the log viewer
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Log display area
    logViewer->setReadOnly(true);
    layout->addWidget(logViewer);

    // Refresh button
    QPushButton *refreshButton = new QPushButton("Refresh Logs", this);
    connect(refreshButton, &QPushButton::clicked, this, &LogViewer::loadLogs);
    layout->addWidget(refreshButton);

    loadLogs(); // Load logs when the viewer opens
}

LogViewer::~LogViewer() {}

void LogViewer::loadLogs()
{
    QFile logFile(logFilePath);
    if (logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&logFile);
        logViewer->setPlainText(stream.readAll());
        logFile.close();
    } else {
        logViewer->setPlainText("Unable to load logs.");
    }
}

// Static method to write logs
void LogViewer::writeLog(const QString &message)
{
    QFile logFile(logFilePath);
    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&logFile);
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        stream << timestamp << " - " << message << "\n";
        logFile.close();
    }
}

