#include "settings_page.h"
#include "../settings_manager.h"
#include "../theme_manager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QFrame>
#include <QMessageBox>
#include <QApplication>

SettingsPage::SettingsPage(QWidget* parent) : QWidget(parent) {
    setupUI();
    loadSettings();
}

void SettingsPage::setupUI() {
    auto* outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(24, 24, 24, 24);

    auto* scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    outerLayout->addWidget(scroll);

    auto* content = new QWidget();
    scroll->setWidget(content);
    auto* layout = new QVBoxLayout(content);
    layout->setSpacing(16);
    layout->setContentsMargins(0, 0, 0, 0);

    // Title
    auto* titleLabel = new QLabel("⚙  Settings");
    QFont tf("Segoe UI", 18, QFont::Bold);
    titleLabel->setFont(tf);
    titleLabel->setStyleSheet("color: #F1F5F9;");
    layout->addWidget(titleLabel);

    auto addGroup = [&](const QString& title) -> QVBoxLayout* {
        auto* grp = new QGroupBox(title, content);
        grp->setStyleSheet("QGroupBox { border: 1px solid #1E1B3A; border-radius: 12px;"
                           "margin-top: 12px; padding: 12px; color: #7C3AED; font-weight: bold; }"
                           "QGroupBox::title { subcontrol-origin: margin; left: 12px; padding: 0 6px; }");
        auto* gl = new QVBoxLayout(grp);
        gl->setSpacing(12);
        layout->addWidget(grp);
        return gl;
    };

    // General
    auto* genLayout = addGroup("General");
    m_minimizeTrayCheck = new QCheckBox("Minimize to system tray on close");
    m_startBootCheck = new QCheckBox("Start FrameX on system startup");
    m_floatingWidgetCheck = new QCheckBox("Show floating control widget during recording");
    genLayout->addWidget(m_minimizeTrayCheck);
    genLayout->addWidget(m_startBootCheck);
    genLayout->addWidget(m_floatingWidgetCheck);

    // Appearance
    auto* appLayout = addGroup("Appearance");
    auto* themeRow = new QHBoxLayout();
    auto* themeLabel = new QLabel("Theme:");
    themeLabel->setStyleSheet("color: #CBD5E1;");
    m_themeCombo = new QComboBox();
    m_themeCombo->addItems({"Dark Mode", "Light Mode"});
    themeRow->addWidget(themeLabel);
    themeRow->addWidget(m_themeCombo);
    themeRow->addStretch();
    appLayout->addLayout(themeRow);

    auto* langRow = new QHBoxLayout();
    auto* langLabel = new QLabel("Language:");
    langLabel->setStyleSheet("color: #CBD5E1;");
    m_languageCombo = new QComboBox();
    m_languageCombo->addItems({"English", "Arabic (العربية)", "French", "Spanish", "German"});
    langRow->addWidget(langLabel);
    langRow->addWidget(m_languageCombo);
    langRow->addStretch();
    appLayout->addLayout(langRow);

    // Performance
    auto* perfLayout = addGroup("Performance");
    m_hardwareAccelCheck = new QCheckBox("Enable GPU hardware acceleration (NVENC / AMD / Intel)");
    m_autoSaveCheck = new QCheckBox("Auto-save recording on crash");
    perfLayout->addWidget(m_hardwareAccelCheck);
    perfLayout->addWidget(m_autoSaveCheck);

    // Privacy
    auto* privLayout = addGroup("Privacy & Security");
    auto* privacyCheck = new QCheckBox("Blur password fields automatically during recording");
    auto* noPassCheck = new QCheckBox("Hide content from screen capture in secure areas");
    privLayout->addWidget(privacyCheck);
    privLayout->addWidget(noPassCheck);

    // About
    auto* aboutLayout = addGroup("About FrameX");
    auto* aboutGrid = new QVBoxLayout();
    auto addAboutRow = [&](const QString& k, const QString& v) {
        auto* row = new QHBoxLayout();
        auto* kl = new QLabel(k + ":");
        kl->setStyleSheet("color: #64748B; font-size: 12px;");
        kl->setFixedWidth(120);
        auto* vl = new QLabel(v);
        vl->setStyleSheet("color: #CBD5E1; font-size: 12px;");
        row->addWidget(kl); row->addWidget(vl); row->addStretch();
        aboutGrid->addLayout(row);
    };
    addAboutRow("Version", "FrameX 1.0.0");
    addAboutRow("Developer", "ZAKI");
    addAboutRow("Instagram", "@zg22x");
    addAboutRow("TikTok", "@zg22x");
    addAboutRow("Built with", "C++ / Qt Framework / FFmpeg");
    aboutLayout->addLayout(aboutGrid);

    // Buttons
    auto* btnRow = new QHBoxLayout();
    m_saveBtn = new QPushButton("💾  Save Settings");
    m_saveBtn->setObjectName("primaryBtn");
    m_saveBtn->setFixedHeight(42);
    connect(m_saveBtn, &QPushButton::clicked, this, &SettingsPage::saveSettings);

    m_resetBtn = new QPushButton("↺  Reset to Default");
    m_resetBtn->setFixedHeight(42);
    connect(m_resetBtn, &QPushButton::clicked, [this]() {
        auto reply = QMessageBox::question(this, "Reset Settings",
            "Reset all settings to default?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            SettingsManager::instance().app = AppSettings{};
            SettingsManager::instance().recording = RecordingSettings{};
            loadSettings();
        }
    });

    btnRow->addWidget(m_saveBtn);
    btnRow->addWidget(m_resetBtn);
    btnRow->addStretch();
    layout->addLayout(btnRow);
    layout->addStretch();
}

void SettingsPage::loadSettings() {
    auto& a = SettingsManager::instance().app;
    auto& r = SettingsManager::instance().recording;
    m_minimizeTrayCheck->setChecked(a.minimizeToTray);
    m_startBootCheck->setChecked(a.startOnBoot);
    m_floatingWidgetCheck->setChecked(a.showFloatingWidget);
    m_hardwareAccelCheck->setChecked(r.gpuAcceleration);
    m_autoSaveCheck->setChecked(r.autoSave);
    m_themeCombo->setCurrentIndex(a.theme == "dark" ? 0 : 1);
}

void SettingsPage::saveSettings() {
    auto& a = SettingsManager::instance().app;
    auto& r = SettingsManager::instance().recording;
    a.minimizeToTray = m_minimizeTrayCheck->isChecked();
    a.startOnBoot = m_startBootCheck->isChecked();
    a.showFloatingWidget = m_floatingWidgetCheck->isChecked();
    r.gpuAcceleration = m_hardwareAccelCheck->isChecked();
    r.autoSave = m_autoSaveCheck->isChecked();
    a.theme = m_themeCombo->currentIndex() == 0 ? "dark" : "light";
    SettingsManager::instance().save();
}
