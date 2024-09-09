#include "util.h"

#include <QApplication>
#include <QFile>
#include <QLayout>
#include <QUuid>
#include <QWidget>

void Util::clearLayout(QLayout *layout) {
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != 0) {
        if (child->widget()) {
            child->widget()->setParent(nullptr);
        }

        delete child;
    }
}

QString Util::uuid() { return QUuid::createUuid().toString(QUuid::Id128); }

QByteArray Util::readFile(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return QByteArray();
    }

    QByteArray data = file.readAll();
    file.close();
    return data;
}

bool Util::writeFile(const QByteArray &data, const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write(data);
    file.flush();
    file.close();
    return true;
}

bool Util::setQss(const QVariantList &list, int index) {
    if ((index < 0) || (index >= list.count())) {
        qCritical() << "index range error: " << index;
        return false;
    }

    QString path = qApp->applicationDirPath() + list[index].toMap()["path"].toString();
    if (!QFile::exists(path)) {
        qCritical() << "qss file not exists: " << path;
        return false;
    }

    qApp->setStyleSheet(readFile(path));
    return true;
}

QString Util::secToTime(int sec) {
    QString result;
    int h = sec / 3600;
    if (h > 0) result += QString("%1:").arg(h, 2, 10, QChar('0'));
    sec = sec % 3600;
    int m = sec / 60;
    result += QString("%1:").arg(m, 2, 10, QChar('0'));
    int s = sec % 60;
    result += QString("%1").arg(s, 2, 10, QChar('0'));
    return result;
}

bool Util::removeFile(const QString &path) {
    if (!QFile::exists(path)) return true;
    return QFile::remove(path);
}
