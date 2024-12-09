#ifndef CONNEXION_H
#define CONNEXION_H


#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class connexion
{private:

public:
    QSqlDatabase db;
    connexion();
    bool createconnect();
    void closeconnection();
};


#endif
