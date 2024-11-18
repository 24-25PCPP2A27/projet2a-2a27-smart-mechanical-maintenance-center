#ifndef SERVICE_H
#define SERVICE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Service {
private:
    int id;
    double cout;
    QString type;
    double duree;
    QString etats; // Possible values: "annulé", "en cours", "terminée"

public:
    // Constructors
    Service();
    Service(int id, double cout, QString type, double duree, QString etats);
    QSqlQueryModel* obtenirStatistiquesService();
    // Getters
    int getId() const;
    double getCout() const;
    QString getType() const;
    double getDuree() const;
    QString getEtats() const;

    // Setters
    void setId(int id);
    void setCout(double cout);
    void setType(QString type);
    void setDuree(double duree);
    void setEtats(QString etats);

    // CRUD Operations
    bool ajouter(int idserv, const QString &type, const QString &duree, const QString &etats, float cout);
    QSqlQueryModel* afficher();
    bool supprimer(int id);
    bool modifier(int id);

    // Functionalities
    QSqlQueryModel* rechercher(const QString& keyword);
    QSqlQueryModel* trier(const QString& critere, const QString& ordre);
    void genererPDF();
};

#endif // SERVICE_H
