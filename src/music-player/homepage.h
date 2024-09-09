#pragma once

#include "basepage.h"

namespace Ui {
class HomePage;
}

class HomePage : public BasePage {
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();

    bool init() override;

private:
    void addCategory(const QVariantMap &map);

private:
    Ui::HomePage *ui;
};
