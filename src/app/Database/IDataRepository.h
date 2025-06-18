#pragma once

#include <QSqlDatabase>

#include "models.h"


class IDataRepository {
public:
	virtual QList<Country> loadCountries() = 0;

public:
	virtual ~IDataRepository();

protected:
	explicit IDataRepository(const QString& db_file_path, const QString& db_type);

	QSqlDatabase m_Database;

private:
	QString m_Type;
};
