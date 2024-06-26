#include "database.h"

#include <QDebug>
#include <QGlobalStatic>
#include <QMutex>
#include <QMutexLocker>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

Q_GLOBAL_STATIC(Database, database)

Database::Database() { m_mutex = std::make_unique<QMutex>(); }

Database::~Database() {}

Database* Database::instance() { return database; }

bool Database::init(const QString& path) {
    QMutexLocker locker(m_mutex.get());
    if (!m_name.isEmpty()) {
        qWarning() << "database initialized";
        return true;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    db.open();
    if (db.isOpen()) m_name = path;
    return db.isOpen();
}

QVariantList Database::getPlaylistList() {
    QSqlQuery query(QSqlDatabase::database(m_name));
    QString sql = "select * from playlist";
    query.prepare(sql);
    if (!query.exec()) {
        qCritical() << query.lastError();
        return QVariantList();
    }

    QVariantList list;
    while (query.next()) {
        QVariantMap map;
        map.insert("id", query.value("id"));
        map.insert("name", query.value("name"));
        map.insert("cover", query.value("cover"));
        map.insert("created", query.value("created"));
        list.append(map);
    }
    return list;
}
