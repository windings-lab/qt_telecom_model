#include "TelecomTreeModel.h"

#include <QStandardItemModel>
#include <QFont>
#include <QIcon>

#include "Database/IDataRepository.h"

TelecomTreeModel::TelecomTreeModel() : model(std::make_unique<QStandardItemModel>())
{
}

QStandardItemModel* TelecomTreeModel::buildModel(const QList<Country>& countries) const
{
	model->setHorizontalHeaderLabels({"Location"});

	for (const Country& country : countries) {
		QString countryStr = QString("%1 (%2)").arg(country.name, country.code);
		QString countryIconPath = QString("icons/Countries/%1.png").arg(country.code);

		auto* countryItem = new QStandardItem(countryStr);

		countryItem->setIcon(QIcon(countryIconPath));

		QFont countryFont = countryItem->font();
		countryFont.setBold(true);
		countryItem->setFont(countryFont);

		countryItem->setFlags(countryItem->flags() & ~Qt::ItemIsEditable);

		for (const CountryData& data : country.data) {
			for (const Operator& op : data.operators) {
				QString opStr = QString("%1 (%2-%3)").arg(op.name).arg(data.mcc).arg(op.mnc);
				QString opIconPath = QString("icons/Operators/%1_%2.png").arg(data.mcc).arg(op.mnc);

				auto* opItem = new QStandardItem(opStr);

				opItem->setIcon(QIcon(opIconPath));

				opItem->setFlags(opItem->flags() & ~Qt::ItemIsEditable);

				countryItem->appendRow(opItem);
			}
		}

		model->appendRow(countryItem);
	}

	return model.get();
}
