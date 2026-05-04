#include "mainwindow.h"
#include "ui/sidebar_widget.h"
#include "ui/dashboard_page.h"
#include "ui/recordings_page.h"
#include "ui/screenshots_page.h"
#include "ui/settings_page.h"
#include "ui/hotkeys_page.h"
#include "ui/tools_page.h"
#include "ui/streaming_page.h"
#include "ui/recording_settings_panel.h"
#include "ui/floating_widget.h"
#include "recording/recorder.h"
#include "settings_manager.h"
#include "hotkey_manager.h"
#include "history_manager.h"

#include <QApplication>
#include <QCloseEvent>
#include <QMenu>
#include <QAction>
#include <QTimer>
#include <QDateTime>
#include <QDir>
#include <QMessageBox>
#include <QScreen>
#include <QShortcut>
#include <QStatusBar>
#include <QLabel>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("FrameX");
    setMinimumSize(1100, 700);
    resize(1280, 780);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground, false);

    // Center on screen
    QScreen* screen = QApplication::primaryScreen();
    QRect sg = screen->availableGeometry();
    move((sg.width() - width()) / 2, (sg.height() - height()) / 2);

    m_recorder = new Recorder(this);
    m_recordingTimer = new QTimer(this);
    connect(m_recordingTimer, &QTimer::timeout, this, &MainWindow::onRecordingTimer);

    setupUI();
    setupTray();
    setupHotkeys();

    // Floating widget
    m_floatingWidget = new FloatingWidget(nullptr);
    m_floatingWidget->hide();
    connect(m_floatingWidget, &FloatingWidget::stopRequested, this, &MainWindow::onStopRecording);
    connect(m_floatingWidget, &FloatingWidget::pauseRequested, this, &MainWindow::onPauseRecording);
    connect(m_floatingWidget, &FloatingWidget::screenshotRequested, this, &MainWindow::onTakeScreenshot);
}

MainWindow::~MainWindow() {
    SettingsManager::instance().save();
}

void MainWindow::setupUI() {
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    m_mainLayout = new QHBoxLayout(m_centralWidget);
    m_mainLayout->setSpacing(0);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);

    // Sidebar
    m_sidebar = new SidebarWidget(this);
    m_sidebar->setFixedWidth(220);
    m_sidebar->setObjectName("sidebar");
    connect(m_sidebar, &SidebarWidget::pageSelected, this, &MainWindow::onPageSelected);
    connect(m_sidebar, &SidebarWidget::recordingRequested, this, &MainWindow::onStartRecording);
    connect(m_sidebar, &SidebarWidget::themeToggleRequested, this, &MainWindow::onToggleTheme);

    // Content area
    m_contentFrame = new QFrame(this);
    m_contentFrame->setObjectName("contentArea");

    QHBoxLayout* contentLayout = new QHBoxLayout(m_contentFrame);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    // Pages
    m_pageStack = new QStackedWidget(this);

    m_dashboardPage = new DashboardPage(m_recorder, this);
    m_recordingsPage = new RecordingsPage(this);
    m_streamingPage = new StreamingPage(this);
    m_screenshotsPage = new ScreenshotsPage(this);
    m_settingsPage = new SettingsPage(this);
    m_hotkeysPage = new HotkeysPage(this);
    m_toolsPage = new ToolsPage(this);

    m_pageStack->addWidget(m_dashboardPage);     // 0
    m_pageStack->addWidget(m_recordingsPage);    // 1
    m_pageStack->addWidget(m_streamingPage);     // 2
    m_pageStack->addWidget(m_screenshotsPage);   // 3
    m_pageStack->addWidget(m_settingsPage);      // 4
    m_pageStack->addWidget(m_hotkeysPage);       // 5
    m_pageStack->addWidget(m_toolsPage);         // 6

    // Recording settings panel (right side)
    m_settingsPanel = new RecordingSettingsPanel(this);
    m_settingsPanel->setFixedWidth(280);
    m_settingsPanel->setObjectName("settingsPanel");
    connect(m_settingsPanel, &RecordingSettingsPanel::settingsChanged,
            m_dashboardPage, &DashboardPage::onSettingsChanged);

    contentLayout->addWidget(m_pageStack, 1);
    contentLayout->addWidget(m_settingsPanel, 0);

    m_mainLayout->addWidget(m_sidebar, 0);
    m_mainLayout->addWidget(m_contentFrame, 1);

    // Connect dashboard
    connect(m_dashboardPage, &DashboardPage::startRecordingRequested,
            this, &MainWindow::onStartRecording);
    connect(m_dashboardPage, &DashboardPage::screenshotRequested,
            this, &MainWindow::onTakeScreenshot);
}

void MainWindow::setupTray() {
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setToolTip("FrameX - Screen Recorder");

    QMenu* trayMenu = new QMenu(this);
    QAction* showAction = trayMenu->addAction("Show FrameX");
    trayMenu->addSeparator();
    QAction* recAction = trayMenu->addAction("Start Recording");
    QAction* ssAction = trayMenu->addAction("Take Screenshot");
    trayMenu->addSeparator();
    QAction* quitAction = trayMenu->addAction("Quit");

    connect(showAction, &QAction::triggered, this, &QMainWindow::show);
    connect(recAction, &QAction::triggered, this, &MainWindow::onStartRecording);
    connect(ssAction, &QAction::triggered, this, &MainWindow::onTakeScreenshot);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayActivated);

    m_trayIcon->setContextMenu(trayMenu);
    m_trayIcon->show();
}

void MainWindow::setupHotkeys() {
    auto& hk = SettingsManager::instance().hotkeys;
    HotkeyManager::instance().registerHotkey("start_stop", hk.startStop, [this]() {
        if (m_isRecording) onStopRecording();
        else onStartRecording();
    });
    HotkeyManager::instance().registerHotkey("pause_resume", hk.pauseResume, [this]() {
        if (m_isRecording) onPauseRecording();
    });
    HotkeyManager::instance().registerHotkey("screenshot", hk.screenshot, [this]() {
        onTakeScreenshot();
    });
}

void MainWindow::onPageSelected(int index) {
    m_pageStack->setCurrentIndex(index);
    // Show/hide settings panel only on dashboard
    m_settingsPanel->setVisible(index == 0);
}

void MainWindow::onStartRecording() {
    if (m_isRecording) return;

    auto& s = SettingsManager::instance().recording;
    m_recorder->setSettings(s);

    // Hide main window before recording
    if (SettingsManager::instance().app.minimizeToTray) {
        hide();
        QTimer::singleShot(300, [this]() {
            m_recorder->startRecording();
        });
    } else {
        m_recorder->startRecording();
    }

    m_isRecording = true;
    m_isPaused = false;
    m_recordingStart = QDateTime::currentDateTime();
    m_recordingTimer->start(1000);

    // Show floating widget
    if (SettingsManager::instance().app.showFloatingWidget) {
        m_floatingWidget->show();
        m_floatingWidget->startTimer();
    }

    m_trayIcon->showMessage("FrameX", "Recording started", QSystemTrayIcon::Information, 2000);
    m_sidebar->setRecordingState(true);
}

void MainWindow::onStopRecording() {
    if (!m_isRecording) return;
    m_recorder->stopRecording();
    m_isRecording = false;
    m_isPaused = false;
    m_recordingTimer->stop();
    m_floatingWidget->hide();
    m_floatingWidget->stopTimer();
    m_sidebar->setRecordingState(false);

    // Save to history
    QString filePath = m_recorder->lastFilePath();
    HistoryManager::instance().addEntry(filePath, m_recordingStart, QDateTime::currentDateTime());

    show();
    raise();
    activateWindow();

    // Refresh recordings page
    m_recordingsPage->refresh();
    m_trayIcon->showMessage("FrameX", "Recording saved: " + filePath,
                            QSystemTrayIcon::Information, 3000);
}

void MainWindow::onPauseRecording() {
    if (!m_isRecording) return;
    if (m_isPaused) {
        m_recorder->resumeRecording();
        m_floatingWidget->setPaused(false);
        m_recordingTimer->start(1000);
    } else {
        m_recorder->pauseRecording();
        m_floatingWidget->setPaused(true);
        m_recordingTimer->stop();
    }
    m_isPaused = !m_isPaused;
}

void MainWindow::onTakeScreenshot() {
    auto& s = SettingsManager::instance().recording;
    QString ssDir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/FrameX Screenshots";
    QDir().mkpath(ssDir);
    QString filename = ssDir + "/Screenshot_" +
                       QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss") + ".png";

    // Capture current screen
    QScreen* screen = QApplication::primaryScreen();
    if (screen) {
        QPixmap px = screen->grabWindow(0);
        if (px.save(filename, "PNG")) {
            m_trayIcon->showMessage("FrameX", "Screenshot saved!", QSystemTrayIcon::Information, 2000);
            m_screenshotsPage->refresh();
        }
    }
}

void MainWindow::onRecordingTimer() {
    qint64 secs = m_recordingStart.secsTo(QDateTime::currentDateTime());
    m_floatingWidget->updateTime(secs);
    m_dashboardPage->updateRecordingTime(secs);
}

void MainWindow::onTrayActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::DoubleClick) {
        show();
        raise();
        activateWindow();
    }
}

void MainWindow::onToggleTheme() {
    auto& s = SettingsManager::instance().app;
    if (s.theme == "dark") {
        s.theme = "light";
        qApp->setStyleSheet("");
    } else {
        s.theme = "dark";
        ThemeManager::instance().applyTheme(*qApp);
    }
    SettingsManager::instance().save();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (SettingsManager::instance().app.minimizeToTray) {
        hide();
        event->ignore();
        m_trayIcon->showMessage("FrameX", "FrameX is still running in the system tray.",
                                QSystemTrayIcon::Information, 2000);
    } else {
        if (m_isRecording) {
            auto reply = QMessageBox::question(this, "FrameX",
                "Recording is in progress. Stop and quit?",
                QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::No) { event->ignore(); return; }
            onStopRecording();
        }
        SettingsManager::instance().save();
        event->accept();
    }
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && event->pos().y() < 50) {
        m_dragging = true;
        m_dragStartPos = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - m_dragStartPos);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
    m_dragging = false;
    QMainWindow::mouseReleaseEvent(event);
}
