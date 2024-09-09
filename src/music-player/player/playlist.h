#pragma once

#include <QObject>
#include <QVariant>

class Playlist : public QObject {
    Q_OBJECT
public:
    enum PLAYBACK_MODE {
        CurrentItemOnce = 0,
        CurrentItemInLoop,
        Sequential,
        Loop,
        Random,
        PM_Unknown = 5
    };
    explicit Playlist(QObject *parent = nullptr);

    void setSongList(const QVariantList &songList);
    QVariantList songList() const { return m_list; }

    int appendSong(const QVariantMap &song);
    void insertSong(int index, const QVariantMap &song);
    bool removeSong(int index);
    void removeSongList(const QVariantList &list);
    void clear();
    bool isEmpty() const;

    PLAYBACK_MODE playbackMode() const { return m_mode; }
    void setPlaybackMode(PLAYBACK_MODE mode);
    void nextPlaybackMode();

    void setCurrentIndex(int index);
    QString songAt(int index);
    QString currentSong();
    QString previousSong();
    QString nextSong();

signals:
    void currentIndexChanged();
    void playbackModeChanged(int);

private:
    bool isAvailableIndex(int index) const;

private:
    PLAYBACK_MODE m_mode = Sequential;
    QVariantList m_list;
    int m_index = -1;
    QList<int> m_history;
};
