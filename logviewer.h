#ifndef LOGVIEWER_H
#define LOGVIEWER_H

#include <QDialog>
#include <QTextEdit>
#include <QString>

class LogViewer : public QDialog
{
    Q_OBJECT

public:
    explicit LogViewer(QWidget *parent = nullptr);
    ~LogViewer();

    // Static method to write log entries
    static void writeLog(const QString &message);

private:
    QTextEdit *logViewer;

    void loadLogs(); // Method to load and display logs
    static QString logFilePath; // Path to the log file
};

#endif // LOGVIEWER_H
