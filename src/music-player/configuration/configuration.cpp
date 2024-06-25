#include "configuration.h"

#include <QDebug>
#include <QFile>
#include <QGlobalStatic>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QReadLocker>
#include <QWriteLocker>

Q_GLOBAL_STATIC(Configuration, configuration)

Configuration *Configuration::instance() { return configuration; }

bool Configuration::readFile(const QString &path, bool allowNotExist) {
    if (!QFile::exists(path)) {
        if (!allowNotExist) {
            qCritical() << "configuration file not exists: " << path;
            return false;
        }

        m_path = path;
        m_config = QVariantMap();
        return true;
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qCritical() << "open file failed: " << file.errorString();
        return false;
    }

    QByteArray data = file.readAll();
    file.close();
    m_path = path;
    m_config = json2variant(data).toMap();
    return true;
}

bool Configuration::saveFile() {
    if (m_path.isEmpty()) {
        qCritical() << "configuration file's path cannot be empty";
        return false;
    }

    QFile file(m_path);
    if (!file.open(QIODevice::WriteOnly)) {
        qCritical() << "open configuration file failed: " << file.errorString();
        return false;
    }

    file.write(variant2json(m_config));
    file.flush();
    file.close();
    return true;
}

QVariant Configuration::value(const QString &key) {
    QReadLocker locker(&m_lock);
    return m_config[key];
}

bool Configuration::setValue(const QString &key, const QVariant &variant) {
    QWriteLocker locker(&m_lock);
    m_config.insert(key, variant);
    return saveFile();
}

QVariant Configuration::tempValue(const QString &key) const { return m_tempConfig[key]; }

void Configuration::setTempValue(const QString &key, const QVariant &variant) {
    m_tempConfig.insert(key, variant);
}

QVariant Configuration::json2variant(const QByteArray &json) {
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(json, &parseError);
    if (QJsonParseError::NoError != parseError.error) {
        return QVariant();
    }
    return doc.toVariant();
}

QByteArray Configuration::variant2json(const QVariant &variant) {
    QJsonDocument doc = QJsonDocument::fromVariant(variant);
    return doc.toJson();
}
