#pragma once
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QStackedWidget>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>

class SidebarWidget;
class DashboardPage;
class RecordingsPage;
class ScreenshotsPage;
class SettingsPage;
class HotkeysPage;
class ToolsPage;
class StreamingPage;
class RecordingSettingsPanel;
class FloatingWidget;
class Recorder;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private slots:
    void onPageSelected(int index);
    void onStartRecording();
    void onStopRecording();
    void onPauseRecording();
    void onTakeScreenshot();
    void onRecordingTimer();
    void onTrayActivated(QSystemTrayIcon::ActivationReason reason);
    void onToggleTheme();

private:
    void setupUI();
    void setupTray();
    void setupHotkeys();
    void setupStatusBar();
    void updateWindowTitle();

    // Layout
    QWidget* m_centralWidget;
    QHBoxLayout* m_mainLayout;
    SidebarWidget* m_sidebar;
    QStackedWidget* m_pageStack;
    QFrame* m_contentFrame;
    RecordingSettingsPanel* m_settingsPanel;

    // Pages
    DashboardPage* m_dashboardPage;
    RecordingsPage* m_recordingsPage;
    ScreenshotsPage* m_screenshotsPage;
    SettingsPage* m_settingsPage;
    HotkeysPage* m_hotkeysPage;
    ToolsPage* m_toolsPage;
    StreamingPage* m_streamingPage;

    // System tray
    QSystemTrayIcon* m_trayIcon;
    QMenu* m_trayMenu;

    // Recording
    Recorder* m_recorder;
    FloatingWidget* m_floatingWidget;
    QTimer* m_recordingTimer;
    QDateTime m_recordingStart;
    bool m_isRecording = false;
    bool m_isPaused = false;

    // Window drag
    bool m_dragging = false;
    QPoint m_dragStartPos;
};
