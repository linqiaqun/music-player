#include "categoryitem.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QStandardPaths>

#include "./util/util.h"
#include "ui_categoryitem.h"

CategoryItem::CategoryItem(QWidget *parent) : QWidget(parent), ui(new Ui::CategoryItem) {
    ui->setupUi(this);
    connect(ui->ciEdit, &LabelEdit::editingFinished, this, &CategoryItem::editingFinished);

    m_btn = new QPushButton(this);
    m_btn->setObjectName("closeBtn");
    connect(m_btn, &QPushButton::clicked, this, [=]() { emit removed(); });
    m_btn->hide();
}

CategoryItem::CategoryItem(const QVariantMap &map, QWidget *parent) : CategoryItem(parent) {
    setData(map);
}

CategoryItem::~CategoryItem() { delete ui; }

void CategoryItem::setData(const QVariantMap &map) {
    m_map = map;
    ui->ciLabel->setPixmap(QPixmap(map["cover"].toString()));
    ui->ciEdit->setText(map["name"].toString());
    ui->ciEdit->setReadOnly(true);
}

void CategoryItem::resizeEvent(QResizeEvent *ev) {
    Q_UNUSED(ev)
    m_btn->move(width() - m_btn->width(), 0);
}

void CategoryItem::enterEvent(QEnterEvent *ev) {
    Q_UNUSED(ev)
    m_btn->show();
}

void CategoryItem::leaveEvent(QEvent *ev) {
    Q_UNUSED(ev)
    m_btn->hide();
}

void CategoryItem::mouseDoubleClickEvent(QMouseEvent *ev) {
    if (Qt::LeftButton != ev->button()) return;
    emit enter();
}

void CategoryItem::changeCover() {
    QString path = QFileDialog::getOpenFileName(
        nullptr, tr("change cover picture"),
        QStandardPaths::standardLocations(QStandardPaths::PicturesLocation)[0],
        tr("Cover Picture(*.jpg *.jpeg *.png *.bmp)"));
    if (path.isEmpty()) return;

    QString newPath = QCoreApplication::applicationDirPath() + "/data/cover/" + Util::uuid() + "." +
                      QFileInfo(path).suffix();
    if (!QFile::copy(path, newPath)) {
        QMessageBox::critical(nullptr, tr("error"), tr("save cover picture failed"));
        return;
    }

    ui->ciLabel->setPixmap(QPixmap(newPath));
    m_map.insert("cover", newPath);
    emit updated(m_map);
}

void CategoryItem::editingFinished() {
    ui->ciEdit->setReadOnly(true);
    QString name = ui->ciEdit->text();
    if (name.isEmpty()) {
        ui->ciEdit->setText(m_map["name"].toString());
        return;
    }

    m_map.insert("name", name);
    emit updated(m_map);
}
