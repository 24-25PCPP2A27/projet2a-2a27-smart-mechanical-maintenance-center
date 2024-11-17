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
    QString NOM, PRENOM, POSTE, MDP;  // Added MDP (password) field
    int ID;
    int SALAIRE;
    int DUREE;

public:
    employe(){};  // Default constructor
    employe(int ID, QString NOM, QString PRENOM, QString POSTE, int SALAIRE, int DUREE, QString MDP);

    // Getters
    QString getNom() { return NOM; }
    QString getPRENOM() { return PRENOM; }
    QString getPOSTE() { return POSTE; }
    int getID() { return ID; }
    int getSALAIRE() { return SALAIRE; }
    int getDUREE() { return DUREE; }
    QString getMDP() { return MDP; }  // Getter for MDP

    // Setters
    void setNOM(QString n) { NOM = n; }
    void setPRENOM(QString pre) { PRENOM = pre; }
    void setPOSTE(QString po) { POSTE = po; }
    void setID(int id) { this->ID = id; }
    void setSALAIRE(int salaire) { SALAIRE = salaire; }
    void setDUREE(int duree) { DUREE = duree; }
    void setMDP(QString mdp) { MDP = mdp; }  // Setter for MDP

    // Functionality
    bool ajouter();
    bool supprimer(int id);
    bool update(int ID, QString NOM, QString PRENOM, QString POSTE, int SALAIRE, int DUREE, QString MDP);
    bool validateInputs() const;
    QSqlQueryModel* afficher();
    QSqlQueryModel* sortBySalaire();
    QMap<QString, QVariant> getDureeStatistics();
    bool exportToPDF(const QString &filePath);  // Declare the export function
    QSqlQueryModel* search(const QString &searchQuery);
    QString getPhoneNumberForEmployee(int employeeId);
};

#endif // EMPLOYE_H
