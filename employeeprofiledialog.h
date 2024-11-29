#ifndef EMPLOYEEPROFILEDIALOG_H
#define EMPLOYEEPROFILEDIALOG_H

#include <QDialog>
#include <QPixmap>

namespace Ui {
class EmployeeProfileDialog;
}

class EmployeeProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmployeeProfileDialog(QWidget *parent = nullptr);
    ~EmployeeProfileDialog();

    void setEmployeeDetails(const QString &nom, const QString &prenom, const QString &poste,
                            int salaire, int duree, const QPixmap &profilePicture);
    void displayEmployeeLogs(const QString &RFID);


private:
    Ui::EmployeeProfileDialog *ui;
};

#endif // EMPLOYEEPROFILEDIALOG_H
