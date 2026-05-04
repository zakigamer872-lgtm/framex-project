#pragma once
#include <QObject>
#include <QString>
#include <QRect>
#include <QProcess>

class ScreenCapture : public QObject {
    Q_OBJECT
public:
    explicit ScreenCapture(QObject* parent = nullptr);
    ~ScreenCapture();

    void setOutputFile(const QString& path) { m_outputFile = path; }
    void setQuality(const QString& q) { m_quality = q; }
    void setFps(int fps) { m_fps = fps; }
    void setEncoder(const QString& enc) { m_encoder = enc; }
    void setFormat(const QString& fmt) { m_format = fmt; }
    void setCaptureMode(const QString& mode) { m_captureMode = mode; }
    void setGpuAcceleration(bool enabled) { m_gpuAccel = enabled; }
    void setCaptureRegion(const QRect& region) { m_region = region; }
    void setTargetWindowId(qint64 winId) { m_windowId = winId; }

    void start();
    void stop();
    void pause();
    void resume();

    bool isRunning() const { return m_running; }

signals:
    void frameReady(const QByteArray& frameData);
    void error(const QString& msg);

private:
    void buildFFmpegCommand();
    void startFFmpeg();
    void stopFFmpeg();

    QString m_outputFile;
    QString m_quality = "1080p";
    int m_fps = 60;
    QString m_encoder = "H.264 (NVIDIA NVENC)";
    QString m_format = "MP4";
    QString m_captureMode = "fullscreen";
    bool m_gpuAccel = true;
    QRect m_region;
    qint64 m_windowId = 0;
    bool m_running = false;

    QProcess* m_ffmpegProcess = nullptr;
};
