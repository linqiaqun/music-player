#include "TagLibHelper.h"

#include <fileref.h>
#include <taglib.h>

#include <QDebug>
#include <QFile>

QStringList TagLibHelper::propertyNames() {
    return {"title", "artist", "album", "genre", "year", "track", "comment", "duration"};
}

QVariantMap TagLibHelper::getPropertyList(const QString &path, const QStringList &nameList) {
    if (!QFile::exists(path)) {
        qCritical() << "file not exists: " << path;
        return QVariantMap();
    }

    TagLib::FileRef *ref = new TagLib::FileRef(TagLib::FileName(path.toLocal8Bit().data()));
    if (nullptr == ref) {
        qCritical() << "load file failed: " << path;
        return QVariantMap();
    }
    TagLib::Tag *tag = ref->tag();

    QVariantMap resultMap;
    for (auto &name : nameList) {
        QVariantMap map;
        switch (propertyNames().indexOf(name)) {
            case 0:
                resultMap.insert(name, QString::fromWCharArray(tag->title().toCWString()));
                break;

            case 1:
                resultMap.insert(name, QString::fromWCharArray(tag->artist().toCWString()));
                break;

            case 2:
                resultMap.insert(name, QString::fromWCharArray(tag->album().toCWString()));
                break;

            case 3:
                resultMap.insert(name, QString::fromWCharArray(tag->genre().toCWString()));
                break;

            case 4:
                resultMap.insert(name, tag->year());
                break;

            case 5:
                resultMap.insert(name, tag->track());
                break;

            case 6:
                resultMap.insert(name, QString::fromWCharArray(tag->comment().toCWString()));
                break;

            case 7:
                resultMap.insert(name, ref->file()->audioProperties()->length());
                break;

            default:
                qCritical() << "unsupported property: " << name;
                break;
        }
    }

    delete ref;
    return resultMap;
}

bool TagLibHelper::setPropertyList(const QString &path, const QVariantList &list) {
    if (!QFile::exists(path)) {
        qCritical() << "file not exists: " << path;
        return false;
    }

    TagLib::FileRef *ref = new TagLib::FileRef(TagLib::FileName(path.toLocal8Bit().data()));
    if (nullptr == ref) {
        qCritical() << "load file failed: " << path;
        return false;
    }

    TagLib::Tag *tag = ref->tag();
    for (auto &item : list) {
        QVariantMap map = item.toMap();
        switch (propertyNames().indexOf(map["name"].toString())) {
            case 0:
                tag->setArtist(TagLib::String(map["value"].toString().toStdWString().data()));
                break;

            case 1:
                tag->setAlbum(TagLib::String(map["value"].toString().toStdWString().data()));
                break;

            case 2:
                tag->setComment(TagLib::String(map["value"].toString().toStdWString().data()));
                break;

            case 3:
                tag->setGenre(TagLib::String(map["value"].toString().toStdWString().data()));
                break;

            case 4:
                tag->setTitle(TagLib::String(map["value"].toString().toStdWString().data()));
                break;

            case 5:
                tag->setTrack(map["value"].toInt());
                break;

            case 6:
                tag->setYear(map["value"].toInt());
                break;

            default:
                qCritical() << "unsupported property: " << map["name"].toString();
                return false;
        }
    }

    bool ret = ref->save();
    delete ref;
    if (!ret) qCritical() << "save failed";
    return ret;
}
