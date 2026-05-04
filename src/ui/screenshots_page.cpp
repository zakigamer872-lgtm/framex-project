#include "screenshots_page.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QPixmap>
#include <QLabel>
#include <QPushButton>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QUrl>

ScreenshotsPage::ScreenshotsPage(QWidget* parent) : QWidget(parent) {
    setupUI();
    loadScreenshots();
}

void ScreenshotsPage::setupUI() {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(16);

    // Header
    auto* headerRow = new QHBoxLayout();
    auto* titleLabel = new QLabel("🖼  Screenshots");
    QFont tf("Segoe UI", 18, QFont::Bold);
    titleLabel->setFont(tf);
    titleLabel->setStyleSheet("color: #F1F5F9;");

    m_countLabel = new QLabel("0 screenshots");
    m_countLabel->setStyleSheet("color: #64748B;");

    auto* openFolderBtn = new QPushButton("📁  Open Folder");
    openFolderBtn->setFixedHeight(38);
    connect(openFolderBtn, &QPushButton::clicked, [this]() {
        QString dir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/FrameX Screenshots";
        QDesktopServices::openUrl(QUrl::fromLocalFile(dir));
    });

    headerRow->addWidget(titleLabel);
    headerRow->addWidget(m_countLabel);
    headerRow->addStretch();
    headerRow->addWidget(openFolderBtn);
    layout->addLayout(headerRow);

    // Grid
    auto* scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);

    m_gridWidget = new QWidget();
    m_gridLayout = new QGridLayout(m_gridWidget);
    m_gridLayout->setSpacing(12);

    scroll->setWidget(m_gridWidget);
    layout->addWidget(scroll);

    m_emptyLabel = new QLabel("No screenshots yet.\nPress Ctrl+Shift+S to take a screenshot.");
    m_emptyLabel->setAlignment(Qt::AlignCenter);
    m_emptyLabel->setStyleSheet("color: #475569; font-size: 14px;");
    layout->addWidget(m_emptyLabel);
}

void ScreenshotsPage::loadScreenshots() {
    // Clear grid
    while (QLayoutItem* item = m_gridLayout->takeAt(0)) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    QString dir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/FrameX Screenshots";
    QDir ssDir(dir);
    QStringList files = ssDir.entryList({"*.png", "*.jpg"}, QDir::Files, QDir::Time);

    m_countLabel->setText(QString("%1 screenshot%2").arg(files.size()).arg(files.size() == 1 ? "" : "s"));
    m_emptyLabel->setVisible(files.isEmpty());
    m_gridWidget->setVisible(!files.isEmpty());

    int col = 0, row = 0;
    for (const QString& file : files) {
        auto* card = new QFrame(m_gridWidget);
        card->setFixedSize(200, 160);
        card->setStyleSheet("QFrame { background: #12122A; border: 1px solid #1E1B3A; border-radius: 10px; }");
        card->setCursor(Qt::PointingHandCursor);

        auto* cl = new QVBoxLayout(card);
        cl->setContentsMargins(6, 6, 6, 6);
        cl->setSpacing(4);

        QPixmap px(dir + "/" + file);
        auto* imgLabel = new QLabel();
        imgLabel->setPixmap(px.scaled(188, 118, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        imgLabel->setAlignment(Qt::AlignCenter);

        auto* nameLabel = new QLabel(QFileInfo(file).baseName());
        nameLabel->setStyleSheet("color: #94A3B8; font-size: 10px;");
        nameLabel->setAlignment(Qt::AlignCenter);

        cl->addWidget(imgLabel);
        cl->addWidget(nameLabel);

        QString filePath = dir + "/" + file;
        connect(card, &QFrame::destroyed, []{});

        m_gridLayout->addWidget(card, row, col);
        col++;
        if (col >= 4) { col = 0; row++; }
    }
}

void ScreenshotsPage::refresh() { loadScreenshots(); }
