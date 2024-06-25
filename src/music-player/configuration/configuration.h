#pragma once

#include <QReadWriteLock>
#include <QVariant>

#define CONFIG Configuration::instance()

class Configuration {
public:
    Configuration() = default;
    virtual ~Configuration() = default;

    static Configuration *instance();

    bool readFile(const QString &path, bool allowNotExist = false);
    bool saveFile();

    QVariant value(const QString &key);
    bool setValue(const QString &key, const QVariant &variant);

    QVariant tempValue(const QString &key) const;
    void setTempValue(const QString &key, const QVariant &variant);

protected:
    QString m_path = "";
    QVariantMap m_config;
    QVariantMap m_tempConfig;
    QReadWriteLock m_lock;

private:
    QVariant json2variant(const QByteArray &json);
    QByteArray variant2json(const QVariant &variant);
};
