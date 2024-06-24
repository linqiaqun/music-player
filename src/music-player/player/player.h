/*!
 *    file: player.h
 *    author: linqiaqun@163.com
 *    date: 2024-06-20 11:43
 *    license: LGPL-3.0-only
 */

#pragma once

#include <QObject>

#include "playlist.h"

#define PLAYER Player::instance()

class QAudioOutput;
class QMediaPlayer;
class Playlist;
class Player : public QObject {
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);
    static Player *instance();

    Playlist *playlist() const { return m_list; }

signals:
    void mutedChanged(bool);
    void volumeChanged(float);
    void durationChanged(qint64);
    void positionChanged(qint64);

public slots:
    void play();
    void pause();
    void stop();
    void previous();
    void next();

    void setMuted(bool muted);
    void setVolume(float);
    void setPosition(qint64 pos);

private:
    QAudioOutput *m_output = nullptr;
    QMediaPlayer *m_player = nullptr;
    Playlist *m_list = nullptr;
};
