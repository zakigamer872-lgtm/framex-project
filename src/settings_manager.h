#pragma once
#include <QSettings>
#include <QString>
#include <QKeySequence>

struct RecordingSettings {
    QString captureMode = "fullscreen"; // fullscreen, window, custom
    QString videoQuality = "1080p";
    int fps = 60;
    QString encoder = "H.264 (NVIDIA NVENC)";
    QString outputFormat = "MP4";
    QString savePath;
    bool systemAudio = true;
    bool microphone = true;
    bool webcamEnabled = false;
    QString webcamDevice;
    int webcamSize = 25; // percent
    QString webcamPosition = "bottom-right";
    bool showCursor = true;
    bool highlightCursor = false;
    bool showClicks = false;
    bool autoSave = true;
    bool gpuAcceleration = true;
};

struct HotkeySettings {
    QKeySequence startStop = QKeySequence("Ctrl+Shift+R");
    QKeySequence pauseResume = QKeySequence("Ctrl+Shift+P");
    QKeySequence screenshot = QKeySequence("Ctrl+Shift+S");
    QKeySequence cancelRecording = QKeySequence("Ctrl+Shift+X");
    QKeySequence openEditor = QKeySequence("Ctrl+Shift+E");
};

struct AppSettings {
    QString theme = "dark";
    bool minimizeToTray = true;
    bool startOnBoot = false;
    bool showFloatingWidget = true;
    bool hardwareAcceleration = true;
    QString language = "en";
};

class SettingsManager {
public:
    static SettingsManager& instance() {
        static SettingsManager inst;
        return inst;
    }

    void initialize();
    void save();
    void load();

    RecordingSettings recording;
    HotkeySettings hotkeys;
    AppSettings app;

private:
    SettingsManager() {}
    QSettings* m_settings = nullptr;
};
