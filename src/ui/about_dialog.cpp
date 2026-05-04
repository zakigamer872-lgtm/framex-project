#include "about_dialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>

AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("About FrameX");
    setFixedSize(400, 460);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(12);

    // Background frame
    auto* frame = new QFrame(this);
    frame->setStyleSheet("QFrame { background: #0F0F1F; border: 1px solid #2D2B55; border-radius: 16px; }");
    auto* fl = new QVBoxLayout(frame);
    fl->setContentsMargins(24, 24, 24, 24);
    fl->setSpacing(14);
    layout->addWidget(frame);

    // Logo
    auto* logoLabel = new QLabel("🎬 FrameX");
    QFont lf("Segoe UI", 24, QFont::Bold);
    logoLabel->setFont(lf);
    logoLabel->setStyleSheet("color: #C084FC;");
    logoLabel->setAlignment(Qt::AlignCenter);
    fl->addWidget(logoLabel);

    auto* versionLabel = new QLabel("Version 1.0.0");
    versionLabel->setAlignment(Qt::AlignCenter);
    versionLabel->setStyleSheet("color: #64748B;");
    fl->addWidget(versionLabel);

    auto* tagLabel = new QLabel("Capture everything. Share anything.");
    tagLabel->setAlignment(Qt::AlignCenter);
    tagLabel->setStyleSheet("color: #94A3B8; font-style: italic;");
    fl->addWidget(tagLabel);

    auto* divider = new QFrame(); divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet("color: #1E1B3A;");
    fl->addWidget(divider);

    // Developer info
    struct InfoRow { QString key; QString val; };
    QList<InfoRow> rows = {
        {"Developer", "ZAKI"},
        {"Instagram", "@zg22x"},
        {"TikTok", "@zg22x"},
        {"Framework", "Qt 6 / C++17"},
        {"Video Engine", "FFmpeg"},
        {"License", "Free to use"},
    };

    for (auto& r : rows) {
        auto* row = new QHBoxLayout();
        auto* kl = new QLabel(r.key + ":");
        kl->setStyleSheet("color: #64748B; font-size: 12px;");
        kl->setFixedWidth(100);
        auto* vl = new QLabel(r.val);
        vl->setStyleSheet("color: #E2E8F0; font-size: 12px; font-weight: bold;");
        row->addWidget(kl);
        row->addWidget(vl);
        row->addStretch();
        fl->addLayout(row);
    }

    fl->addStretch();

    // Social buttons
    auto* socialRow = new QHBoxLayout();
    auto* igBtn = new QPushButton("📸 Instagram");
    igBtn->setStyleSheet("QPushButton { background: #1A1A2E; border: 1px solid #2D2B55; border-radius: 8px; color: #CBD5E1; padding: 8px 16px; }"
                         "QPushButton:hover { background: #7C3AED; border-color: #9333EA; color: white; }");
    connect(igBtn, &QPushButton::clicked, []() {
        QDesktopServices::openUrl(QUrl("https://instagram.com/zg22x"));
    });

    auto* tkBtn = new QPushButton("🎵 TikTok");
    tkBtn->setStyleSheet("QPushButton { background: #1A1A2E; border: 1px solid #2D2B55; border-radius: 8px; color: #CBD5E1; padding: 8px 16px; }"
                         "QPushButton:hover { background: #7C3AED; border-color: #9333EA; color: white; }");
    connect(tkBtn, &QPushButton::clicked, []() {
        QDesktopServices::openUrl(QUrl("https://tiktok.com/@zg22x"));
    });

    socialRow->addWidget(igBtn);
    socialRow->addWidget(tkBtn);
    fl->addLayout(socialRow);

    auto* closeBtn = new QPushButton("Close");
    closeBtn->setObjectName("primaryBtn");
    closeBtn->setFixedHeight(38);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    fl->addWidget(closeBtn);
}
