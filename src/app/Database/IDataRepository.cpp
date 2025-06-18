#include "IDataRepository.h"

#include <QSqlError>


IDataRepository::IDataRepository(const QString& db_name, const QString& db_type) : m_Type(db_type)
{
	m_Database = QSqlDatabase::addDatabase(m_Type);
	m_Database.setDatabaseName(db_name);

	if (!m_Database.open()) {
		qWarning() << "Failed to open DB:" << m_Database.lastError().text();
	}
}

IDataRepository::~IDataRepository()
{
	m_Database.close();
	QSqlDatabase::removeDatabase(m_Database.connectionName());
}