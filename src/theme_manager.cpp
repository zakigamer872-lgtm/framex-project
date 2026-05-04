#include "theme_manager.h"
#include "settings_manager.h"
#include <QFile>

void ThemeManager::applyTheme(QApplication& app, Theme t) {
    m_theme = t;
    app.setStyleSheet(darkStyleSheet());
}

void ThemeManager::setTheme(Theme t) {
    m_theme = t;
}

QString ThemeManager::darkStyleSheet() {
    return R"(
QMainWindow, QDialog, QWidget {
    background-color: #0D0D1A;
    color: #E2E8F0;
    font-family: 'Segoe UI', Arial, sans-serif;
    font-size: 13px;
}
QFrame#sidebar {
    background-color: #0A0A16;
    border-right: 1px solid #1E1B3A;
}
QFrame#contentArea {
    background-color: #0D0D1A;
}
QFrame#settingsPanel {
    background-color: #0F0F1F;
    border-left: 1px solid #1E1B3A;
}
QPushButton {
    background-color: #1A1A2E;
    color: #CBD5E1;
    border: 1px solid #2D2B55;
    border-radius: 8px;
    padding: 8px 16px;
    font-size: 13px;
}
QPushButton:hover {
    background-color: #252545;
    border-color: #7C3AED;
    color: #E2E8F0;
}
QPushButton:pressed {
    background-color: #7C3AED;
    border-color: #9333EA;
}
QPushButton#recordBtn {
    background: qradialgradient(cx:0.5, cy:0.5, radius:0.5,
        fx:0.5, fy:0.5,
        stop:0 #EF4444,
        stop:0.6 #DC2626,
        stop:1 #B91C1C);
    border: 3px solid #F87171;
    border-radius: 50px;
    color: white;
    font-size: 18px;
    font-weight: bold;
    min-width: 100px;
    min-height: 100px;
}
QPushButton#recordBtn:hover {
    background: qradialgradient(cx:0.5, cy:0.5, radius:0.5,
        fx:0.5, fy:0.5,
        stop:0 #F87171,
        stop:0.6 #EF4444,
        stop:1 #DC2626);
    border-color: #FCA5A5;
}
QPushButton#primaryBtn {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
        stop:0 #7C3AED, stop:1 #9333EA);
    color: white;
    border: none;
    border-radius: 8px;
    padding: 10px 20px;
    font-weight: bold;
}
QPushButton#primaryBtn:hover {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
        stop:0 #6D28D9, stop:1 #7C3AED);
}
QPushButton#captureTypeBtn {
    background-color: #12122A;
    border: 1px solid #2D2B55;
    border-radius: 10px;
    color: #94A3B8;
    padding: 12px 16px;
    text-align: left;
    font-size: 13px;
}
QPushButton#captureTypeBtn:hover {
    background-color: #1A1A3E;
    border-color: #7C3AED;
    color: #E2E8F0;
}
QPushButton#captureTypeBtn:checked {
    background-color: #1E1050;
    border-color: #7C3AED;
    border-width: 2px;
    color: #C084FC;
}
QPushButton#sidebarBtn {
    background: transparent;
    border: none;
    border-radius: 10px;
    color: #64748B;
    padding: 12px 16px;
    text-align: left;
    font-size: 13px;
}
QPushButton#sidebarBtn:hover {
    background-color: #1A1A2E;
    color: #C084FC;
}
QPushButton#sidebarBtn:checked {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
        stop:0 #1E1050, stop:1 #12122A);
    color: #C084FC;
    border-left: 3px solid #7C3AED;
}
QLabel {
    color: #E2E8F0;
    background: transparent;
}
QLabel#titleLabel {
    font-size: 22px;
    font-weight: bold;
    color: #F1F5F9;
}
QLabel#subtitleLabel {
    font-size: 13px;
    color: #64748B;
}
QLabel#sectionLabel {
    font-size: 11px;
    font-weight: bold;
    color: #475569;
    text-transform: uppercase;
    letter-spacing: 1px;
}
QComboBox {
    background-color: #12122A;
    border: 1px solid #2D2B55;
    border-radius: 8px;
    padding: 8px 12px;
    color: #E2E8F0;
    font-size: 13px;
    min-height: 36px;
}
QComboBox:hover {
    border-color: #7C3AED;
}
QComboBox::drop-down {
    border: none;
    width: 24px;
}
QComboBox QAbstractItemView {
    background-color: #12122A;
    border: 1px solid #2D2B55;
    selection-background-color: #7C3AED;
    color: #E2E8F0;
    border-radius: 8px;
}
QSlider::groove:horizontal {
    height: 4px;
    background: #2D2B55;
    border-radius: 2px;
}
QSlider::handle:horizontal {
    background: #7C3AED;
    border: 2px solid #9333EA;
    width: 16px;
    height: 16px;
    margin: -6px 0;
    border-radius: 8px;
}
QSlider::sub-page:horizontal {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
        stop:0 #7C3AED, stop:1 #C084FC);
    border-radius: 2px;
}
QCheckBox {
    color: #CBD5E1;
    spacing: 8px;
}
QCheckBox::indicator {
    width: 18px;
    height: 18px;
    border-radius: 4px;
    border: 1px solid #2D2B55;
    background: #12122A;
}
QCheckBox::indicator:checked {
    background: #7C3AED;
    border-color: #9333EA;
}
QLineEdit, QTextEdit {
    background-color: #12122A;
    border: 1px solid #2D2B55;
    border-radius: 8px;
    padding: 8px 12px;
    color: #E2E8F0;
    selection-background-color: #7C3AED;
}
QLineEdit:focus, QTextEdit:focus {
    border-color: #7C3AED;
}
QScrollBar:vertical {
    background: transparent;
    width: 6px;
    margin: 0;
}
QScrollBar::handle:vertical {
    background: #2D2B55;
    border-radius: 3px;
    min-height: 30px;
}
QScrollBar::handle:vertical:hover {
    background: #7C3AED;
}
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
QScrollBar:horizontal {
    background: transparent;
    height: 6px;
}
QScrollBar::handle:horizontal {
    background: #2D2B55;
    border-radius: 3px;
}
QTabWidget::pane {
    border: 1px solid #2D2B55;
    border-radius: 10px;
    background: #0F0F1F;
}
QTabBar::tab {
    background: #12122A;
    color: #64748B;
    border: none;
    padding: 10px 20px;
    border-radius: 8px;
    margin-right: 4px;
}
QTabBar::tab:selected {
    background: #7C3AED;
    color: white;
}
QProgressBar {
    background: #1A1A2E;
    border: none;
    border-radius: 4px;
    height: 8px;
    text-align: center;
}
QProgressBar::chunk {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
        stop:0 #7C3AED, stop:1 #C084FC);
    border-radius: 4px;
}
QGroupBox {
    border: 1px solid #1E1B3A;
    border-radius: 10px;
    margin-top: 16px;
    padding: 16px;
    color: #94A3B8;
    font-size: 12px;
    font-weight: bold;
}
QGroupBox::title {
    subcontrol-origin: margin;
    left: 12px;
    padding: 0 6px;
    color: #7C3AED;
}
QTableWidget {
    background: #0F0F1F;
    border: 1px solid #1E1B3A;
    border-radius: 10px;
    gridline-color: #1E1B3A;
    color: #CBD5E1;
}
QTableWidget::item:selected {
    background: #1E1050;
    color: #C084FC;
}
QHeaderView::section {
    background: #12122A;
    color: #64748B;
    border: none;
    border-bottom: 1px solid #1E1B3A;
    padding: 10px;
    font-weight: bold;
    font-size: 11px;
}
QMenu {
    background: #12122A;
    border: 1px solid #2D2B55;
    border-radius: 8px;
    padding: 4px;
}
QMenu::item {
    padding: 8px 20px;
    border-radius: 6px;
    color: #CBD5E1;
}
QMenu::item:selected {
    background: #7C3AED;
    color: white;
}
QToolTip {
    background: #1E1B3A;
    color: #E2E8F0;
    border: 1px solid #2D2B55;
    border-radius: 6px;
    padding: 6px 10px;
}
QSplitter::handle {
    background: #1E1B3A;
    width: 1px;
}
)";
}

QString ThemeManager::lightStyleSheet() {
    return R"(
QMainWindow, QDialog, QWidget {
    background-color: #F8FAFC;
    color: #1E293B;
    font-family: 'Segoe UI', Arial, sans-serif;
    font-size: 13px;
}
QFrame#sidebar {
    background-color: #FFFFFF;
    border-right: 1px solid #E2E8F0;
}
QPushButton {
    background-color: #F1F5F9;
    color: #334155;
    border: 1px solid #E2E8F0;
    border-radius: 8px;
    padding: 8px 16px;
}
QPushButton:hover {
    background-color: #EDE9FE;
    border-color: #7C3AED;
}
QPushButton#sidebarBtn {
    background: transparent;
    border: none;
    text-align: left;
    color: #64748B;
}
QPushButton#sidebarBtn:checked {
    background: #EDE9FE;
    color: #7C3AED;
    border-left: 3px solid #7C3AED;
}
QLabel { color: #1E293B; background: transparent; }
QComboBox {
    background: #F1F5F9;
    border: 1px solid #E2E8F0;
    border-radius: 8px;
    padding: 8px 12px;
    color: #1E293B;
}
)";
}
