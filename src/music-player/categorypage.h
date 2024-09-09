#pragma once

#include <QWidget>

namespace Ui {
class CategoryPage;
}

class CategoryPage : public QWidget {
    Q_OBJECT

public:
    explicit CategoryPage(QWidget *parent = nullptr);
    ~CategoryPage();

    void setData(const QVariantMap &map);
    QVariantMap data() const { return m_data; }

signals:
    void back();

private slots:
    void editingFinished();
    void loadBtnClicked();
    void clearBtnClicked();
    void playBtnClicked();
    void updateCategory(const QVariantMap &category);
    void coverClicked();

private:
    Ui::CategoryPage *ui;
    QVariantMap m_data;
};
