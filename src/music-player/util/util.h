#pragma once

#include <QString>

class QLayout;
class Util {
public:
    static void clearLayout(QLayout *layout);
    static QString uuid();
    static QByteArray readFile(const QString &path);
    static bool writeFile(const QByteArray &data, const QString &path);
    static bool setQss(const QVariantList &list, int index);
    static QString secToTime(int sec);
    static bool removeFile(const QString &path);

private:
    Util() = default;
};
