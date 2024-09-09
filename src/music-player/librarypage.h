#pragma once

#include "basepage.h"

namespace Ui {
class LibraryPage;
}

class FlowLayout;
class CategoryItem;
class LibraryPage : public BasePage {
    Q_OBJECT

public:
    explicit LibraryPage(QWidget *parent = nullptr);
    ~LibraryPage();

    bool init() override;

private slots:
    void addBtnClicked();
    void removeCategory();
    void updateCategory(QVariantMap);
    void enterCategory();

private:
    CategoryItem *createItem(const QVariantMap &map);
    void addCategory(const QVariantMap &map);

private:
    Ui::LibraryPage *ui;
    FlowLayout *m_layout = nullptr;
};
