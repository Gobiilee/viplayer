#ifndef PLAYERVIEWMODEL_H
#define PLAYERVIEWMODEL_H

#include <QObject>
#include <QQmlEngine>
#include "../models/VideoModel.h"
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QtConcurrent/QtConcurrent>
#include <QMutex>


class PlayerViewModel : public QObject {
    Q_OBJECT
    // Register attribute with QML, which make auto updated UI when data changed
    Q_PROPERTY(QString videoSource READ videoSource NOTIFY videoSourceChanged)
    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(bool isPlaying READ isPlaying WRITE setIsPlaying NOTIFY isPlayingChanged)

    Q_PROPERTY(float volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool hasPlaylist READ hasPlaylist NOTIFY playlistChanged)

    Q_PROPERTY(QUrl currentFolderUrl READ currentFolderUrl NOTIFY currentFolderUrlChanged)
public:
    explicit PlayerViewModel(QObject *parent = nullptr);

    QString videoSource() const;

    // Func INVOKABLE accept QML, which called directly when the user opens a video file
    Q_INVOKABLE void openVideoFile(const QString &localUrl);

    // Getters & Setters for position of cursor player, duration of time video, mode is play or pause
    qint64 position() const { return m_position; }
    void setPosition(qint64 pos);

    qint64 duration() const { return m_duration; }
    void setDuration(qint64 dur);

    bool isPlaying() const { return m_isPlaying; }
    void setIsPlaying(bool playing);

    Q_INVOKABLE QString formatTime(qint64 ms) const;

    Q_INVOKABLE void seekForward();
    Q_INVOKABLE void seekBackward();

    Q_INVOKABLE void nextVideo();
    Q_INVOKABLE void previousVideo();

    float volume() const { return m_volume; }
    void setVolume(float vol);
    bool hasPlaylist() const { return !m_playlist.isEmpty(); }
    Q_INVOKABLE void openMultipleFiles(const QList<QUrl> &urls);
    Q_INVOKABLE void openFolder(const QUrl &folderUrl);
    QUrl currentFolderUrl() const { return m_currentFolderUrl; }
signals:
    void videoSourceChanged();
    void positionChanged();
    void durationChanged();
    void isPlayingChanged();
    void volumeChanged();
    void playlistChanged();
    void currentFolderUrlChanged();

private:
    VideoModel *m_videoModel;
    qint64 m_position = 0;
    qint64 m_duration = 0;
    bool m_isPlaying = false;

    float m_volume = 1.0f;
    QStringList m_playlist;
    int m_currentIndex = -1;
    QMutex m_mutex;
    QUrl m_currentFolderUrl;
};

#endif // PLAYERVIEWMODEL_H
