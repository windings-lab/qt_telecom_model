#pragma once
#include "IDataRepository.h"


class SqliteRepository final : public IDataRepository
{
public:
    explicit SqliteRepository(const QString& dbFilePath);

    virtual QList<Country> loadCountries() override;
};
