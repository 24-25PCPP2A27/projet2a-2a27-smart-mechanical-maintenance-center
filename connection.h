#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

class connection {
    QSqlDatabase db;
public:
    connection();
    bool createconnection();
    void closeconnection();
    void update_equipment_state(int id, const QString& new_state);



    };


#endif // CONNECTION_Ha


