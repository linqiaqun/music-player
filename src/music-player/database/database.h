#pragma once

#include <QSqlDatabase>
#include <QString>
#include <memory>

#define DB Database::instance()

class QMutex;
class Database {
public:
    Database();
    virtual ~Database();
    static Database *instance();
    bool init(const QString &path);

    QVariantMap addCategory(const QString &name);
    bool removeCategory(const QString &id);
    bool updateCategory(const QVariantMap &map);
    QVariantList getCategoryList();

    bool addSongList(const QString &categoryID, const QVariantList &list);
    bool clearSongList(const QString &categoryID);
    bool removeSong(const QString &ID);
    bool updateSong(const QVariantMap &map);
    QVariantList getSongList(const QString &categoryID = "");
    QVariantList getFavorite();

    bool setPlaylist(const QVariantList &list);
    bool clearPlaylist();
    QVariantList getPlaylist();

private:
    QString currentDateTime();

private:
    QSqlDatabase m_db;
    std::unique_ptr<QMutex> m_mutex;
    QString m_name = "";
};
