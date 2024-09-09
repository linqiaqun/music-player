#include "database.h"

#include <QDateTime>
#include <QDebug>
#include <QGlobalStatic>
#include <QMutex>
#include <QMutexLocker>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QUuid>

Q_GLOBAL_STATIC(Database, database)

Database::Database() { m_mutex = std::make_unique<QMutex>(); }

Database::~Database() {}

Database *Database::instance() { return database; }

bool Database::init(const QString &path) {
    QMutexLocker locker(m_mutex.get());
    if (!m_name.isEmpty()) {
        qWarning() << "database initialized";
        return true;
    }

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    m_db.open();
    if (m_db.isOpen()) m_name = path;
    return m_db.isOpen();
}

QVariantMap Database::addCategory(const QString &name) {
    QVariantMap map;
    map.insert("id", QUuid::createUuid().toString(QUuid::Id128));
    map.insert("name", name);
    map.insert("cover", ":/images/category.png");
    map.insert("created", currentDateTime());

    QSqlQuery query(m_db);
    query.prepare(
        "INSERT INTO category (id, name, cover, created) VALUES (:id, :name, :cover, :created)");
    query.bindValue(":id", map["id"]);
    query.bindValue(":name", name);
    query.bindValue(":cover", map["cover"]);
    query.bindValue(":created", map["created"]);
    bool ret = query.exec();
    if (ret) return map;

    qCritical() << query.lastError().text();
    return QVariantMap();
}

bool Database::removeCategory(const QString &id) {
    if (!m_db.transaction()) {
        qCritical() << "transaction failed: " << m_db.lastError().text();
        return false;
    }

    // delete playlist record
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM playlist WHERE song_id IN (SELECT id FROM song WHERE category_id=:category_id)");
    query.bindValue(":category_id", id);
    if (!query.exec()) qCritical() << "playlist" << query.lastError().text();

    // delete song record
    query.prepare("DELETE FROM song WHERE category_id=:category_id");
    query.bindValue(":category_id", id);
    if (!query.exec()) qCritical() << "song" << query.lastError().text();

    // delete category record
    query.prepare("DELETE FROM category WHERE id=:id");
    query.bindValue(":id", id);
    if (!query.exec()) qCritical() << "category" << query.lastError().text();

    if (!m_db.commit()) {
        qCritical() << "commit failed: " << m_db.lastError().text();
        if (!m_db.rollback()) qCritical() << "rollback failed: " << m_db.lastError().text();
        return false;
    }
    return true;
}

bool Database::updateCategory(const QVariantMap &map) {
    QSqlQuery query(m_db);
    query.prepare("UPDATE category SET name=:name, cover=:cover WHERE id=:id");
    query.bindValue(":id", map["id"]);
    query.bindValue(":name", map["name"].toString());
    query.bindValue(":cover", map["cover"]);
    bool ret = query.exec();
    if (!ret) qCritical() << query.lastError().text();
    return ret;
}

QVariantList Database::getCategoryList() {
    QSqlQuery query(m_db);
    QString sql = "SELECT * FROM category";
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

bool Database::addSongList(const QString &categoryID, const QVariantList &list) {
    if (!m_db.transaction()) {
        qCritical() << "transaction failed: " << m_db.lastError().text();
        return false;
    }

    QSqlQuery query(m_db);
    for (auto &item : list) {
        QVariantMap map = item.toMap();
        query.prepare(
            "INSERT INTO song (id, category_id, path, album, artist, comment, duration, genre, "
            "title, track, year, favorite, created) VALUES (:id, :category_id, :path, :album, "
            ":artist, :comment, :duration, :genre, :title, :track, :year, :favorite, :created)");
        query.bindValue(":id", QUuid::createUuid().toString(QUuid::Id128));
        query.bindValue(":category_id", categoryID);
        query.bindValue(":path", map["path"]);
        query.bindValue(":album", map["album"]);
        query.bindValue(":artist", map["artist"]);
        query.bindValue(":comment", map["comment"]);
        query.bindValue(":duration", map["duration"]);
        query.bindValue(":genre", map["genre"]);
        query.bindValue(":title", map["title"]);
        query.bindValue(":track", map["track"]);
        query.bindValue(":year", map["year"]);
        query.bindValue(":favorite", map["favorite"]);
        query.bindValue(":created", currentDateTime());
        query.exec();
    }
    if (!m_db.commit()) {
        qCritical() << "commit failed: " << m_db.lastError().text();
        if (!m_db.rollback()) qCritical() << "rollback failed: " << m_db.lastError().text();
        return false;
    }
    return true;
}

bool Database::clearSongList(const QString &categoryID) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM song WHERE category_id=:id");
    query.bindValue(":id", categoryID);
    bool ret = query.exec();
    if (!ret) qCritical() << query.lastError().text();
    return ret;
}

bool Database::removeSong(const QString &ID) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM song WHERE id=:id");
    query.bindValue(":id", ID);
    bool ret = query.exec();
    if (!ret) qCritical() << query.lastError().text();
    return ret;
}

bool Database::updateSong(const QVariantMap &map) {
    QString sql =
        QString(
            "UPDATE song SET album='%1', artist='%2', comment='%3', genre='%4', title='%5', "
            "track=%6, year=%7, favorite=%8 WHERE id='%9' AND category_id='%10'")
            .arg(map["album"].toString())
            .arg(map["artist"].toString())
            .arg(map["comment"].toString())
            .arg(map["genre"].toString())
            .arg(map["title"].toString())
            .arg(map["track"].toInt())
            .arg(map["year"].toInt())
            .arg(map["favorite"].toInt())
            .arg(map["id"].toString())
            .arg(map["category_id"].toString());
    QSqlQuery query(m_db);
    bool ret = query.exec(sql);
    if (!ret) qCritical() << query.lastError().text();
    return ret;
}

QVariantList Database::getSongList(const QString &categoryID) {
    QSqlQuery query(m_db);
    QString sql = "SELECT * FROM song";
    if (!categoryID.isEmpty()) sql += " WHERE category_id=\"" + categoryID + "\"";
    if (!query.exec(sql)) {
        qCritical() << query.lastError();
        return QVariantList();
    }

    QVariantList list;
    while (query.next()) {
        QVariantMap map;
        map.insert("id", query.value("id"));
        map.insert("category_id", query.value("category_id"));
        map.insert("path", query.value("path"));
        map.insert("album", query.value("album"));
        map.insert("artist", query.value("artist"));
        map.insert("comment", query.value("comment"));
        map.insert("duration", query.value("duration"));
        map.insert("genre", query.value("genre"));
        map.insert("title", query.value("title"));
        map.insert("track", query.value("track"));
        map.insert("year", query.value("year"));
        map.insert("favorite", query.value("favorite"));
        map.insert("created", query.value("created"));
        list.append(map);
    }
    return list;
}

QVariantList Database::getFavorite() {
    QSqlQuery query(m_db);
    QString sql = QString("SELECT * FROM song WHERE favorite=1");
    query.prepare(sql);
    if (!query.exec()) {
        qCritical() << query.lastError();
        return QVariantList();
    }

    QVariantList list;
    while (query.next()) {
        QVariantMap map;
        map.insert("id", query.value("id"));
        map.insert("category_id", query.value("category_id"));
        map.insert("path", query.value("path"));
        map.insert("album", query.value("album"));
        map.insert("artist", query.value("artist"));
        map.insert("comment", query.value("comment"));
        map.insert("duration", query.value("duration"));
        map.insert("genre", query.value("genre"));
        map.insert("title", query.value("title"));
        map.insert("track", query.value("track"));
        map.insert("year", query.value("year"));
        map.insert("favorite", query.value("favorite"));
        map.insert("created", query.value("created"));
        list.append(map);
    }
    return list;
}

bool Database::setPlaylist(const QVariantList &list) {
    if (!m_db.transaction()) {
        qCritical() << "transaction failed: " << m_db.lastError().text();
        return false;
    }

    QSqlQuery query(m_db);
    if (!query.exec("DELETE FROM playlist")) {
        qCritical() << "clear table playlist failed: " << query.lastError().text();
    }

    for (auto i = 0; i < list.count(); ++i) {
        QVariantMap map = list[i].toMap();
        query.prepare("INSERT INTO playlist (id, song_id, idx) VALUES (:id, :song_id, :index)");
        query.bindValue(":id", QUuid::createUuid().toString(QUuid::Id128));
        query.bindValue(":song_id", map["id"]);
        query.bindValue(":index", i);
        if (!query.exec()) {
            qCritical() << query.lastError().text();
        }
    }
    if (!m_db.commit()) {
        qCritical() << "commit failed: " << m_db.lastError().text();
        if (!m_db.rollback()) qCritical() << "rollback failed: " << m_db.lastError().text();
        return false;
    }
    return true;
}

bool Database::clearPlaylist() {
    QSqlQuery query(m_db);
    bool ret = query.exec("DELETE FROM playlist");
    if (!ret) qCritical() << "remove playlist failed: " << query.lastError().text();
    return ret;
}

QVariantList Database::getPlaylist() {
    QSqlQuery query(m_db);
    QString sql = "SELECT * FROM song JOIN playlist ON song.id=playlist.song_id";
    if (!query.exec(sql)) {
        qCritical() << query.lastError();
        return QVariantList();
    }

    QVariantList list;
    while (query.next()) {
        QVariantMap map;
        map.insert("id", query.value("id"));
        map.insert("category_id", query.value("category_id"));
        map.insert("path", query.value("path"));
        map.insert("album", query.value("album"));
        map.insert("artist", query.value("artist"));
        map.insert("comment", query.value("comment"));
        map.insert("duration", query.value("duration"));
        map.insert("genre", query.value("genre"));
        map.insert("title", query.value("title"));
        map.insert("track", query.value("track"));
        map.insert("year", query.value("year"));
        map.insert("favorite", query.value("favorite"));
        map.insert("created", query.value("created"));
        list.append(map);
    }
    return list;
}

QString Database::currentDateTime() {
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}
