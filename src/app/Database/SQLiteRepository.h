#pragma once
#include "IDataRepository.h"



class SqliteRepository : public IDataRepository
{
public:
    explicit SqliteRepository(const QString& dbFilePath);

    virtual QList<Country> loadCountries() override;

private:
    QString m_DBPath;
};
