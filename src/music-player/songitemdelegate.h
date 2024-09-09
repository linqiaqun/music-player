#pragma once

#include <QStyledItemDelegate>

class SongItemDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit SongItemDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}
    virtual ~SongItemDelegate() {}

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                           const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                     const QModelIndex &index) override;

signals:
    void favoriteChanged(int, bool);

private:
    QString secToTime(int sec) const;
    void showMenu(QAbstractItemModel *model, const QModelIndex &index);
    QVariantMap editMetaData(const QVariantMap &map);
    void changeFavorite(QAbstractItemModel *model, const QModelIndex &index);

private:
    int m_itemHeight = 32;
};
