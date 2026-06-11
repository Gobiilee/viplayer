#ifndef VIDEOMODEL_H
#define VIDEOMODEL_H

#include <QObject>

class VideoModel : public QObject
{
    Q_OBJECT
public:
    explicit VideoModel(QObject *parent = nullptr);

    // Func setter/getter update data video
    void setVideoData(const QString &filePath);

    QString filePath() const;
    QString fileName() const;

signals:
    // Send Signals when source data video change (Observer Pattern)
    void dataChanged();

private:
    QString m_filePath;
    QString m_fileName;
};

#endif // VIDEOMODEL_H
