#pragma once

#include <QVariant>

class TagLibHelper {
public:
    static QStringList propertyNames();
    static QVariantMap getPropertyList(const QString &path, const QStringList &nameList);
    static bool setPropertyList(const QString &path, const QVariantList &list);

private:
    TagLibHelper() = default;
};
