#include "launchermodel.h"

#include <QSettings>
#include <QDebug>
#include <QDir>
#include <QUuid>

LauncherModel::LauncherModel(QObject *parent) :
    QAbstractListModel(parent), entriesDirPath(QDir::homePath() + "/.local/share/applications")
{

    QDir dir(entriesDirPath);
    dir.setFilter(QDir::Files);

    QStringList filters;
    filters << "*.desktop";
    dir.setNameFilters(filters);

    qDebug() << "Looking entries at: " << entriesDirPath;
    for (QString fileName: dir.entryList()) {
        const QString filePath = dir.filePath(fileName);
        qDebug() << "Scaning entry: " << filePath;
        QSettings settings(filePath, QSettings::IniFormat);

        settings.beginGroup("Desktop Entry");
        QStringList keys = settings.allKeys();


        if (keys.contains("IceUtility")) {
            entriesPaths << dir.filePath(filePath);
            entries[filePath] = loadFromFile(dir.filePath(filePath));
        }
    }
}


QHash<int, QByteArray> LauncherModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(Qt::DisplayRole, "display");
    roles.insert(Qt::DecorationRole, "decoration");
    roles.insert(Name, "name");
    roles.insert(Icon, "icon");
    roles.insert(Categories, "categories");
    roles.insert(Runner, "runner");
    roles.insert(Url, "url");
    roles.insert(Comment, "comment");
    return roles;
}

QVariantMap LauncherModel::get(int row) {
    QHash<int, QByteArray> names = roleNames();
    QHashIterator<int, QByteArray> i(names);
    QVariantMap res;
    while (i.hasNext()) {
        i.next();
        QModelIndex idx = index(row, 0);
        QVariant data = idx.data(i.key());
        res[i.value()] = data;
        qDebug() << i.key() << ": " << i.value() << endl;
    }
    return res;
}
QStringList LauncherModel::runners() const {
    QStringList runners;
    runners << "firefox";
    runners << "chromiun";
    return runners;
}

int LauncherModel::rowCount(const QModelIndex& index) const {
    return index.isValid() ? 0 : entriesPaths.size();
}

int LauncherModel::count() const {
    return entriesPaths.size();
}

QVariant LauncherModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.parent().isValid() ||
        index.column() > 0 || index.row() < 0 || index.row() >= entries.size()) {
        // index requested must be valid, but we have no child items!
        return QVariant();
    }


    QString filePath = entries.keys().at(index.row());
    QVariantMap entry = entries[filePath];


    if (role == Qt::DisplayRole || role == Name)
        return entry["Name"];
    else if (role == Icon)
        return entry["Icon"];
    else if (role == Categories)
        return entry["Categories"];
    else if (role == Runner)
        return entry["Runner"];
    else if (role == Url)
        return entry["Url"];
    else if (role == Comment)
        return entry["Comment"];

    return QVariant();
}

bool LauncherModel::setData(const QModelIndex & index, const QVariant & value, int role) {
    if (!index.isValid() || index.parent().isValid() ||
        index.column() > 0 || index.row() < 0 || index.row() >= entries.size()) {
        // index requested must be valid, but we have no child items!
        return false;
    }

    QString filePath = entries.keys().at(index.row());
    QVariantMap properties = entries[filePath];

    if (role == Qt::EditRole || role == Name)
        properties["Name"] = value;
    else if (role == Icon)
        properties["Icon"] = value;
    else if (role == Categories)
        properties["Categories"] = value;
    else if (role == Runner) {
        properties["Runner"] = value;

        QString runner = properties["Runner"].toString();
        QString url = properties["Url"].toString();
        properties["Exec"] = generateExec(runner, url);
    }
    else if (role == Url) {
        properties["Url"] = value;

        QString runner = properties["Runner"].toString();
        QString url = properties["Url"].toString();
        properties["Exec"] = generateExec(runner, url);
    }
    else if (role == Comment)
        properties["Comment"] = value;

    saveToFile(filePath, properties);
    emit dataChanged(index, index);
}

void LauncherModel::create(QString name, QString icon, QString categories, QString runner, QString url, QString comment) {
    QString fileName = QUuid::createUuid().toString().toLower().remove('{').remove('}');
    QString filePath = entriesDirPath + "/" + fileName + ".desktop";
    qDebug() << "Creating new object " << filePath;
    QVariantMap properties;
    properties["Name"] = name;
    properties["Icon"] = icon;
    properties["Categories"] = categories;
    properties["Runner"] = runner;
    properties["Url"] = url;
    properties["Comment"] = comment;

    properties["Exec"] = generateExec(runner, url);
    properties["IceUtility"] = true;

    properties["Type"] = "Application";



    saveToFile(filePath, properties);

    int last = entriesPaths.size();
    beginInsertRows(QModelIndex(), 0, 0);

    entries[filePath] = properties;
    entriesPaths << filePath;

    endInsertRows();
    emit countChanged();
}

void LauncherModel::update(int index, QString name, QString icon, QString categories, QString runner, QString url, QString comment) {
    if (index < 0 || index >= entriesPaths.size())
        return;

    QString filePath = entriesPaths[index];
    QVariantMap properties = entries[filePath];

    properties["Name"] = name;
    properties["Icon"] = icon;
    properties["Categories"] = categories;
    properties["Runner"] = runner;
    properties["Url"] = url;
    properties["Comment"] = comment;

    properties["Exec"] = generateExec(runner, url);
    properties["IceUtility"] = true;

    saveToFile(filePath, properties);
}

void LauncherModel::remove(int index) {
    if (index < 0 || index >= entriesPaths.size())
        return;

    QString filePath = entriesPaths[index];

    beginRemoveRows(QModelIndex(), index, index);
    entriesPaths.removeOne(filePath);
    entries.remove(filePath);
    endRemoveRows();

    QFile file(filePath);
    file.remove();


    emit countChanged();
}

QString LauncherModel::generateExec(QString runner, QString url) {
    if (runner.compare("firefox") == 0)
        return QString("firefox %1").arg(url);
    else if (runner.compare("chromiun") == 0)
        return QString("chromium --kiosk %1").arg(url);

    return "";
}

void LauncherModel::saveToFile(QString path, QVariantMap properties) {
    qDebug() << "Saving entry: " << path;
    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup("Desktop Entry");

    for (QString key: properties.keys())
        settings.setValue(key,properties[key]);

    settings.endGroup();
}

QVariantMap LauncherModel::loadFromFile(QString path) {
    qDebug() << "Loading entry: " << path;
    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup("Desktop Entry");

    QVariantMap properties;
    for (QString key: settings.allKeys())
        properties[key] = settings.value(key);

    return properties;
}


