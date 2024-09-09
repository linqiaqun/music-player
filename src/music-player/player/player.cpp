#include "player.h"

#include <QAudioOutput>
#include <QDebug>
#include <QGlobalStatic>
#include <QMediaMetaData>
#include <QMediaPlayer>

#include "playlist.h"

Q_GLOBAL_STATIC(Player, player)

Player::Player(QObject *parent) : QObject{parent} {
    m_output = new QAudioOutput(this);
    connect(m_output, &QAudioOutput::mutedChanged, this, &Player::mutedChanged);
    connect(m_output, &QAudioOutput::volumeChanged, this, &Player::volumeChanged);

    m_player = new QMediaPlayer(this);
    m_player->setAudioOutput(m_output);
    connect(m_player, &QMediaPlayer::durationChanged, this, &Player::durationChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &Player::positionChanged);
    connect(m_player, &QMediaPlayer::sourceChanged, this,
            [=](const QUrl &url) { emit sourceChanged(url.path()); });
    connect(m_player, &QMediaPlayer::metaDataChanged, this, [=]() {
        auto data = m_player->metaData();
        QVariantMap map;
        map.insert("album", data.value(QMediaMetaData::AlbumTitle));
        map.insert("artist", data.value(QMediaMetaData::ContributingArtist));
        map.insert("comment", data.value(QMediaMetaData::Comment));
        map.insert("duration", data.value(QMediaMetaData::Duration));
        map.insert("genre", data.value(QMediaMetaData::Genre));
        map.insert("title", data.value(QMediaMetaData::Title));
        map.insert("track", data.value(QMediaMetaData::TrackNumber));
        map.insert("cover", data.value(QMediaMetaData::CoverArtImage));
        map.insert("thumb", data.value(QMediaMetaData::ThumbnailImage));
        emit mediaDataChanged(map);
    });
    connect(m_player, &QMediaPlayer::playbackStateChanged, this, &Player::playbackStateChanged);

    m_list = new Playlist(this);
    connect(m_list, &Playlist::currentIndexChanged, this, &Player::currentIndexChanged);
}

Player *Player::instance() { return player; }

float Player::volume() const { return m_output->volume(); }

void Player::play() {
    QString path = m_list->nextSong();
    if (path.isEmpty()) return;

    m_player->setSource(QUrl(path));
    m_player->play();
}

void Player::play(QVariantMap song) {
    m_player->setSource(QUrl(song["path"].toString()));
    m_player->play();
}

void Player::pause() { m_player->pause(); }

void Player::stop() { m_player->stop(); }

void Player::previous() {
    QString url = m_list->previousSong();
    if (url.isEmpty()) return;

    disconnect(m_player, &QMediaPlayer::playbackStateChanged, this, &Player::playbackStateChanged);
    m_player->setSource(QUrl(url));
    m_player->play();
    connect(m_player, &QMediaPlayer::playbackStateChanged, this, &Player::playbackStateChanged);
}

void Player::next() { play(); }

void Player::setVolume(float vol) {
    QAudioOutput *device = m_player->audioOutput();
    if (nullptr != device) device->setVolume(vol);
}

void Player::setMuted(bool muted) {
    QAudioOutput *device = m_player->audioOutput();
    if (nullptr != device) device->setMuted(muted);
}

void Player::setPosition(qint64 pos) { m_player->setPosition(pos); }

void Player::playbackStateChanged(int state) {
    emit stateChanged(int(state));
}

void Player::currentIndexChanged() {
    if (m_player->isPlaying()) m_player->stop();
    play();
}
