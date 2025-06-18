#include "utility.h"

#include <QFile>
#include <QTextStream>


QString loadStyleSheet(const QString& filePath)
{
	QFile file(filePath);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		qWarning("Failed to load style sheet file!");
		return "";
	}

	QTextStream stream(&file);
	return stream.readAll();
}
