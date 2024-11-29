/*#ifndef SERVICE_H
#define SERVICE_H

#include <QString>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMap>

class Service {
private:
    int idserv;
    double coutserv;
    QString typeserv;
    QString dureeserv; // Changed from double to QString
    QString etatsserv;

public:
    // Constructors
    Service();
    Service(int idserv, double coutserv, QString typeserv, QString duree, QString etatsserv);

    // Getters
    int getId() const;
    double getCout() const;
    QString getType() const;
    QString getDuree() const; // Changed return type to QString
    QString getEtats() const;

    // Setters
    void setId(int idserv);
    void setCout(double coutserv);
    void setType(const QString &typeserv);
    void setDuree(const QString &duree); // Changed parameter type to QString
    void setEtats(const QString &etatsserv);

    // Method to add a new service
    bool ajouter(int idserv, const QString &typeserv, const QString &duree, const QString &etatsserv, double coutserv);

    // Method to display services
    QSqlQueryModel* afficher();

    // Method to delete a service by ID
    bool supprimer(int idserv);

    // Method to modify an existing service
    bool modifier(int idserv);

    // Method to search for a service by ID
    QSqlQueryModel* rechercher(int idrech);

    // Method to sort services based on a given criteria and order
    QSqlQueryModel* trier(const QString& critere, const QString& ordre);

    // Method to generate a PDF report of the services
    void genererPDF();

    // Method to get statistics on the number of services by state
    QMap<QString, int> statistiquesParEtats();
};

#endif // SERVICE_H
*/
#ifndef SERVICE_H
#define SERVICE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QObject>

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

    QMap<QString, int> statistiquesParEtats();
    QSqlQueryModel* trier(const QString& critere, const QString& ordre);

    void genererPDF();

    void onSendMessage();

};

#endif // SERVICE_H
