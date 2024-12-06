#include "RDV.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
// Constructor
rdv::rdv(int id, int jour, int mois, int annee, int heure, const QString &adresse)
    : IDR(id), JOUR(jour), MOIS(mois), ANNEE(annee), HEURE(heure), ADRESSE(adresse) {}

