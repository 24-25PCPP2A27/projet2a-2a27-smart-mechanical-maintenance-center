#ifndef SERVICE_H
#define SERVICE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Service
{
private:
    int IDS;
    QString TYPE;
    QString DUREE;
    QString EQUIPMENT;
    QString ETATS;
    QString COUT;

public:
    // Constructeurs
    Service() {}
    Service(int ids, QString type, QString duree, QString equipment, QString etats, QString cout);

    // Getters
    int getId() const { return IDS; }
    QString getType() const { return TYPE; }
    QString getDuree() const { return DUREE; }
    QString getEquipment() const { return EQUIPMENT; }
    QString getEtats() const { return ETATS; }
    QString getCout() const { return COUT; }

    // Setters
    void setId(int ids) { IDS = ids; }
    void setType(const QString &type) { TYPE = type; }
    void setDuree(const QString &duree) { DUREE = duree; }
    void setEquipment(const QString &equipment) { EQUIPMENT = equipment; }
    void setEtats(const QString &etats) { ETATS = etats; }
    void setCout(const QString &cout) { COUT = cout; }

    // Méthodes de base de données
    bool Ajouter();               // Ajouter un service dans la base de données
    QSqlQueryModel* afficher();   // Afficher tous les services
    bool supprimer(int ids);       // Supprimer un service par ID
    bool update(int ids, QString type, QString duree, QString equipment, QString etats, QString cout); // Mettre à jour un service
};

#endif // SERVICE_H
