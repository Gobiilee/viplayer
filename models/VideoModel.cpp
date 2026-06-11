#include "VideoModel.h"
#include <QFileInfo>

VideoModel::VideoModel(QObject *parent)
    : QObject{parent}
{
    // Default empty value
    m_filePath = "";
    m_fileName = "";
}

void VideoModel::setVideoData(const QString &filePath) {
    if (m_filePath != filePath) {
        m_filePath = filePath;
        m_fileName = QFileInfo(filePath).fileName();

        // Active Observer Pattern: Send signals data changed to all (ViewModel)
        emit dataChanged();
    }
}

QString VideoModel::filePath() const {
    return m_filePath;
}

QString VideoModel::fileName() const {
    return m_fileName;
}
