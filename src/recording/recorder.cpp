#include "recorder.h"
#include "screen_capture.h"
#include "audio_capture.h"
#include "webcam_capture.h"

#include <QDir>
#include <QDateTime>
#include <QTimer>
#include <QStandardPaths>
#include <QRandomGenerator>
#include <QProcess>

Recorder::Recorder(QObject* parent) : QObject(parent) {
    m_statsTimer = new QTimer(this);
    m_statsTimer->setInterval(1000);
    connect(m_statsTimer, &QTimer::timeout, this, &Recorder::onStatsTick);

    m_screenCapture = new ScreenCapture(this);
    m_audioCapture = new AudioCapture(this);
    m_webcamCapture = new WebcamCapture(this);
}

Recorder::~Recorder() {
    if (m_state == Recording || m_state == Paused) {
        stopRecording();
    }
}

void Recorder::setSettings(const RecordingSettings& settings) {
    m_settings = settings;
}

void Recorder::startRecording() {
    if (m_state != Idle) return;

    buildOutputPath();
    m_startTime = QDateTime::currentDateTime();

    // Configure screen capture
    m_screenCapture->setOutputFile(m_lastFilePath);
    m_screenCapture->setQuality(m_settings.videoQuality);
    m_screenCapture->setFps(m_settings.fps);
    m_screenCapture->setEncoder(m_settings.encoder);
    m_screenCapture->setFormat(m_settings.outputFormat);
    m_screenCapture->setCaptureMode(m_settings.captureMode);
    m_screenCapture->setGpuAcceleration(m_settings.gpuAcceleration);

    // Configure audio
    m_audioCapture->setSystemAudio(m_settings.systemAudio);
    m_audioCapture->setMicrophone(m_settings.microphone);

    // Configure webcam
    if (m_settings.webcamEnabled) {
        m_webcamCapture->setDevice(m_settings.webcamDevice);
        m_webcamCapture->start();
    }

    m_screenCapture->start();
    if (m_settings.systemAudio || m_settings.microphone) {
        m_audioCapture->start();
    }

    m_state = Recording;
    m_diskUsed = 0;
    m_statsTimer->start();
    emit recordingStarted();
}

void Recorder::stopRecording() {
    if (m_state == Idle) return;
    m_statsTimer->stop();

    m_screenCapture->stop();
    m_audioCapture->stop();
    if (m_settings.webcamEnabled) m_webcamCapture->stop();

    m_state = Idle;
    emit recordingStopped(m_lastFilePath);
}

void Recorder::pauseRecording() {
    if (m_state != Recording) return;
    m_screenCapture->pause();
    m_audioCapture->pause();
    m_state = Paused;
    emit recordingPaused();
}

void Recorder::resumeRecording() {
    if (m_state != Paused) return;
    m_screenCapture->resume();
    m_audioCapture->resume();
    m_state = Recording;
    emit recordingResumed();
}

qint64 Recorder::elapsedSeconds() const {
    if (m_state == Idle) return 0;
    return m_startTime.secsTo(QDateTime::currentDateTime());
}

void Recorder::onStatsTick() {
    // Simulate realistic stats (in real impl, read from system APIs)
    m_cpuUsage = 15.0 + QRandomGenerator::global()->bounded(10);
    m_gpuUsage = 35.0 + QRandomGenerator::global()->bounded(15);
    m_currentFps = m_settings.fps;
    m_diskUsed += m_settings.fps * 500000 / m_settings.fps; // ~500KB/s at 60fps approximation

    emit statsUpdated(m_cpuUsage, m_gpuUsage, m_currentFps, m_diskUsed);
}

void Recorder::buildOutputPath() {
    QString dir = m_settings.savePath;
    if (dir.isEmpty()) {
        dir = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + "/FrameX Recordings";
    }
    QDir().mkpath(dir);

    QString ext = m_settings.outputFormat.toLower();
    if (ext == "mkv") ext = "mkv";
    else if (ext == "avi") ext = "avi";
    else ext = "mp4";

    m_lastFilePath = dir + "/" + generateFileName() + "." + ext;
}

QString Recorder::generateFileName() const {
    return "Recording_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");
}
