#include "favoritepage.h"

#include "./database/database.h"
#include "./player/player.h"
#include "songitemdelegate.h"
#include "ui_favoritepage.h"

FavoritePage::FavoritePage(QWidget *parent) : BasePage(parent), ui(new Ui::FavoritePage) {
    ui->setupUi(this);
    connect(ui->fpPlayBtn, &QPushButton::clicked, this, &FavoritePage::playBtnClicked);

    SongItemDelegate *delegate = new SongItemDelegate(ui->fpList);
    connect(delegate, &SongItemDelegate::favoriteChanged, this, [=](int index, bool fav) {
        if (!fav) {
            QListWidgetItem *item = ui->fpList->takeItem(index);
            delete item;
        }
    });
    ui->fpList->setItemDelegate(delegate);
}

FavoritePage::~FavoritePage() { delete ui; }

bool FavoritePage::init() {
    ui->fpList->clear();
    QVariantList list = DB->getFavorite();
    for (auto &item : list) {
        QListWidgetItem *songItem = new QListWidgetItem;
        songItem->setData(Qt::UserRole, item.toMap());
        ui->fpList->addItem(songItem);
    }
    ui->fpCount->setText(QString::number(list.count()));

    return true;
}

void FavoritePage::playBtnClicked() { PLAYER->playlist()->setSongList(DB->getFavorite()); }
