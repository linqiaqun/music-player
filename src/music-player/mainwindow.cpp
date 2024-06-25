#include "mainwindow.h"

#include <QGraphicsDropShadowEffect>

#include "./player/player.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : BaseWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->minBtn, &QPushButton::clicked, this, &MainWindow::showMinimized);
    connect(ui->maxBtn, &QPushButton::clicked, this, &MainWindow::showMaximized);
    connect(ui->restoreBtn, &QPushButton::clicked, this, &MainWindow::showNormal);
    connect(ui->closeBtn, &QPushButton::clicked, this, &MainWindow::close);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(Qt::gray);
    shadow->setBlurRadius(32);
    ui->container->setGraphicsEffect(shadow);

#if 1  // debug
    QVariantList list;
    QVariantMap map1;
    map1.insert("path", "C:/test/1.mp3");
    list.append(map1);
    QVariantMap map2;
    map2.insert("path", "C:/test/2.mp3");
    list.append(map2);

    Playlist *playlist = PLAYER->playlist();
    playlist->setSongList(list);

    PLAYER->play();
#endif
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::resizeEvent(QResizeEvent *ev) {
    Q_UNUSED(ev)
    ui->maxBtn->setHidden(isMaximized());
    ui->restoreBtn->setHidden(!isMaximized());
    setContentsMargins(isMaximized() ? QMargins(0, 0, 0, 0) : QMargins(8, 8, 8, 8));
}
