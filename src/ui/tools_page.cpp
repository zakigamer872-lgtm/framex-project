#include "tools_page.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QFrame>
#include <QScrollArea>

ToolsPage::ToolsPage(QWidget* parent) : QWidget(parent) {
    setupUI();
}

void ToolsPage::setupUI() {
    auto* outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(24, 24, 24, 24);

    auto* scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    outerLayout->addWidget(scroll);

    auto* content = new QWidget();
    scroll->setWidget(content);
    auto* layout = new QVBoxLayout(content);
    layout->setSpacing(20);

    auto* titleLabel = new QLabel("🔧  Tools");
    QFont tf("Segoe UI", 18, QFont::Bold);
    titleLabel->setFont(tf);
    titleLabel->setStyleSheet("color: #F1F5F9;");
    layout->addWidget(titleLabel);

    struct Tool {
        QString icon, name, description;
    };

    QList<Tool> tools = {
        {"✂", "Video Trimmer", "Cut and trim your recordings with precision"},
        {"T+", "Add Text Overlay", "Add animated text and titles to your videos"},
        {"🎵", "Add Music", "Add background music to your recordings"},
        {"⊛", "Blur / Hide Regions", "Blur sensitive areas during or after recording"},
        {"⇄", "Format Converter", "Convert videos between MP4, MKV, AVI, and more"},
        {"🎙", "Voice to Text", "AI-powered speech transcription"},
        {"🎞", "GIF Creator", "Convert clips to animated GIFs"},
        {"📐", "Crop & Resize", "Crop and resize your video output"},
        {"🎨", "Color Correction", "Adjust brightness, contrast, and color"},
        {"✨", "Highlight Moments", "AI auto-detection of key moments"},
        {"🔊", "Audio Mixer", "Mix and adjust multiple audio tracks"},
        {"📤", "Export / Share", "Export and share to YouTube, Twitter, and more"},
    };

    auto* grid = new QGridLayout();
    grid->setSpacing(14);

    for (int i = 0; i < tools.size(); ++i) {
        auto& t = tools[i];
        auto* card = new QFrame(content);
        card->setStyleSheet("QFrame { background: #0F0F1F; border: 1px solid #1E1B3A; border-radius: 12px; }"
                            "QFrame:hover { border-color: #7C3AED; background: #12122A; }");
        card->setCursor(Qt::PointingHandCursor);
        card->setFixedHeight(110);

        auto* cl = new QVBoxLayout(card);
        cl->setContentsMargins(16, 14, 16, 14);
        cl->setSpacing(6);

        auto* iconLabel = new QLabel(t.icon);
        QFont iFont("Segoe UI", 22);
        iconLabel->setFont(iFont);

        auto* nameLabel = new QLabel(t.name);
        QFont nf("Segoe UI", 13, QFont::Bold);
        nameLabel->setFont(nf);
        nameLabel->setStyleSheet("color: #E2E8F0;");

        auto* descLabel = new QLabel(t.description);
        descLabel->setStyleSheet("color: #64748B; font-size: 11px;");
        descLabel->setWordWrap(true);

        cl->addWidget(iconLabel);
        cl->addWidget(nameLabel);
        cl->addWidget(descLabel);

        grid->addWidget(card, i / 3, i % 3);
    }

    layout->addLayout(grid);
    layout->addStretch();
}
