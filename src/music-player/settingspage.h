#pragma once

#include "basepage.h"

namespace Ui {
class SettingsPage;
}

class SettingsPage : public BasePage {
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = nullptr);
    ~SettingsPage();

    bool init() override;

private:
    Ui::SettingsPage *ui;
};
