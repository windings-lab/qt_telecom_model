#include <QApplication>
#include <QTreeView>
#include <QStandardItemModel>

#include "app/TelecomTreeModel.h"
#include "app/utility.h"
#include "app/Database/SQLiteRepository.h"

class IDataRepository;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    std::unique_ptr<IDataRepository> repository = std::make_unique<SqliteRepository>("OM_system.db");
    QList<Country> countries = repository->loadCountries();

    QTreeView treeView;
    treeView.setStyleSheet(loadStyleSheet("style/treeview_style.qss"));
    treeView.setWindowTitle("Telecom Tree");
    treeView.showMaximized();

    const TelecomTreeModel telecomTreeModel;
    QStandardItemModel* model = telecomTreeModel.buildModel(countries);
    treeView.setModel(model);

    repository.reset();
    countries.clear();
    return app.exec();
}
