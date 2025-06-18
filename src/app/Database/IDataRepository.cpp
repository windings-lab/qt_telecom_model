#include "IDataRepository.h"

#include <QSqlError>


SqlRepositoryBase::SqlRepositoryBase(const QString& db_name, const QString& db_type)
	: m_Type(db_type)
{
	m_Database = QSqlDatabase::addDatabase(m_Type);
	m_Database.setDatabaseName(db_name);

	if (!m_Database.open()) {
		qWarning() << "Failed to open DB:" << m_Database.lastError().text();
	}
}

IDataRepository::~IDataRepository()
{
}

SqlRepositoryBase::~SqlRepositoryBase()
{
	m_Database.close();
	QSqlDatabase::removeDatabase(m_Database.connectionName());
}
