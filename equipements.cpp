#include "equipements.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts>
#include <QString>
#include <QPrinter>


// Default constructor
Equipement::Equipement() : id(0), nom(""), modele(""), etat(""), dateInstallation(QDate::currentDate()), emplacement("") {}

// Parameterized constructor
Equipement::Equipement(int id, const QString &nom, const QString &modele, const QString &etat, const QDate &dateInstallation, const QString &emplacement, QString image)
    : id(id), nom(nom), modele(modele), etat(etat), dateInstallation(dateInstallation), emplacement(emplacement), image(image) {}

// Getters
int Equipement::getId() const { return id; }
QString Equipement::getNom() const { return nom; }
QString Equipement::getModele() const { return modele; }
QString Equipement::getEtat() const { return etat; }
QDate Equipement::getDateInstallation() const { return dateInstallation; }
QString Equipement::getEmplacement() const { return emplacement; }

// Setters
void Equipement::setId(int id) { this->id = id; }
void Equipement::setNom(const QString &nom) { this->nom = nom; }
void Equipement::setModele(const QString &modele) { this->modele = modele; }
void Equipement::setEtat(const QString &etat) { this->etat = etat; }
void Equipement::setDateInstallation(const QDate &dateInstallation) { this->dateInstallation = dateInstallation; }
void Equipement::setEmplacement(const QString &emplacement) { this->emplacement = emplacement; }

// Method to add equipment to the database
bool Equipement::ajouter() {
    QSqlQuery query;

    // Prepare the SQL query for inserting equipment into the database
    query.prepare("INSERT INTO GESTION_EQUIPEMENTS (ID, NOM, MODELE, ETAT, DATEINSTALLATION, EMPLACEMENT, IMAGE) "
                  "VALUES (:id, :nom, :modele, :etat, :dateInstallation, :emplacement, :image)");

    // Bind the non-image fields to the query
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":modele", modele);
    query.bindValue(":etat", etat);
    query.bindValue(":dateInstallation", dateInstallation);
    query.bindValue(":emplacement", emplacement);


    query.bindValue(":image", image);  // 'image' is assumed to be a QByteArray

    // Execute the query
    if (!query.exec()) {
        qDebug() << "Failed to add equipment:" << query.lastError().text();
        return false;
    }

    return true;
}


// Method to display equipment
QSqlQueryModel* Equipement::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM GESTION_EQUIPEMENTS");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Modèle"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date d'installation"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Emplacement"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Image"));
    return model;
}

// Method to delete equipment by ID
bool Equipement::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM GESTION_EQUIPEMENTS WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Failed to delete equipment:" << query.lastError().text();
        return false;
    }
    return true;
}

// Method to modify equipment details
bool Equipement::modifier(int id, const QString &nom, const QString &modele, const QString &etat, const QDate &dateInstallation, const QString &emplacement,QString image) {
    QSqlQuery query;
    query.prepare("UPDATE GESTION_EQUIPEMENTS SET NOM = :nom, MODELE = :modele, ETAT = :etat, DATEINSTALLATION = :dateInstallation, EMPLACEMENT = :emplacement,IMAGE= :image WHERE ID = :id");
    query.bindValue(":nom", nom);
    query.bindValue(":modele", modele);
    query.bindValue(":etat", etat);
    query.bindValue(":dateInstallation", dateInstallation);
    query.bindValue(":emplacement", emplacement);
    query.bindValue(":id", id);
    query.bindValue(":image", image);


}
QSqlQueryModel* Equipement::rechercher(QString searchType, QString value)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    // Determine the search column based on the searchType parameter
    QString columnName;
    if (searchType == "nom") {
        columnName = "NOM";
    } else if (searchType == "modele") {
        columnName = "MODELE";
    }  else if (searchType == "id") {
        columnName = "ID";
    }else if (searchType == "etat") {
        columnName = "ETAT";
    } else if (searchType == "dateInstallation") {
        columnName = "DATEINSTALLATION";
    } else if (searchType == "emplacement") {
        columnName = "EMPLACEMENT";
    } else {
        qDebug() << "Invalid search type provided.";
        return model; // Return an empty model if invalid search type
    }

    // Prepare the query
    query.prepare(QString("SELECT ID, NOM, MODELE, ETAT, DATEINSTALLATION, EMPLACEMENT, IMAGE "
                          "FROM GESTION_EQUIPEMENTS WHERE %1 LIKE :value").arg(columnName));
    query.bindValue(":value", "%" + value + "%");

    if (query.exec()) {
        model->setQuery(query);
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Modèle"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("État"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date d'installation"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Emplacement"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Image"));
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

    return model;
}
QSqlQueryModel* Equipement::tri(const QString sortColumn)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString queryStr;

    // Construct the SQL query based on the sort column
    if (sortColumn == "id") {
        queryStr = "SELECT * FROM GESTION_EQUIPEMENTS ORDER BY ID";  // Sorting by price
    } else if (sortColumn == "nom") {
        queryStr = "SELECT * FROM GESTION_EQUIPEMENTS ORDER BY NOM";  // Sorting by reference
    } else if (sortColumn == "etat") {
        queryStr = "SELECT * FROM GESTION_EQUIPEMENTS ORDER BY ETAT";  // Sorting by SDR (Stock de Réapprovisionnement)
    } else if(sortColumn == "modele"){
        queryStr = QString("SELECT * FROM GESTION_EQUIPEMENTS ORDER BY MODELE");  // General sorting for other columns
    }

    QSqlQuery query(queryStr);
    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Modèle"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date d'installation"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Emplacement"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Image"));
    // Return the modelui with sorted data
    return model;
}

QChartView* Equipement::createPieChartModel()
   {
       // Initialize cost categories counters
       int modelRange1 = 0; // Atelier de fabrication
       int modelRange2 = 0; // Salle de maintenance
       int modelRange3 = 0; //Salle des compresseurs

       QPieSeries *series = new QPieSeries();

       // Query to get all costs from the GS_SERVICE table
       QSqlQuery query("SELECT EMPLACEMENT FROM GESTION_EQUIPEMENTS");

       // Loop through the results to categorize costs
       while (query.next()) {
           QString place = query.value(0).toString();

           if (place == "Atelier de fabrication") {
               modelRange1++;
           } else if (place == "Salle de maintenance") {
               modelRange2++;
           } else  {
               modelRange3++;
           }
       }

       // Add each cost range as a slice in the pie chart
       if (modelRange1 > 0) {
           QPieSlice *slice1 = series->append("0-50", modelRange1);
           slice1->setLabel(QString("Atelier de fabrication: %1").arg(modelRange1));
       }

       if (modelRange2 > 0) {
           QPieSlice *slice2 = series->append("50-100", modelRange2);
           slice2->setLabel(QString("50-100: %1").arg(modelRange2));
       }

       if (modelRange3 > 0) {
           QPieSlice *slice3 = series->append("100-200", modelRange3);
           slice3->setLabel(QString("100-200: %1").arg(modelRange3));
       }



       // Create and set up the chart
       QChart *chart = new QChart();
       chart->addSeries(series);
       chart->setTitle("Distribution des Coûts");
       series->setLabelsVisible();

       // Set up chart view
       QChartView *chartView = new QChartView(chart);
       chartView->setRenderHint(QPainter::Antialiasing);
       chartView->chart()->setAnimationOptions(QChart::AllAnimations);
       chartView->chart()->legend()->hide();

       return chartView;
   }

QChartView* Equipement::etat1()
   {
       // Initialize counters for each type category (a1, a2, a3)
       int a1 = 0;//Actif
       int a2 = 0;//Defaillant
       int a3 = 0;//En revision
    int a4 = 0;//Hors service

       // Query to select all the types (type) from the produit table
       QSqlQuery query("SELECT ETAT FROM GESTION_EQUIPEMENTS");

       // Process the query result
       while(query.next())
       {
           QString type = query.value(0).toString();

           // Increment the respective counter based on type category
           if (type == "Actif") {
               a1++;
           } else if (type == "Defaillant") {
               a2++;
           } else if (type == "En revision") {
               a3++;
           }
        else if (type == "Hors service") {
           a4++;
       }
       }

       // Create a bar set with custom colors for each bar
       QBarSet *typeSet = new QBarSet("Types");

       // Set the values for each category
       *typeSet << a1 << a2 << a3 << a4;

       // Set custom colors for the bars
       typeSet->setColor(QColor("#4B1F1F"));   // Color for "a1"
       typeSet->setColor(QColor("#5C2A2A"));   // Color for "a2"
       typeSet->setColor(QColor("#6E3636"));   // Color for "a3"

       // Create a bar series and add the set to it
       QBarSeries *series = new QBarSeries();
       series->append(typeSet);

       // Create a chart and add the series to it
       QChart *chart = new QChart();
       chart->addSeries(series);
       chart->setTitle("Statistiques des types des produits");

       // Set up categories for the x-axis
       QStringList categories;
       categories << "Actif" << "Defaillant" << "En revision" << "Hors service";

       QBarCategoryAxis *axis = new QBarCategoryAxis();
       axis->append(categories);
       chart->createDefaultAxes();
       chart->setAxisX(axis, series);

       // Create a chart view and enable animations
       QChartView *chartView = new QChartView(chart);
       chartView->chart()->setAnimationOptions(QChart::AllAnimations);

       return chartView;
   }
void Equipement::exportDataToPDF()
{
    QSqlQuery query;

    // Create a PDF file
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save PDF", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) {
        qDebug() << "No file selected. Aborting PDF generation.";
        return; // Exit if no file is selected
    }

    // Prepare query to retrieve all equipment
    query.prepare("SELECT ID, NOM, MODELE, ETAT, DATEINSTALLATION, EMPLACEMENT FROM GESTION_EQUIPEMENTS");

    // Create a QPrinter object
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QPainter painter;
    if (!painter.begin(&printer)) {
        qDebug() << "Failed to begin painting on printer.";
        return; // Exit if painter fails to start
    }

    // HTML for better styling
    QString html = "<html><head><style>"
                   "body { font-family: Arial, sans-serif; margin: 20px; }"
                   "h1 { text-align: center; }"
                   "table { width: 100%; border-collapse: collapse; margin-top: 20px; }"
                   "th, td { border: 1px solid black; padding: 8px; text-align: center; }"
                   "th { background-color: #f2f2f2; }"
                   "</style></head><body>"
                   "<h1>Equipement Information</h1>"
                   "<table>"
                   "<tr><th>ID</th><th>Name</th><th>Model</th><th>Status</th><th>Installation Date</th><th>Location</th></tr>";

    // Execute the query
    if (query.exec()) {
        while (query.next()) {
            html += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td><td>%6</td></tr>")
                        .arg(query.value("ID").toString())
                        .arg(query.value("NOM").toString())
                        .arg(query.value("MODELE").toString())
                        .arg(query.value("ETAT").toString())
                        .arg(query.value("DATEINSTALLATION").toDate().toString("yyyy-MM-dd"))
                        .arg(query.value("EMPLACEMENT").toString());
        }
    } else {
        qDebug() << "Error retrieving data for GESTION_EQUIPEMENTS table:" << query.lastError().text();
    }

    html += "</table></body></html>"; // Close the HTML tags

    // Create a QTextDocument and set its HTML content
    QTextDocument document;
    document.setHtml(html);

    // Draw the document to the painter
    document.drawContents(&painter);
    painter.end(); // Ensure the painter is ended

    qDebug() << "PDF generated successfully!";
}
