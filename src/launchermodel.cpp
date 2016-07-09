#include "launchermodel.h"

#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <QDir>
#include <QUuid>

#include <KDesktopFile>
#include <KConfigGroup>

LauncherModel::LauncherModel(QObject *parent) :
    QAbstractListModel(parent),
    m_entriesDirPath(QDir::homePath() + "/.local/share/applications"),
    m_iconsCacheDirPath(QDir::homePath() + "/.local/share/" + QApplication::applicationName()),
    CUSTOM_GROUP_NAME("Ice Util"),  CUSTOM_KEYS {"Url", "Runner", "Uuid"}
{
    QDir dir(m_entriesDirPath);
    dir.setFilter(QDir::Files);

    QStringList filters;
    filters << "*.desktop";
    dir.setNameFilters(filters);

    qDebug() << "Looking entries at: " << m_entriesDirPath;
    for (QString fileName: dir.entryList()) {
        const QString filePath = dir.filePath(fileName);
        qDebug() << "Scaning entry: " << filePath;
        KDesktopFile file(filePath);

        if (file.hasGroup(CUSTOM_GROUP_NAME)) {
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
    roles.insert(UUID, "uuid");
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
        //qDebug() << i.key() << ": " << i.value() << data << endl;
    }
    return res;
}
QStringList LauncherModel::runners() const {
    QStringList runners;
    runners << "firefox";
    runners << "chromiun";
    return runners;
}

QString LauncherModel::entriesDirPath() const {
    return m_entriesDirPath;
}

QString LauncherModel::iconsCacheDirPath() const {
    return m_iconsCacheDirPath;
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
    else if (role == UUID)
        return entry["Uuid"];

    return QVariant();
}

bool LauncherModel::setData(const QModelIndex & index, const QVariant & value, int role) {
    if (!index.isValid() || index.parent().isValid() ||
        index.column() > 0 || index.row() < 0 || index.row() >= entries.size()) {
        // index requested must be valid, but we have no child items!
        return false;
    }
     qDebug() << "Setting data " << role << value;

    QString filePath = entries.keys().at(index.row());
    QVariantMap properties = entries[filePath];

    switch(role) {
        case Qt::EditRole:
        case Name:
            properties["Name"] = value;
            break;
        case Icon:
            properties["Icon"] = value;
            break;
        case Categories:
            properties["Categories"] = value;
            break;
        case Runner:
            properties["Runner"] = value;
            break;
        case Url:
            properties["Url"] = value;
            break;
        case Comment:
            properties["Comment"] = value;
            break;
    }
    entries[filePath] = properties;

    QString runner = properties["Runner"].toString();
    QString url = properties["Url"].toString();
    properties["Exec"] = generateExec(runner, url);

    saveToFile(filePath, properties);
    emit dataChanged(index, index);
}

void LauncherModel::create(QString name, QString icon, QString categories, QString runner, QString url, QString comment) {
    QString uuid = QUuid::createUuid().toString().toLower().remove('{').remove('}');
    QString filePath = m_entriesDirPath + "/" + uuid + ".desktop";
    qDebug() << "Creating new object " << filePath;
    QVariantMap properties;
    properties["Name"] = name;
    properties["Icon"] = icon;
    properties["Categories"] = categories;
    properties["Runner"] = runner;
    properties["Url"] = url;
    properties["Comment"] = comment;
    properties["Uuid"] = uuid;

    properties["Exec"] = generateExec(runner, url);

    properties["Type"] = "Application";

    saveToFile(filePath, properties);

    int last = entriesPaths.size();

    for (QString path: entriesPaths)
        qDebug() << path;
    qDebug() << last;
    beginInsertRows(QModelIndex(), last, last);
    entries[filePath] = properties;
    entriesPaths.insert(last, filePath);
    endInsertRows();

    emit countChanged();
}

void LauncherModel::remove(int index) {
    if (index < 0 || index >= entriesPaths.size())
        return;

    QString filePath = entriesPaths[index];
    QString iconCacheDirPath = m_iconsCacheDirPath + "/" + entries[filePath]["Uuid"].toString() + ".png";

    beginRemoveRows(QModelIndex(), index, index);
    entriesPaths.removeOne(filePath);
    entries.remove(filePath);
    endRemoveRows();

    QFile file(filePath);
    file.remove();

    QFile iconFile(iconCacheDirPath);
    iconFile.remove();

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
    KDesktopFile file(path);
    KConfigGroup group = file.desktopGroup();
    KConfigGroup groupIU = file.group(CUSTOM_GROUP_NAME);

    for (QString key: properties.keys())
        if (CUSTOM_KEYS.contains(key))
            groupIU.writeEntry(key,properties[key]);
        else
            group.writeEntry(key,properties[key]);
}

QVariantMap LauncherModel::loadFromFile(QString path) {
    qDebug() << "Loading entry: " << path;
    KDesktopFile file(path);
    KConfigGroup group = file.desktopGroup();
    KConfigGroup groupIU = file.group(CUSTOM_GROUP_NAME);

    QVariantMap properties;
    for (QString key: group.keyList())
            properties[key] = group.readEntry(key);

    for (QString key: groupIU.keyList())
        properties[key] = groupIU.readEntry(key);

    return properties;
}


