#ifndef VEHICULE_H
#define VEHICULE_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "qrcodegen.hpp"

using namespace qrcodegen;

class vehicule
{
private:
    int nums, numc, numtel;
    QDate datea;
    float km;
    QString model, marque, nom;

public:
    vehicule();
    vehicule(int Nums, int Numc, int NumTel, QDate Datea, float Km, QString Model, QString Marque, QString Nom);

    // Getters
    int getNums();
    int getNumc();
    int getNumTel();
    QDate getDate();
    float getKm();
    QString getModel();
    QString getMarque();
    QString getNom();

    // Setters
    void setNums(int nums);
    void setNumc(int numc);
    void setNumTel(int numtel);
    void setDate(QDate date);
    void setKm(float km);
    void setModel(QString model);
    void setMarque(QString marque);
    void setNom(QString nom);

    bool Ajouter();
    QSqlQueryModel* afficher();
    bool Supprimer(int nums);
    bool modifier();

    /*Recherche*/
    QSqlQueryModel *afficher_numc(QString ch);
    QSqlQueryModel *afficher_nums(QString ch);
    QSqlQueryModel *afficher_model(QString ch);
    /*Tri*/
    QSqlQueryModel *afficher_choix(QString choix);
    /*Statistique*/
    int statistique1();
    int statistique2();
    /*PDF*/
    vehicule* readvehicule (int val);

};

#endif // VEHICULE_H
