#ifndef RDV_H
#define RDV_H

#include <QString>

class rdv {
public:
    // Constructor
    rdv(int id, int jour, int mois, int annee, int heure, const QString &adresse);

    // Getter methods
    int getIDR() const;
    int getJOUR() const;
    int getMOIS() const;
    int getANNEE() const;
    int getHEURE() const;
    QString getADRESSE() const;

    // Database operations
    bool ajouter();
    bool supprimer(int id);
    bool update(int id, int jour, int mois, int annee, int heure, const QString &adresse);

private:
    int IDR;        // Record ID
    int JOUR;      // Day
    int MOIS;      // Month
    int ANNEE;     // Year
    int HEURE;     // Hour
    QString ADRESSE; // Address
};

#endif // RDV_H

