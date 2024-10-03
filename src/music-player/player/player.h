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
    float volume() const;
    bool isPlaying() const;

signals:
    void mutedChanged(bool);
    void volumeChanged(float);
    void durationChanged(qint64);
    void positionChanged(qint64);
    void sourceChanged(QString);
    void mediaDataChanged(QVariantMap);
    void stateChanged(int);

public slots:
    void play();
    void play(QVariantMap);
    void pause();
    void stop();
    void previous();
    void next();

    void setMuted(bool muted);
    void setVolume(float);
    void setPosition(qint64 pos);

private slots:
    void playbackStateChanged(int);
    void currentIndexChanged();

private:
    QAudioOutput *m_output = nullptr;
    QMediaPlayer *m_player = nullptr;
    Playlist *m_list = nullptr;
};
