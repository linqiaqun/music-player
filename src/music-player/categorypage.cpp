#include "categorypage.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QtConcurrent>

#include "./database/database.h"
#include "./player/player.h"
#include "songitemdelegate.h"
#include "./taglibhelper/taglibhelper.h"
#include "./util/util.h"
#include "ui_categorypage.h"

QVariantList loadSongList(const QStringList &pathList) {
    QVariantList list;
    for (auto &path : pathList) {
        QVariantMap map = TagLibHelper::getPropertyList(path, TagLibHelper::propertyNames());
        if (map.isEmpty()) continue;
        map.insert("path", path);
        map.insert("favorite", 0);
        list.append(map);
    }

    return list;
}

CategoryPage::CategoryPage(QWidget *parent) : QWidget(parent), ui(new Ui::CategoryPage) {
    ui->setupUi(this);
    ui->cpList->setItemDelegate(new SongItemDelegate(ui->cpList));
    connect(ui->cpCover, &ClickableLabel::clicked, this, &CategoryPage::coverClicked);
    connect(ui->cpTitle, &LabelEdit::editingFinished, this, &CategoryPage::editingFinished);
    connect(ui->cpLoadBtn, &QPushButton::clicked, this, &CategoryPage::loadBtnClicked);
    connect(ui->cpClearBtn, &QPushButton::clicked, this, &CategoryPage::clearBtnClicked);
    connect(ui->cpPlayBtn, &QPushButton::clicked, this, &CategoryPage::playBtnClicked);
    connect(ui->cpBackBtn, &QPushButton::clicked, this, &CategoryPage::back);
}

CategoryPage::~CategoryPage() { delete ui; }

void CategoryPage::setData(const QVariantMap &map) {
    m_data = map;
    m_data.insert("song_list", DB->getSongList(m_data["id"].toString()));
    updateCategory(m_data);
}

void CategoryPage::editingFinished() {
    QString name = ui->cpTitle->text();
    if (name.isEmpty()) {
        ui->cpTitle->setText(m_data["name"].toString());
        return;
    }

    QVariantMap map = m_data;
    map.insert("name", name);
    if (!DB->updateCategory(map)) {
        QMessageBox::critical(nullptr, tr("error"), tr("update category failed"));
        ui->cpTitle->setText(m_data["name"].toString());
        return;
    }

    m_data = map;
}

void CategoryPage::loadBtnClicked() {
    QStringList pathList = QFileDialog::getOpenFileNames(
        nullptr, tr("select music file"),
        QStandardPaths::standardLocations(QStandardPaths::MusicLocation)[0],
        "Music File (*.mp3 *.wav *.flac *.ape *.ogg *.aac *.wma)");
    if (pathList.isEmpty()) return;

    QFutureWatcher<QVariantList> *watcher = new QFutureWatcher<QVariantList>;
    connect(watcher, &QFutureWatcher<QVariantList>::finished, this, [=]() {
        QVariantList list = watcher->result();
        watcher->deleteLater();

        if (!DB->addSongList(m_data["id"].toString(), list)) {
            QMessageBox::critical(nullptr, tr("error"), tr("add song list failed"));
            return;
        };

        m_data.insert("song_list", DB->getSongList(m_data["id"].toString()));
        updateCategory(m_data);
    });
    watcher->setFuture(QtConcurrent::run(loadSongList, pathList));
}

void CategoryPage::clearBtnClicked() {
    if (!DB->clearSongList(m_data["id"].toString())) {
        QMessageBox::critical(nullptr, tr("error"), tr("clear song list failed"));
        return;
    }

    PLAYER->playlist()->removeSongList(m_data["song_list"].toList());
    m_data.remove("song_list");
    ui->cpList->clear();
    ui->cpCount->setText("0");
}

void CategoryPage::playBtnClicked() {
    PLAYER->playlist()->setSongList(m_data["song_list"].toList());
    DB->setPlaylist(m_data["song_list"].toList());
}

void CategoryPage::updateCategory(const QVariantMap &category) {
    ui->cpCover->setPixmap(QPixmap(m_data["cover"].toString()));
    ui->cpTitle->setText(m_data["name"].toString());
    ui->cpCount->setText(QString::number(m_data["song_list"].toList().count()));

    ui->cpList->clear();
    for (auto &item : category["song_list"].toList()) {
        QListWidgetItem *songItem = new QListWidgetItem;
        songItem->setData(Qt::UserRole, item.toMap());
        ui->cpList->addItem(songItem);
    }
}

void CategoryPage::coverClicked() {
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

    // remove old cover
    QFile::remove(m_data["cover"].toString());

    // update new cover
    QVariantMap map = m_data;
    map.insert("cover", newPath);
    if (!DB->updateCategory(map)) {
        QMessageBox::critical(nullptr, tr("error"), tr("update playlist failed"));
        return;
    }

    m_data = map;
    ui->cpCover->setPixmap(QPixmap(newPath));
}
