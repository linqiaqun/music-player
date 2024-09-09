#include "librarypage.h"

#include <QMessageBox>
#include <QPushButton>

#include "./database/database.h"
#include "./flowlayout/flowlayout.h"
#include "./util/util.h"
#include "categoryitem.h"
#include "ui_librarypage.h"

LibraryPage::LibraryPage(QWidget *parent) : BasePage(parent), ui(new Ui::LibraryPage) {
    ui->setupUi(this);
    connect(ui->detailPage, &CategoryPage::back, this, [=]() {
        init();
        ui->stackedWidget->setCurrentWidget(ui->listPage);
    });

    m_layout = new FlowLayout(16, 16, 16);
    ui->listPage->setLayout(m_layout);
}

LibraryPage::~LibraryPage() { delete ui; }

bool LibraryPage::init() {
    Util::clearLayout(m_layout);
    for (auto item : DB->getCategoryList()) addCategory(item.toMap());

    QPushButton *btn = new QPushButton(this);
    btn->setObjectName("addBtn");
    connect(btn, &QPushButton::clicked, this, &LibraryPage::addBtnClicked);
    m_layout->addWidget(btn);

    ui->stackedWidget->setCurrentWidget(ui->listPage);
    return true;
}

void LibraryPage::addBtnClicked() {
    QVariantMap map = DB->addCategory(tr("category"));
    if (map.isEmpty()) {
        QMessageBox::critical(nullptr, tr("error"), tr("create category failed"));
        return;
    }

    QPushButton *btn = (QPushButton *)sender();
    m_layout->removeWidget(btn);

    CategoryItem *item = createItem(map);
    m_layout->addWidget(item);
    m_layout->addWidget(btn);
}

void LibraryPage::removeCategory() {
    CategoryItem *item = (CategoryItem *)sender();
    if (nullptr == item) return;

    QVariantMap map = item->data();
    if (QMessageBox::Ok !=
        QMessageBox::warning(nullptr, tr("warning"),
                             tr("are you sure to remove category %1").arg(map["name"].toString()),
                             QMessageBox::Cancel | QMessageBox::Ok)) {
        return;
    }

    Util::removeFile(map["cover"].toString());
    if (!DB->removeCategory(map["id"].toString())) {
        QMessageBox::critical(nullptr, tr("error"), tr("remove category failed"));
        return;
    }

    m_layout->removeWidget(item);
    item->setParent(nullptr);
    item->deleteLater();
}

void LibraryPage::updateCategory(QVariantMap map) {
    if (!DB->updateCategory(map)) {
        QMessageBox::critical(nullptr, tr("error"), tr("update category failed"));
        return;
    }
}

void LibraryPage::enterCategory() {
    CategoryItem *item = (CategoryItem *)sender();
    if (nullptr == item) return;

    ui->detailPage->setData(item->data());
    ui->stackedWidget->setCurrentWidget(ui->detailPage);
}

CategoryItem *LibraryPage::createItem(const QVariantMap &map) {
    CategoryItem *item = new CategoryItem(map);
    connect(item, &CategoryItem::removed, this, &LibraryPage::removeCategory);
    connect(item, &CategoryItem::updated, this, &LibraryPage::updateCategory);
    connect(item, &CategoryItem::enter, this, &LibraryPage::enterCategory);
    return item;
}

void LibraryPage::addCategory(const QVariantMap &map) {
    CategoryItem *item = createItem(map);
    if (nullptr != item) m_layout->addWidget(item);
}
