#include "connection.h"

connection::connection(){}

bool connection::createconnection(){
    db = QSqlDatabase::addDatabase("QODBC");
    bool test=false;
<<<<<<< HEAD
    db.setDatabaseName("test-bd");
    db.setUserName("gouader");
    db.setPassword("gouader");
=======
    db.setDatabaseName("PROJET_CPP");
    db.setUserName("mechanic");
    db.setPassword("oracle");
>>>>>>> 36a7267668d4c9d3fab1507a21c81fb9ced4f56f
    if (db.open()) test=true;
    return test;
}

void connection::closeconnection(){db.close();}
