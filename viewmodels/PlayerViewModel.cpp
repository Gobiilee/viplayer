#include "PlayerViewModel.h"
#include <QFileInfo>
#include <QPointer>

PlayerViewModel::PlayerViewModel(QObject *parent) : QObject(parent) {
    // 1. Apply Parent-Child Hierarchy: Input 'this' to Constructor.
    // When PlayerViewModel was destroyed, m_videoModel will be automatically cancelled by default.
    m_videoModel = new VideoModel(this);

    // 2. Apply Observer Pattern: listenning data changed from Model by Signal-Slot
    connect(m_videoModel, &VideoModel::dataChanged, this, &PlayerViewModel::videoSourceChanged);
}

QString PlayerViewModel::videoSource() const {
    return m_videoModel->filePath();
}

void PlayerViewModel::openVideoFile(const QString &localUrl) {
    // Command only need to change data
    // the connect function above will automatically trigger it when the Model changes!
    m_videoModel->setVideoData(localUrl);
    setIsPlaying(false);
}

void PlayerViewModel::setPosition(qint64 pos) {
    if (m_position != pos) {
        m_position = pos;
        emit positionChanged();
    }
}

void PlayerViewModel::setDuration(qint64 dur) {
    if (m_duration != dur) {
        m_duration = dur;
        emit durationChanged();
    }
}

void PlayerViewModel::setIsPlaying(bool playing) {
    if (m_isPlaying != playing) {
        m_isPlaying = playing;
        emit isPlayingChanged();
    }
}

QString PlayerViewModel::formatTime(qint64 ms) const {
    QTime time((ms / 3600000) % 24, (ms / 60000) % 60, (ms / 1000) % 60, ms % 1000);
    QString format = ms >= 3600000 ? "hh:mm:ss" : "mm:ss";
    return time.toString(format);
}

void PlayerViewModel::seekForward() {
    qint64 targetPos = m_position + 10000; // +10s
    if (targetPos > m_duration) targetPos = m_duration;
    setPosition(targetPos);
}

void PlayerViewModel::seekBackward() {
    qint64 targetPos = m_position - 10000; // -10s
    if (targetPos < 0) targetPos = 0;
    setPosition(targetPos);
}
void PlayerViewModel::nextVideo() {
    QMutexLocker locker(&m_mutex); 

    if (m_playlist.isEmpty()) return;

    m_currentIndex = (m_currentIndex + 1) % m_playlist.size();
    m_videoModel->setVideoData(m_playlist[m_currentIndex]);
    setIsPlaying(true);
}

void PlayerViewModel::previousVideo() {
    QMutexLocker locker(&m_mutex);

    if (m_playlist.isEmpty()) return;

    m_currentIndex = (m_currentIndex - 1 + m_playlist.size()) % m_playlist.size();
    m_videoModel->setVideoData(m_playlist[m_currentIndex]);
    setIsPlaying(true);
}

void PlayerViewModel::setVolume(float vol) {
    if (m_volume != vol) {
        m_volume = vol;
        emit volumeChanged();
    }
}
void PlayerViewModel::openMultipleFiles(const QList<QUrl> &urls) {
    QMutexLocker locker(&m_mutex);
    m_playlist.clear();
    for (const QUrl &url : urls) {
        m_playlist.append(url.toLocalFile()); // Convert QML URL to standard C++ Path string
    }

    if (!m_playlist.isEmpty()) {
        m_currentIndex = 0;
        m_videoModel->setVideoData(m_playlist[m_currentIndex]);
        QString firstFilePath = m_playlist[m_currentIndex];
        QString parentDirPath = QFileInfo(firstFilePath).absolutePath();

        m_currentFolderUrl = QUrl::fromLocalFile(parentDirPath);
        emit currentFolderUrlChanged();

        setIsPlaying(true);
        emit playlistChanged();
    }
}

void PlayerViewModel::openFolder(const QUrl &folderUrl) {
    QString localDirPath = folderUrl.toLocalFile();
    if (localDirPath.isEmpty()) return;

    // 1. Create a weak guarded pointer of 'this' before entering the concurrent thread
    QPointer<PlayerViewModel> safePointer(this);

    auto future = QtConcurrent::run([this, localDirPath, safePointer]() {
        QDir directory(localDirPath);
        QStringList filters = {"*.mp4", "*.mkv", "*.avi"};
        QFileInfoList fileInfoList = directory.entryInfoList(filters, QDir::Files);

        QStringList tempPlaylist;
        for (const QFileInfo &fileInfo : qAsConst(fileInfoList)) {
            tempPlaylist.append(fileInfo.absoluteFilePath());
        }

        // Pass the guarded safePointer into the main-thread execution context
        QMetaObject::invokeMethod(this, [safePointer, tempPlaylist]() {

            // 2. SATISFY BOTH CHECKS: Check the smart pointer, NOT the native 'this' keyword
            if (safePointer.isNull()) return;

            // Access members safely via the smart guarded pointer
            {
                QMutexLocker locker(&(safePointer->m_mutex));
                safePointer->m_playlist = tempPlaylist;
                if (safePointer->m_playlist.isEmpty()) return;
                safePointer->m_currentIndex = 0;
            }

            if (safePointer->m_videoModel) {
                safePointer->m_videoModel->setVideoData(safePointer->m_playlist[safePointer->m_currentIndex]);
            }

            safePointer->setIsPlaying(true);
            emit safePointer->playlistChanged();
        }, Qt::QueuedConnection);
    });
}
