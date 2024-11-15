#ifndef EMPLOYE_H
#define EMPLOYE_H
#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include <QDate>
#include <QMap>
#include <QVariant>


class employe
{
    QString NOM , PRENOM, POSTE;
    int ID;
    int SALAIRE;
    int DUREE;

public:
    employe(){};
    employe(int ID,QString nom,QString PRENOM ,QString POSTE ,int SALAIRE ,int DUREE);

    //getteres mteena
    QString getNom(){return NOM;}
    QString getPRENOM(){return PRENOM;}
    QString getPOSTE(){return POSTE;}
    int getID(){return ID;}
    int getSALAIRE(){return SALAIRE;}
    int getDUREE(){return DUREE;}
    QSqlQueryModel* afficher();
    QSqlQueryModel* sortBySalaire();
    QMap<QString, QVariant> getDureeStatistics();
    bool exportToPDF(const QString &filePath);  // Declare the export function
    QSqlQueryModel* search(const QString &searchQuery);
    //setters mteena

    void setNOM(QString n){NOM=n;}
    void setPRENOM(QString pre){PRENOM=pre;}
    void setPOSTE(QString po){POSTE=po;}
    void setID(int ID ){this->ID=ID;}
    void setDUREE(int date) { DUREE =DUREE; }
       // fonctionnalités
    bool ajouter();
    bool supprimer(int);
    bool update(int ID, QString NOM, QString PRENOM, QString POSTE, int SALAIRE, int DUREE);
    bool validateInputs() const;


};

#endif // EMPLOYE_H
