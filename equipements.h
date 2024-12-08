#ifndef EQUIPEMENTS_H
#define EQUIPEMENTS_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QByteArray>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts>
class Equipement {
private:
    int id;
    QString nom;
    QString modele;
    QString etat;
    QDate dateInstallation;
    QString emplacement;
    QString image;

public:
    // Constructors
    Equipement();
    Equipement(int id, const QString &nom, const QString &modele, const QString &etat, const QDate &dateInstallation, const QString &emplacement, QString image);


    // Getters
    int getId() const;
    QString getNom() const;
    QString getModele() const;
    QString getEtat() const;
    QDate getDateInstallation() const;
    QString getEmplacement() const;

    // Setters
    void setId(int id);
    void setNom(const QString &nom);
    void setModele(const QString &modele);
    void setEtat(const QString &etat);
    void setDateInstallation(const QDate &dateInstallation);
    void setEmplacement(const QString &emplacement);

    // Methods to interact with the database
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id);
   bool modifier(int id, const QString &nom, const QString &modele, const QString &etat, const QDate &dateInstallation, const QString &emplacement,QString image);
QSqlQueryModel* rechercher(QString searchType, QString value);
QSqlQueryModel* tri(const QString sortColumn);
QChartView* createPieChartModel();
void exportDataToPDF();

QChartView* etat1();

};

#endif // EQUIPEMENTS_H
