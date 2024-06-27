#include "settingspage.h"

#include "ui_settingspage.h"

SettingsPage::SettingsPage(QWidget *parent) : BasePage(parent), ui(new Ui::SettingsPage) {
    ui->setupUi(this);
}

SettingsPage::~SettingsPage() { delete ui; }

bool SettingsPage::init() { return true; }
