#include <QApplication>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QFile>
#include <QTextStream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QIcon>
#include <memory>

// --- Domain Models ---
struct Operator {
    int mcc;
    int mnc;
    QString name;
};

struct CountryData {
    int mcc;
    int mcc_length;
    QList<Operator> operators;
};

struct Country {
    QString code;
    QString name;
    QList<CountryData> data;
};

// --- Abstract Repository Interface ---
class IDataRepository {
public:
    virtual ~IDataRepository() = default;
    virtual QList<Country> loadCountries() = 0;
};

// --- SQLite Implementation ---
class SqliteRepository : public IDataRepository {
    QString m_dbFilePath;

public:
    explicit SqliteRepository(const QString& dbFilePath) : m_dbFilePath(dbFilePath) {}

    QList<Country> loadCountries() override {
        QList<Country> countries;

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(m_dbFilePath);

        if (!db.open()) {
            qWarning() << "Failed to open DB:" << db.lastError().text();
            return countries;
        }

        QSqlQuery countryQuery(db);
        countryQuery.prepare(R"(
            SELECT MIN(mcc) AS mcc, code, MIN(name) AS name, MIN(mnc_length) AS mnc_length
            FROM countries
            GROUP BY code
            ORDER BY name ASC;
        )");

        if (!countryQuery.exec()) return countries;

        while (countryQuery.next()) {
            QString code = countryQuery.value(1).toString();
            QString countryName = countryQuery.value(2).toString();
            QList<CountryData> data;

            QSqlQuery countryDataQuery(db);
            countryDataQuery.prepare(R"(
                SELECT mcc, code, name, mnc_length FROM countries
                WHERE code = :code
                ORDER BY mcc
            )");
            countryDataQuery.bindValue(":code", code);
            if (!countryDataQuery.exec()) continue;

            while (countryDataQuery.next()) {
                int mcc = countryDataQuery.value(0).toInt();
                int mcc_length = countryDataQuery.value(3).toInt();
                QList<Operator> operators;

                QSqlQuery operatorQuery(db);
                operatorQuery.prepare(R"(
                    SELECT mcc, mnc, name FROM operators
                    WHERE mcc = :mcc
                    ORDER BY CAST(mnc AS INTEGER);
                )");
                operatorQuery.bindValue(":mcc", mcc);
                if (!operatorQuery.exec()) continue;

                while (operatorQuery.next()) {
                    int mnc = operatorQuery.value(1).toInt();
                    QString operatorName = operatorQuery.value(2).toString();
                    operators.append({mcc, mnc, operatorName});
                }

                data.append({mcc, mcc_length, operators});
            }

            countries.append({code, countryName, data});
        }

        db.close();
        return countries;
    }
};

// --- UI Tree Builder ---
class TelecomTreeModel {
public:
    TelecomTreeModel() : model(std::make_unique<QStandardItemModel>()) {}

    QStandardItemModel* buildModel(const QList<Country>& countries) const
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

private:
    std::unique_ptr<QStandardItemModel> model;
};

// --- Utility ---
QString loadStyleSheet(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qWarning("Failed to load style sheet file!");
        return "";
    }

    QTextStream stream(&file);
    return stream.readAll();
}

// --- Main Application ---
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
