#include "playlist.h"

#include <QRandomGenerator>

Playlist::Playlist(QObject *parent) : QObject{parent} {}

void Playlist::setSongList(const QVariantList &songList) {
    m_list = songList;
    m_history.clear();
    m_index = -1;
}

void Playlist::appendSong(const QVariantMap &song) { m_list.append(song); }

void Playlist::insertSong(int index, const QVariantMap &song) {
    m_list.insert(index, song);
    m_history.clear();
}

bool Playlist::removeSong(int index) {
    if ((index < 0) || (index >= m_list.count())) return false;
    m_list.removeAt(index);
    m_history.clear();
    return true;
}

void Playlist::clear() {
    m_list.clear();
    m_index = -1;
    m_history.clear();
}

bool Playlist::isEmpty() const { return m_list.isEmpty(); }

void Playlist::setPlaybackMode(PLAYBACK_MODE mode) { m_mode = mode; }

void Playlist::setCurrentIndex(int index) {
    if ((index >= 0) && (index < m_list.count())) m_index = index;
}

QString Playlist::songAt(int index) {
    if (!isAvailableIndex(index)) return "";
    return m_list[index].toMap()["path"].toString();
}

QString Playlist::currentSong() {
    if ((m_index < 0) || (m_index >= m_list.count())) return "";
    return m_list[m_index].toMap()["path"].toString();
}

QString Playlist::previousSong() {
    if ((CurrentItemOnce == m_mode) || (CurrentItemInLoop == m_mode) || (m_history.isEmpty()))
        return "";
    int index = m_history.last();
    m_history.pop_back();
    return songAt(index);
}

QString Playlist::nextSong() {
    switch (m_mode) {
        case CurrentItemOnce:
        case CurrentItemInLoop:
            return "";

        case Sequential:
            if (m_index == m_list.count() - 1) return "";
            return songAt(++m_index);

        case Loop:
            m_index = (m_index + 1) % m_list.count();
            return songAt(m_index);

        case Random: {
            int index = -1;
            while ((-1 == index) || (index == m_index)) {
                index = QRandomGenerator::global()->bounded(0, m_list.count());
            }
            m_index = index;
            return songAt(index);
        }

        default:
            return "";
    }
}

bool Playlist::isAvailableIndex(int index) const {
    return ((index >= 0) && (index < m_list.count()));
}
