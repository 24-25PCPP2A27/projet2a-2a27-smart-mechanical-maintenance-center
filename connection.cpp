#include "connection.h"

connection::connection(){}

bool connection::createconnection(){
    db = QSqlDatabase::addDatabase("QODBC");
    bool test=false;
    db.setDatabaseName("PROJET_CPP");
    db.setUserName("mechanic");
    db.setPassword("oracle");
    if (db.open()) test=true;
    return test;
}

void connection::closeconnection(){db.close();}
