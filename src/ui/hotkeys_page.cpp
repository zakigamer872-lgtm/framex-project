#include "hotkeys_page.h"
#include "../settings_manager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QFrame>
#include <QLabel>

HotkeysPage::HotkeysPage(QWidget* parent) : QWidget(parent) {
    setupUI();
    loadHotkeys();
}

void HotkeysPage::setupUI() {
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

    auto* titleLabel = new QLabel("⌨  Hotkeys");
    QFont tf("Segoe UI", 18, QFont::Bold);
    titleLabel->setFont(tf);
    titleLabel->setStyleSheet("color: #F1F5F9;");
    layout->addWidget(titleLabel);

    auto* infoLabel = new QLabel("Click on a hotkey field and press your desired key combination to change it.");
    infoLabel->setStyleSheet("color: #64748B; font-size: 12px;");
    infoLabel->setWordWrap(true);
    layout->addWidget(infoLabel);

    auto addHotkeyRow = [&](const QString& label, QKeySequenceEdit*& edit) {
        auto* row = new QHBoxLayout();
        auto* lbl = new QLabel(label);
        lbl->setStyleSheet("color: #CBD5E1; font-size: 13px;");
        lbl->setFixedWidth(200);
        edit = new QKeySequenceEdit();
        edit->setFixedHeight(38);
        edit->setStyleSheet("QKeySequenceEdit { background: #12122A; border: 1px solid #2D2B55; border-radius: 8px; color: #C084FC; padding: 0 10px; font-size: 13px; }"
                            "QKeySequenceEdit:focus { border-color: #7C3AED; }");
        auto* clearBtn = new QPushButton("Clear");
        clearBtn->setFixedSize(60, 38);
        clearBtn->setStyleSheet("QPushButton { background: #1A1A2E; border: 1px solid #2D2B55; border-radius: 6px; color: #94A3B8; }"
                                "QPushButton:hover { background: #2D2B55; }");
        connect(clearBtn, &QPushButton::clicked, edit, &QKeySequenceEdit::clear);
        row->addWidget(lbl);
        row->addWidget(edit, 1);
        row->addWidget(clearBtn);
        layout->addLayout(row);
    };

    // Hotkey group
    auto* grp = new QGroupBox("Recording Hotkeys", content);
    grp->setStyleSheet("QGroupBox { border: 1px solid #1E1B3A; border-radius: 12px; margin-top: 12px; padding: 16px; color: #7C3AED; font-weight: bold; }"
                       "QGroupBox::title { subcontrol-origin: margin; left: 12px; padding: 0 6px; }");
    auto* grpLayout = new QVBoxLayout(grp);
    grpLayout->setSpacing(12);

    auto addGrpRow = [&](const QString& lbl, QKeySequenceEdit*& edit) {
        auto* row = new QHBoxLayout();
        auto* lb = new QLabel(lbl);
        lb->setStyleSheet("color: #CBD5E1;");
        lb->setFixedWidth(220);
        edit = new QKeySequenceEdit();
        edit->setFixedHeight(38);
        edit->setStyleSheet("QKeySequenceEdit { background: #12122A; border: 1px solid #2D2B55; border-radius: 8px; color: #C084FC; padding: 0 10px; }"
                            "QKeySequenceEdit:focus { border-color: #7C3AED; }");
        auto* clr = new QPushButton("✕");
        clr->setFixedSize(36, 36);
        clr->setStyleSheet("QPushButton { background: #1A1A2E; border: 1px solid #2D2B55; border-radius: 6px; color: #64748B; }"
                           "QPushButton:hover { color: #EF4444; }");
        connect(clr, &QPushButton::clicked, edit, &QKeySequenceEdit::clear);
        row->addWidget(lb);
        row->addWidget(edit, 1);
        row->addWidget(clr);
        grpLayout->addLayout(row);
    };

    addGrpRow("Start / Stop Recording", m_startStopEdit);
    addGrpRow("Pause / Resume", m_pauseEdit);
    addGrpRow("Take Screenshot", m_screenshotEdit);
    addGrpRow("Cancel Recording", m_cancelEdit);
    addGrpRow("Open Editor", m_editorEdit);

    layout->addWidget(grp);

    // Save button
    auto* btnRow = new QHBoxLayout();
    auto* saveBtn = new QPushButton("💾  Save Hotkeys");
    saveBtn->setObjectName("primaryBtn");
    saveBtn->setFixedHeight(42);
    connect(saveBtn, &QPushButton::clicked, this, &HotkeysPage::saveHotkeys);
    btnRow->addWidget(saveBtn);
    btnRow->addStretch();
    layout->addLayout(btnRow);
    layout->addStretch();
}

void HotkeysPage::loadHotkeys() {
    auto& hk = SettingsManager::instance().hotkeys;
    m_startStopEdit->setKeySequence(hk.startStop);
    m_pauseEdit->setKeySequence(hk.pauseResume);
    m_screenshotEdit->setKeySequence(hk.screenshot);
    m_cancelEdit->setKeySequence(hk.cancelRecording);
    m_editorEdit->setKeySequence(hk.openEditor);
}

void HotkeysPage::saveHotkeys() {
    auto& hk = SettingsManager::instance().hotkeys;
    hk.startStop = m_startStopEdit->keySequence();
    hk.pauseResume = m_pauseEdit->keySequence();
    hk.screenshot = m_screenshotEdit->keySequence();
    hk.cancelRecording = m_cancelEdit->keySequence();
    hk.openEditor = m_editorEdit->keySequence();
    SettingsManager::instance().save();
}
