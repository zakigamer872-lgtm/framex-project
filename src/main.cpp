#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QPainter>
#include <QTimer>
#include <QDir>
#include <QStandardPaths>
#include <QFont>
#include <QFontDatabase>
#include "mainwindow.h"
#include "settings_manager.h"
#include "theme_manager.h"

int main(int argc, char *argv[])
{
    QApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication app(argc, argv);
    app.setApplicationName("FrameX");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("ZAKI");
    app.setOrganizationDomain("zg22x.framex.app");

    // Create app data directory
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);
    QDir().mkpath(dataPath + "/recordings");
    QDir().mkpath(dataPath + "/screenshots");
    QDir().mkpath(dataPath + "/temp");

    // Initialize settings
    SettingsManager::instance().initialize();

    // Load theme
    ThemeManager::instance().applyTheme(app);

    // Splash Screen
    QPixmap splashPix(600, 400);
    splashPix.fill(Qt::transparent);
    QPainter painter(&splashPix);
    painter.setRenderHint(QPainter::Antialiasing);

    // Background gradient
    QLinearGradient bg(0, 0, 600, 400);
    bg.setColorAt(0, QColor("#0D0D1A"));
    bg.setColorAt(1, QColor("#1A0A2E"));
    painter.fillRect(0, 0, 600, 400, bg);

    // Glow circle
    QRadialGradient glow(300, 200, 120);
    glow.setColorAt(0, QColor(160, 80, 255, 80));
    glow.setColorAt(1, QColor(0, 0, 0, 0));
    painter.fillRect(0, 0, 600, 400, glow);

    // Logo text
    painter.setPen(QColor("#C084FC"));
    QFont logoFont("Arial", 48, QFont::Bold);
    painter.setFont(logoFont);
    painter.drawText(QRect(0, 120, 600, 80), Qt::AlignCenter, "FrameX");

    // Subtitle
    painter.setPen(QColor("#94A3B8"));
    QFont subFont("Arial", 14);
    painter.setFont(subFont);
    painter.drawText(QRect(0, 200, 600, 40), Qt::AlignCenter, "Capture everything. Share anything.");

    // Developer credit
    painter.setPen(QColor("#7C3AED"));
    QFont devFont("Arial", 10);
    painter.setFont(devFont);
    painter.drawText(QRect(0, 360, 600, 30), Qt::AlignCenter, "Developed by ZAKI  |  @zg22x");

    painter.end();

    QSplashScreen splash(splashPix, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    splash.setAttribute(Qt::WA_TranslucentBackground);
    splash.show();
    app.processEvents();

    MainWindow window;

    QTimer::singleShot(2000, [&]() {
        splash.close();
        window.show();
    });

    return app.exec();
}
