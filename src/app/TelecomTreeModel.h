#pragma once

#include <memory>

#include "Database/IDataRepository.h"

class QStandardItemModel;

class TelecomTreeModel {
public:
	TelecomTreeModel();

	QStandardItemModel* buildModel(const QList<Country>& countries) const;

private:
	std::unique_ptr<QStandardItemModel> model;
};
