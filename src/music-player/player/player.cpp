#include "player.h"

#include <QAudioOutput>
#include <QDebug>
#include <QMediaPlayer>

#include "playlist.h"

Player::Player(QObject *parent) : QObject{parent} {
    m_output = new QAudioOutput(this);
    connect(m_output, &QAudioOutput::mutedChanged, this, &Player::mutedChanged);
    connect(m_output, &QAudioOutput::volumeChanged, this, &Player::volumeChanged);

    m_player = new QMediaPlayer(this);
    m_player->setAudioOutput(m_output);
    connect(m_player, &QMediaPlayer::durationChanged, this, &Player::durationChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &Player::positionChanged);

    m_list = new Playlist(this);
}

void Player::play() {
    QString path = m_list->nextSong();
    if (path.isEmpty()) return;

    m_player->setSource(QUrl(path));
    m_player->play();
}

void Player::pause() { m_player->pause(); }

void Player::stop() { m_player->stop(); }

void Player::previous() {}

void Player::next() {}

void Player::setVolume(float vol) {
    QAudioOutput *device = m_player->audioOutput();
    if (nullptr != device) device->setVolume(vol);
}

void Player::setMuted(bool muted) {
    QAudioOutput *device = m_player->audioOutput();
    if (nullptr != device) device->setMuted(muted);
}

void Player::setPosition(qint64 pos) { m_player->setPosition(pos); }
