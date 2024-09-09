#include "VolumeWidget.h"

#include "./configuration/configuration.h"
#include "./player/player.h"
#include "ui_VolumeWidget.h"

VolumeWidget::VolumeWidget(QWidget *parent) : QWidget(parent), ui(new Ui::VolumeWidget) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);

    connect(ui->vwVolume, &QSlider::valueChanged, this,
            [=](int value) { PLAYER->setVolume(value / 100.0); });
    connect(PLAYER, &Player::volumeChanged, this,
            [=](float volume) { CONFIG->setValue("volume", volume); });

    ui->vwVolume->setValue((int)(100 * PLAYER->volume()));
}

VolumeWidget::~VolumeWidget() { delete ui; }
