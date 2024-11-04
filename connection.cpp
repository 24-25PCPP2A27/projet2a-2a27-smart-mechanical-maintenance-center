#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnection()
{
db= QSqlDatabase::addDatabase("QODBC");
bool test=false;

db.setDatabaseName("CPP_Project");//inserer le nom de la source de données
db.setUserName("mechanic");//inserer nom de l'utilisateur
db.setPassword("oracle");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;





    return  test;
}
