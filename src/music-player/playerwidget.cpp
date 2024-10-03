#include "playerwidget.h"

#include "./configuration/configuration.h"
#include "./database/database.h"
#include "./player/player.h"
#include "./util/util.h"
#include "VolumeWidget.h"
#include "ui_playerwidget.h"

PlayerWidget::PlayerWidget(QWidget *parent) : QWidget(parent), ui(new Ui::PlayerWidget) {
    ui->setupUi(this);
    connect(ui->pwProgress, &ClickableProgressBar::valueClicked, this,
            &PlayerWidget::positionClicked);
    connect(ui->pwModeBtn, &QPushButton::clicked, this, &PlayerWidget::modeBtnClicked);
    connect(ui->pwPreviousBtn, &QPushButton::clicked, this, &PlayerWidget::previousBtnClicked);
    connect(ui->pwPlayBtn, &QPushButton::clicked, this, &PlayerWidget::playBtnClicked);
    connect(ui->pwNextBtn, &QPushButton::clicked, this, &PlayerWidget::nextBtnClicked);
    connect(ui->pwVolumeBtn, &QPushButton::clicked, this, &PlayerWidget::volumeBtnClicked);
    connect(ui->pwPlaylistBtn, &QPushButton::clicked, this, &PlayerWidget::playlistBtnClicked);

    connect(PLAYER->playlist(), &Playlist::playbackModeChanged, this, &PlayerWidget::modeChanged);
    connect(PLAYER, &Player::volumeChanged, this, [=](float vol) {
        if (vol >= 0.99) {
            ui->pwVolumeBtn->setIcon(QIcon(QPixmap(":/images/volume_max.png")));
        } else if (0 == vol) {
            ui->pwVolumeBtn->setIcon(QIcon(QPixmap(":/images/volume_muted.png")));
        } else {
            ui->pwVolumeBtn->setIcon(QIcon(QPixmap(":/images/volume_normal.png")));
        }
    });
    connect(PLAYER, &Player::durationChanged, this, [=](qint64 duration) {
        ui->pwDuration->setText(Util::secToTime(duration / 1000));
        ui->pwProgress->setMaximum(duration);
    });
    connect(PLAYER, &Player::positionChanged, this, &PlayerWidget::positionChanged);
    // connect(PLAYER, &Player::stateChanged, this, [=](int state) {
    //     bool playing = (Player::PBS_PLAYING == state);
    //     QIcon icon =
    //         playing ? QIcon(QPixmap(":/images/pause.png")) : QIcon(QPixmap(":/images/play.png"));
    //     ui->cbPlayBtn->setIcon(icon);
    //     ui->cbProgressFrame->setEnabled(playing);
    // });
    // connect(PLAYER, &Player::typeChanged, this, &ControlBar::typeChanged);
    connect(PLAYER, &Player::mediaDataChanged, this, [=](QVariantMap map) {
        ui->pwCover->setPixmap(QPixmap::fromImage(map["thumb"].value<QImage>()));
        ui->pwTitle->setText(map["title"].toString());
        ui->pwArtist->setText(map["artist"].toString());
    });
    connect(PLAYER, &Player::stateChanged, this, &PlayerWidget::stateChanged);
}

PlayerWidget::~PlayerWidget() { delete ui; }

void PlayerWidget::init() {
    if (m_init) return;
    m_init = true;

    QVariantList list = DB->getPlaylist();
    ui->pwPreviousBtn->setEnabled(!list.isEmpty());
    ui->pwPlayBtn->setEnabled(!list.isEmpty());
    ui->pwNextBtn->setEnabled(!list.isEmpty());
    ui->pwFavBtn->setHidden(list.isEmpty());
    if (list.isEmpty()) {
        return;
    }

    // use last play song
    QString id = CONFIG->value("last_play").toString();
    for (auto &item : list) {
        QVariantMap map = item.toMap();
        if (id == map["id"].toString()) {
            setSong(map);
            return;
        }
    }

    // use first song
    setSong(list[0].toMap());
}

void PlayerWidget::modeBtnClicked() { PLAYER->playlist()->nextPlaybackMode(); }

void PlayerWidget::previousBtnClicked() { PLAYER->previous(); }

void PlayerWidget::playBtnClicked() {
    if (PLAYER->isPlaying())
        PLAYER->pause();
    else
        PLAYER->play();
}

void PlayerWidget::nextBtnClicked() { PLAYER->next(); }

void PlayerWidget::volumeBtnClicked() {
    VolumeWidget *widget = new VolumeWidget;
    widget->show();
    widget->move(QCursor::pos().x() - widget->width(), QCursor::pos().y() - widget->height());
}

void PlayerWidget::playlistBtnClicked() {}

void PlayerWidget::modeChanged(int mode) {
    CONFIG->setValue("playback_mode", mode);
    QStringList iconList = {":/images/current-item-once.png", ":/images/current-item-in-loop.png",
                            ":/images/sequential.png", ":/images/loop.png", ":/images/random.png"};
    QStringList tipList = {
        "The current item is played only once", "The current item is played repeatedly in a loop",
        "Starting from the current item, all items are played in sequence once until the end",
        "Play items in sequence and repeatedly", "Play items in random order"};

    ui->pwModeBtn->setIcon(QIcon(QPixmap(iconList[mode])));
    ui->pwModeBtn->setToolTip(tipList[mode]);
}

void PlayerWidget::positionChanged(qint64 pos) {
    ui->pwPosition->setText(Util::secToTime(pos / 1000));
    ui->pwProgress->setValue(pos);
}

void PlayerWidget::positionClicked(qint64 pos) {
    disconnect(PLAYER, &Player::positionChanged, this, &PlayerWidget::positionChanged);
    PLAYER->setPosition(pos);
    connect(PLAYER, &Player::positionChanged, this, &PlayerWidget::positionChanged);
}

void PlayerWidget::stateChanged(int state) {
    if (1 == state)
        ui->pwPlayBtn->setIcon(QIcon(QPixmap(":/images/pause.png")));
    else
        ui->pwPlayBtn->setIcon(QIcon(QPixmap(":/images/play.png")));
}

void PlayerWidget::setSong(const QVariantMap &map) {
    ui->pwTitle->setText(map["name"].toString());
    ui->pwArtist->setText(map["artist"].toString());
    ui->pwFavBtn->setChecked(map["favorite"].toBool());
}
