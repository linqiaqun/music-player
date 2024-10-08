#pragma once

#include "basepage.h"

namespace Ui {
class FavoritePage;
}

class FavoritePage : public BasePage {
    Q_OBJECT

public:
    explicit FavoritePage(QWidget *parent = nullptr);
    ~FavoritePage();

    bool init() override;

private slots:
    void playBtnClicked();

private:
    Ui::FavoritePage *ui;
};
