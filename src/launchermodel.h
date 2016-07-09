#ifndef LAUNCHERMODEL_H
#define LAUNCHERMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QVariantMap>
#include <QByteArray>
#include <QHash>
#include <QMap>


class LauncherModel : public QAbstractListModel
{
    Q_OBJECT

    /**
     * @property Array The list of all allowed runners
     */
    Q_PROPERTY(QStringList runners READ runners)

    Q_PROPERTY(QString iconsCacheDirPath MEMBER m_iconsCacheDirPath READ iconsCacheDirPath)
    Q_PROPERTY(QString entriesDirPath MEMBER m_entriesDirPath READ entriesDirPath)

    /**
     * @property int The number of rows of the model
     */
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    /**
     * @enum Roles of the model, they will be accessible from delegates
     */
    enum Roles {
        Name = Qt::UserRole + 1,
        Icon,
        Categories,
        Runner,
        Url,
        Comment,
        UUID
    };

    LauncherModel(QObject *parent = 0);
    QHash< int, QByteArray > roleNames() const Q_DECL_OVERRIDE;

    Q_INVOKABLE QVariantMap get(int row);

    QStringList runners() const;
    QString iconsCacheDirPath() const;
    QString entriesDirPath() const;

    int rowCount(const QModelIndex&) const Q_DECL_OVERRIDE;
    int count() const;
    QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) Q_DECL_OVERRIDE;


signals:
    void countChanged();

public slots:
    void create(QString name, QString icon, QString categories, QString runner, QString url, QString comment);
    void remove(int index);

private:
    void saveToFile(QString path, QVariantMap properties);
    QVariantMap loadFromFile(QString path);

    QString generateExec(QString runner, QString url);

    QStringList entriesPaths;
    QMap<QString, QVariantMap> entries;

    QString m_iconsCacheDirPath;
    QString m_entriesDirPath;
    const QString CUSTOM_GROUP_NAME;
    const QStringList CUSTOM_KEYS;
};

#endif // LAUNCHERMODEL_H
