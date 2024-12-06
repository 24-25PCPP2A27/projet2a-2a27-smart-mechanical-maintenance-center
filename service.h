#ifndef SERVICE_H
#define SERVICE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QObject>
#include <QMap>

class Service {
private:
    int idserv;
    double coutserv;
    QString typeserv;
    double dureeserv;
    QString etatsserv; // Possible values: "annulé", "en cours", "terminée"

public:
    // Constructors
    Service();
    Service(int idserv, double coutserv, QString typeserv, double dureeserv, QString etatsserv);

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
    QMap<QString, int> statistiquesParEtats();
    QSqlQueryModel* trier(const QString& critere, const QString& ordre);

    void genererPDF();

    void onSendMessage();
};

#endif // SERVICE_H
