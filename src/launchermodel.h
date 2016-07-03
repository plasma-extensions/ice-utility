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
        Comment
    };

    LauncherModel(QObject *parent = 0);
    QHash< int, QByteArray > roleNames() const Q_DECL_OVERRIDE;

    QStringList runners() const;

    int rowCount(const QModelIndex&) const Q_DECL_OVERRIDE;
    int count() const;
    QVariant data(const QModelIndex&, int) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) Q_DECL_OVERRIDE;


signals:
    void countChanged();

public slots:
    void create(QString name, QString icon, QString categories, QString, QString runner, QString url, QString comment);
    void update(int index, QString name, QString icon, QString categories, QString, QString runner, QString url, QString comment);
    void remove(int index);

private:
    void saveToFile(QString path, QVariantMap properties);
    QVariantMap loadFromFile(QString path);

    QString generateExec(QString runner, QString url);

    QStringList entriesPaths;
    QString entriesDirPath;
    QMap<QString, QVariantMap> entries;
};

#endif // LAUNCHERMODEL_H
