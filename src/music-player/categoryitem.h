#pragma once

#include <QWidget>

namespace Ui {
class CategoryItem;
}

class QPushButton;
class CategoryItem : public QWidget {
    Q_OBJECT

public:
    explicit CategoryItem(QWidget *parent = nullptr);
    CategoryItem(const QVariantMap &map, QWidget *parent = nullptr);
    ~CategoryItem();

    void setData(const QVariantMap &map);
    QVariantMap data() const { return m_map; }

signals:
    void removed();
    void updated(QVariantMap);
    void enter();

protected:
    void resizeEvent(QResizeEvent *ev);
    void enterEvent(QEnterEvent *ev) override;
    void leaveEvent(QEvent *ev) override;
    void mouseDoubleClickEvent(QMouseEvent *ev);

private slots:
    void changeCover();
    void editingFinished();

private:
    Ui::CategoryItem *ui;
    QPushButton *m_btn = nullptr;
    QVariantMap m_map;
};
