#pragma once

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

    QVariantList getPlaylistList();

private:
    std::unique_ptr<QMutex> m_mutex;
    QString m_name = "";
};
