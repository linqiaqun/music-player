#include "mainwindow.h"

#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>

#include "./configuration/configuration.h"
#include "./player/player.h"
#include "basepage.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : BaseWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->minBtn, &QPushButton::clicked, this, &MainWindow::showMinimized);
    connect(ui->maxBtn, &QPushButton::clicked, this, &MainWindow::showMaximized);
    connect(ui->restoreBtn, &QPushButton::clicked, this, &MainWindow::showNormal);
    connect(ui->closeBtn, &QPushButton::clicked, this, &MainWindow::close);

    // window shadow setting
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(Qt::gray);
    shadow->setBlurRadius(32);
    ui->container->setGraphicsEffect(shadow);

    // setting of switching menu button
    QList<QToolButton *> menuList = {ui->homeBtn, ui->libBtn, ui->favBtn, ui->settingBtn};
    for (auto i = 0; i < menuList.count(); ++i) ui->menuGroup->setId(menuList[i], i);
    connect(ui->menuGroup, &QButtonGroup::idClicked, this, [=](int id) {
        BasePage *page = dynamic_cast<BasePage *>(ui->stackedWidget->widget(id));
        if (nullptr == page) {
            qCritical() << "page not exist: " << id;
            return;
        }

        if (!page->init()) {
            QMessageBox::critical(nullptr, tr("error"), tr("initialize page failed: %1").arg(id));
        }
        ui->stackedWidget->setCurrentIndex(id);
    });
    ui->homeBtn->click();

    PLAYER->setVolume(CONFIG->value("volume").toFloat());
    PLAYER->playlist()->setPlaybackMode((Playlist::PLAYBACK_MODE)CONFIG->value("playback_mode").toInt());
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::resizeEvent(QResizeEvent *ev) {
    Q_UNUSED(ev)
    ui->maxBtn->setHidden(isMaximized());
    ui->restoreBtn->setHidden(!isMaximized());
    setContentsMargins(isMaximized() ? QMargins(0, 0, 0, 0) : QMargins(8, 8, 8, 8));
}
