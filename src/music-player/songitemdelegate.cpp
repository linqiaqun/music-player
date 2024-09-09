#include "songitemdelegate.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>

#include "./Database/Database.h"
#include "./Player/Player.h"
#include "metadataeditor.h"

void SongItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const {
    // if (0 == index.row() % 2) {
    //     painter->fillRect(option.rect, QColor(147, 178, 129));
    // } else {
    //     painter->fillRect(option.rect, QColor(137, 168, 119));
    // }

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, QColor(32, 32, 32));
        painter->setPen(QColor(123, 144, 236));
    } else {
        painter->setPen(QColor(255, 255, 255));
    }

    QRect rc = option.rect;
    QString orderNum = QString::number(index.row()).rightJustified(3, QChar('0'));
    painter->drawText(0, rc.top(), 40, rc.height(), Qt::AlignCenter, orderNum);

    QVariantMap map = index.data(Qt::UserRole).toMap();
    painter->drawPixmap(52, rc.top() + 8, 16, 16,
                        map["favorite"].toBool() ? QPixmap(":/images/fav-fill.png")
                                                 : QPixmap(":/images/fav-border.png"));

    painter->drawText(80, rc.top(), rc.width() - 240, rc.height(), Qt::AlignLeft | Qt::AlignVCenter,
                      map["title"].toString());
    painter->drawText(rc.width() - 268, rc.top(), 128, rc.height(), Qt::AlignCenter,
                      map["artist"].toString());
    painter->drawText(rc.width() - 144, rc.top(), 72, rc.height(), Qt::AlignCenter,
                      secToTime(map["duration"].toInt()));
    painter->drawText(rc.width() - 48, rc.top(), 48, rc.height(), Qt::AlignCenter, "...");
}

QSize SongItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const {
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(m_itemHeight);
    return size;
}

bool SongItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                   const QStyleOptionViewItem &option, const QModelIndex &index) {
    QMouseEvent *ev = static_cast<QMouseEvent *>(event);
    if (QEvent::MouseButtonPress != ev->type()) {
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }

    QRect rc = option.rect;
    QRect menuRect(rc.width() - 48, rc.top(), 48, rc.height());
    QRect favRect(52, rc.top() + 8, 16, 16);
    if (menuRect.contains(ev->pos())) {
        showMenu(model, index);
    } else if (favRect.contains(ev->pos())) {
        changeFavorite(model, index);
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QString SongItemDelegate::secToTime(int sec) const {
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

void SongItemDelegate::showMenu(QAbstractItemModel *model, const QModelIndex &index) {
    QMenu *menu = new QMenu;

    // play song action
    QAction *playAct = new QAction(QIcon(QPixmap(":/images/play-action.png")), tr("Play"), menu);
    connect(playAct, &QAction::triggered, this, [=]() {
        int songIndex = PLAYER->playlist()->appendSong(index.data(Qt::UserRole).toMap());
        PLAYER->playlist()->setCurrentIndex(songIndex);
        PLAYER->play(index.data(Qt::UserRole).toMap());
    });
    menu->addAction(playAct);

    // modify song's tags action
    QAction *tagAct = new QAction(QIcon(QPixmap(":/images/edit-action.png")), tr("Modify tag"), menu);
    connect(tagAct, &QAction::triggered, this, [=]() {
        QVariantMap map = editMetaData(index.data(Qt::UserRole).toMap());
        model->setData(index, map, Qt::UserRole);
        model->dataChanged(index, index);
    });
    menu->addAction(tagAct);

    // add song to playlist action
    QAction *addAct = new QAction(QIcon(QPixmap(":/images/add-action.png")), tr("Add to playlist"), menu);
    connect(addAct, &QAction::triggered, this,
            [=]() { PLAYER->playlist()->appendSong(index.data(Qt::UserRole).toMap()); });
    menu->addAction(addAct);

    // remove song action
    QAction *delAct = new QAction(QIcon(QPixmap(":/images/remove-action.png")), tr("Remove"), menu);
    connect(delAct, &QAction::triggered, this, [=]() {
        if (!DB->removeSong(index.data(Qt::UserRole).toMap()["id"].toString())) {
            QMessageBox::critical(nullptr, tr("error"), tr("remove song failed"));
            return;
        }

        PLAYER->playlist()->removeSongList(QVariantList() << index.data(Qt::UserRole));
        model->removeRow(index.row());
    });
    menu->addAction(delAct);

    bool value = model->data(index, Qt::UserRole + 1).toBool();
    model->setData(index, !value, Qt::UserRole + 1);
    model->dataChanged(index, index);

    menu->popup(QCursor::pos());
}

QVariantMap SongItemDelegate::editMetaData(const QVariantMap &map) {
    if (map.isEmpty()) return map;

    MetaDataEditor editor(map);
    if (QDialog::Accepted != editor.exec()) return map;
    if (editor.data() == map) return map;
    if (!DB->updateSong(editor.data())) {
        QMessageBox::critical(nullptr, tr("error"), tr("update song information failed"));
        return map;
    }

    return editor.data();
}

void SongItemDelegate::changeFavorite(QAbstractItemModel *model, const QModelIndex &index) {
    QVariantMap map = index.data(Qt::UserRole).toMap();
    map.insert("favorite", !map["favorite"].toBool());
    if (!DB->updateSong(map)) {
        QMessageBox::critical(nullptr, tr("error"), tr("update favorite status failed"));
        return;
    }
    model->setData(index, map, Qt::UserRole);
    emit favoriteChanged(index.row(), map["favorite"].toBool());
}
