#pragma once

#include <QSqlDatabase>

#include "models.h"


class IDataRepository {
public:
	virtual QList<Country> loadCountries() = 0;

	virtual ~IDataRepository();
};

class SqlRepositoryBase : public IDataRepository {
public:
	SqlRepositoryBase() = delete;
	virtual ~SqlRepositoryBase() override;

protected:
	explicit SqlRepositoryBase(const QString& db_name, const QString& db_type);

protected:
	QSqlDatabase m_Database;

private:
	QString m_Type;
};