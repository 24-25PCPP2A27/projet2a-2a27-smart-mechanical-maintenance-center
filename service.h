/*#ifndef SERVICE_H
#define SERVICE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Service {
public:
    Service();
    Service(int id, double cout, QString type, double duree, QString etats);

    bool ajouter(int idserv, const QString &type, double duree, const QString &etats, float cout);
    bool modifier(int id, const QString &type, double duree, double cout);
    bool supprimer(int id);

    QSqlQueryModel* rechercher(const QString &keyword);
    QSqlQueryModel* trier(const QString &critere, const QString &ordre);
    QSqlQueryModel* obtenirStatistiquesService();

private:
    int id;
    double cout;
    double duree;
    QString type;
    QString etats;
};

#endif // SERVICE_H
*/
#ifndef SERVICE_H
#define SERVICE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Service {
private:
    int idserv ;
    double coutserv;
    QString typeserv;
    double dureeserv;
    QString etatsserv; // Possible values: "annulé", "en cours", "terminée"

public:
    // Constructors
    Service();
    Service(int idserv, double coutserv, QString typeserv, double dureeserv, QString etatsserv);
    QSqlQueryModel* obtenirStatistiquesService();

    // Getters
    int getId() const;
    double getCout() const;
    QString getType() const;
    double getDuree() const;
    QString getEtats() const;

    // Setters
    void setId(int idserv);
    void setCout(double coutserv);
    void setType(QString typeserv);
    void setDuree(double dureeserv);
    void setEtats(QString etatsserv);

    // CRUD Operations
    bool ajouter(int idserv, const QString &typeserv, const QString &dureeserv, const QString &etatsserv, float coutserv);
    QSqlQueryModel* afficher();
    bool supprimer(int idserv);
    bool modifier(int idserv);

    // Functionalities
    QSqlQueryModel* rechercher(int idrech);

    QSqlQueryModel* trier(const QString& critere, const QString& ordre);

    void genererPDF();

};

#endif // SERVICE_H
