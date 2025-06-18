#include "SQLiteRepository.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>

SqliteRepository::SqliteRepository(const QString& dbFilePath)
    : m_DBPath(dbFilePath)
{

}

QList<Country> SqliteRepository::loadCountries()
{
    QList<Country> countries;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_DBPath);

    if (!db.open()) {
        qWarning() << "Failed to open DB:" << db.lastError().text();
        return countries;
    }

    QSqlQuery countryQuery(db);
    countryQuery.prepare(R"(
        SELECT MIN(mcc) AS mcc, code, MIN(name) AS name, MIN(mnc_length) AS mnc_length
        FROM countries
        GROUP BY code
        ORDER BY name ASC;
    )");

    if (!countryQuery.exec()) return countries;

    while (countryQuery.next()) {
        QString code = countryQuery.value(1).toString();
        QString countryName = countryQuery.value(2).toString();
        QList<CountryData> data;

        QSqlQuery countryDataQuery(db);
        countryDataQuery.prepare(R"(
            SELECT mcc, code, name, mnc_length FROM countries
            WHERE code = :code
            ORDER BY mcc
        )");
        countryDataQuery.bindValue(":code", code);
        if (!countryDataQuery.exec()) continue;

        while (countryDataQuery.next()) {
            int mcc = countryDataQuery.value(0).toInt();
            int mcc_length = countryDataQuery.value(3).toInt();
            QList<Operator> operators;

            QSqlQuery operatorQuery(db);
            operatorQuery.prepare(R"(
                SELECT mcc, mnc, name FROM operators
                WHERE mcc = :mcc
                ORDER BY CAST(mnc AS INTEGER);
            )");
            operatorQuery.bindValue(":mcc", mcc);
            if (!operatorQuery.exec()) continue;

            while (operatorQuery.next()) {
                int mnc = operatorQuery.value(1).toInt();
                QString operatorName = operatorQuery.value(2).toString();
                operators.append({mcc, mnc, operatorName});
            }

            data.append({mcc, mcc_length, operators});
        }

        countries.append({code, countryName, data});
    }

    db.close();
    return countries;
}
