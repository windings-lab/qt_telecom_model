#include "SQLiteRepository.h"

#include <QSqlQuery>
#include <QMap>

SqliteRepository::SqliteRepository(const QString& dbFilePath)
    : IDataRepository(dbFilePath, "QSQLITE")
{
}

QList<Country> SqliteRepository::loadCountries()
{
    // Loading operators
    QMap<int, QList<Operator>> operatorMap;
    QSqlQuery operatorQuery(m_Database);
    operatorQuery.prepare(R"(
        SELECT mcc, mnc, name FROM operators ORDER BY CAST(mnc AS INTEGER)
    )");
    if (operatorQuery.exec()) {
        while (operatorQuery.next()) {
            const int mcc = operatorQuery.value(0).toInt();
            const int mnc = operatorQuery.value(1).toInt();
            const QString name = operatorQuery.value(2).toString();

            operatorMap[mcc].append({mcc, mnc, name});
        }
    }

    // Loading country data
    QMap<QString, QList<CountryData>> countryDataMap;
    QSqlQuery dataQuery(m_Database);
    dataQuery.prepare(R"(
        SELECT mcc, code, name, mnc_length FROM countries ORDER BY code, mcc
    )");
    if (dataQuery.exec()) {
        while (dataQuery.next()) {
            const int mcc = dataQuery.value(0).toInt();
            const QString code = dataQuery.value(1).toString();
            const int mnc_length = dataQuery.value(3).toInt();

            countryDataMap[code].append({mcc, mnc_length, operatorMap.value(mcc)});
        }
    }

    // Loading countries
    QList<Country> countries;
    QSqlQuery countryQuery(m_Database);
    countryQuery.prepare(R"(
        SELECT MIN(mcc) AS mcc, code, MIN(name) AS name, MIN(mnc_length) AS mnc_length
        FROM countries
        GROUP BY code
        ORDER BY name ASC;
    )");
    if (countryQuery.exec()) {
        while (countryQuery.next()) {
            const QString code = countryQuery.value(1).toString();
            const QString name = countryQuery.value(2).toString();

            countries.append({code, name, countryDataMap.value(code)});
        }
    }

    return countries;
}
