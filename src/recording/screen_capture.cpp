#include "screen_capture.h"
#include <QProcess>
#include <QScreen>
#include <QApplication>
#include <QStringList>
#include <QDebug>

ScreenCapture::ScreenCapture(QObject* parent) : QObject(parent) {
    m_ffmpegProcess = new QProcess(this);
    connect(m_ffmpegProcess, &QProcess::errorOccurred, [this](QProcess::ProcessError err) {
        Q_UNUSED(err)
        emit error("FFmpeg process error: " + m_ffmpegProcess->errorString());
    });
}

ScreenCapture::~ScreenCapture() {
    stop();
}

void ScreenCapture::start() {
    if (m_running) return;
    buildFFmpegCommand();
    m_running = true;
}

void ScreenCapture::stop() {
    if (!m_running) return;
    stopFFmpeg();
    m_running = false;
}

void ScreenCapture::pause() {
    // Send pause signal to FFmpeg
#ifdef Q_OS_WIN
    if (m_ffmpegProcess && m_ffmpegProcess->state() == QProcess::Running) {
        m_ffmpegProcess->write("p");
    }
#endif
}

void ScreenCapture::resume() {
    // Send resume signal to FFmpeg
#ifdef Q_OS_WIN
    if (m_ffmpegProcess && m_ffmpegProcess->state() == QProcess::Running) {
        m_ffmpegProcess->write("p");
    }
#endif
}

void ScreenCapture::buildFFmpegCommand() {
    QStringList args;

    // Resolution mapping
    QString size;
    if (m_quality == "720p") size = "1280x720";
    else if (m_quality == "1080p") size = "1920x1080";
    else if (m_quality == "2K") size = "2560x1440";
    else if (m_quality == "4K") size = "3840x2160";
    else size = "1920x1080";

    // Encoder selection
    QString videoCodec;
    bool isNVENC = m_encoder.contains("NVENC");
    bool isAMD = m_encoder.contains("AMD");
    bool isIntel = m_encoder.contains("Intel");
    bool isH265 = m_encoder.contains("H.265") || m_encoder.contains("HEVC");

    if (isNVENC) {
        videoCodec = isH265 ? "hevc_nvenc" : "h264_nvenc";
    } else if (isAMD) {
        videoCodec = isH265 ? "hevc_amf" : "h264_amf";
    } else if (isIntel) {
        videoCodec = isH265 ? "hevc_qsv" : "h264_qsv";
    } else {
        videoCodec = isH265 ? "libx265" : "libx264";
    }

#ifdef Q_OS_WIN
    // Windows GDI/DXGI capture
    args << "-f" << "gdigrab";
    if (m_captureMode == "fullscreen") {
        args << "-i" << "desktop";
    } else if (m_captureMode == "window") {
        args << "-i" << "title=" + QString::number(m_windowId);
    } else {
        // Custom region
        args << "-offset_x" << QString::number(m_region.x())
             << "-offset_y" << QString::number(m_region.y())
             << "-video_size" << QString("%1x%2").arg(m_region.width()).arg(m_region.height())
             << "-i" << "desktop";
    }
    args << "-framerate" << QString::number(m_fps);
#elif defined(Q_OS_LINUX)
    args << "-f" << "x11grab"
         << "-r" << QString::number(m_fps)
         << "-i" << ":0.0";
#elif defined(Q_OS_MAC)
    args << "-f" << "avfoundation"
         << "-r" << QString::number(m_fps)
         << "-i" << "1:0";
#endif

    // Video encoding options
    args << "-vcodec" << videoCodec;
    args << "-s" << size;
    args << "-r" << QString::number(m_fps);

    if (m_gpuAccel && (isNVENC || isAMD || isIntel)) {
        args << "-preset" << "p4" // NVENC fast preset
             << "-b:v" << "8M"
             << "-maxrate" << "10M";
    } else {
        args << "-preset" << "ultrafast"
             << "-crf" << "18";
    }

    // Output format
    if (m_format == "MKV") args << "-f" << "matroska";
    else if (m_format == "AVI") args << "-f" << "avi";
    else args << "-f" << "mp4" << "-movflags" << "+faststart";

    args << m_outputFile;

    qDebug() << "FFmpeg command: ffmpeg" << args.join(" ");
    startFFmpeg();
}

void ScreenCapture::startFFmpeg() {
    m_ffmpegProcess->start("ffmpeg", QStringList() << "-y" << "-loglevel" << "error");
}

void ScreenCapture::stopFFmpeg() {
    if (m_ffmpegProcess && m_ffmpegProcess->state() == QProcess::Running) {
        m_ffmpegProcess->write("q");
        if (!m_ffmpegProcess->waitForFinished(5000)) {
            m_ffmpegProcess->kill();
        }
    }
}
