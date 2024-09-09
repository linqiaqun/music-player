#include "homepage.h"

#include <QListWidgetItem>
#include <QPushButton>

#include "./database/database.h"
#include "./util/util.h"
#include "ui_homepage.h"

HomePage::HomePage(QWidget *parent) : BasePage(parent), ui(new Ui::HomePage) { ui->setupUi(this); }

HomePage::~HomePage() { delete ui; }

bool HomePage::init() {
    Util::clearLayout(ui->categoryLayout);

    QVariantList list = DB->getCategoryList();
    for (auto item : list) addCategory(item.toMap());
    ui->categoryLayout->addStretch();
    ui->hpStackedWidget->setCurrentWidget(list.isEmpty() ? ui->hpPage2 : ui->hpPage1);

    ui->hpPlayer->init();

    return true;
}

void HomePage::addCategory(const QVariantMap &map) {
    QPushButton *btn = new QPushButton(map["name"].toString());
    btn->setFixedSize(320, 180);
    ui->categoryLayout->addWidget(btn);
}
