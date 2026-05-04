#include "recordings_page.h"
#include "../history_manager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QFrame>

RecordingsPage::RecordingsPage(QWidget* parent) : QWidget(parent) {
    HistoryManager::instance().load();
    setupUI();
    loadRecordings();
}

void RecordingsPage::setupUI() {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(16);

    // Header
    auto* headerRow = new QHBoxLayout();
    auto* titleLabel = new QLabel("📹  Recordings");
    QFont tf("Segoe UI", 18, QFont::Bold);
    titleLabel->setFont(tf);
    titleLabel->setStyleSheet("color: #F1F5F9;");

    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText("🔍  Search recordings...");
    m_searchEdit->setFixedWidth(260);
    m_searchEdit->setFixedHeight(38);

    m_filterCombo = new QComboBox();
    m_filterCombo->addItems({"All Formats", "MP4", "MKV", "AVI"});
    m_filterCombo->setFixedWidth(140);
    m_filterCombo->setFixedHeight(38);

    auto* deleteBtn = new QPushButton("🗑  Delete Selected");
    deleteBtn->setFixedHeight(38);
    deleteBtn->setStyleSheet("QPushButton { background: #2D1010; color: #EF4444; border: 1px solid #7F1D1D; border-radius: 8px; padding: 0 14px; }"
                             "QPushButton:hover { background: #7F1D1D; color: white; }");
    connect(deleteBtn, &QPushButton::clicked, this, &RecordingsPage::deleteSelected);

    headerRow->addWidget(titleLabel);
    headerRow->addStretch();
    headerRow->addWidget(m_searchEdit);
    headerRow->addWidget(m_filterCombo);
    headerRow->addWidget(deleteBtn);
    layout->addLayout(headerRow);

    // Table
    m_table = new QTableWidget(0, 6, this);
    m_table->setHorizontalHeaderLabels({"File Name", "Duration", "Resolution", "FPS", "Size", "Date"});
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setAlternatingRowColors(true);
    m_table->setShowGrid(false);
    m_table->verticalHeader()->hide();
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setContextMenuPolicy(Qt::CustomContextMenu);
    m_table->setStyleSheet("QTableWidget { alternate-background-color: #0F0F1F; }");

    connect(m_table, &QTableWidget::customContextMenuRequested, [this](const QPoint& pos) {
        QMenu menu(this);
        auto* openAct = menu.addAction("📁  Open File");
        auto* editAct = menu.addAction("✂  Open in Editor");
        menu.addSeparator();
        auto* deleteAct = menu.addAction("🗑  Delete");
        auto* action = menu.exec(m_table->viewport()->mapToGlobal(pos));
        if (action == openAct) openSelected();
        else if (action == editAct) openInEditor();
        else if (action == deleteAct) deleteSelected();
    });

    connect(m_table, &QTableWidget::doubleClicked, [this](const QModelIndex&) { openSelected(); });

    m_emptyLabel = new QLabel("No recordings yet.\nStart recording to see your videos here.");
    m_emptyLabel->setAlignment(Qt::AlignCenter);
    m_emptyLabel->setStyleSheet("color: #475569; font-size: 14px;");
    m_emptyLabel->hide();

    layout->addWidget(m_table);
    layout->addWidget(m_emptyLabel);
}

void RecordingsPage::loadRecordings() {
    m_table->setRowCount(0);
    auto history = HistoryManager::instance().getHistory();

    // Add demo entries if empty
    if (history.isEmpty()) {
        struct Demo { QString name; QString dur; QString res; int fps; QString size; QString date; };
        QList<Demo> demos = {
            {"Gameplay_2024-05-20.mp4", "00:12:45", "1080p", 60, "1.2 GB", "20/05/2024"},
            {"Tutorial_video.mp4", "00:08:32", "1080p", 30, "722 MB", "18/05/2024"},
            {"Meeting_recording.mkv", "00:24:19", "2K", 60, "1.8 GB", "18/05/2024"},
            {"Live_stream.mp4", "01:35:42", "1080p", 60, "3.5 GB", "17/05/2024"},
        };
        m_table->setRowCount(demos.size());
        for (int i = 0; i < demos.size(); ++i) {
            m_table->setItem(i, 0, new QTableWidgetItem(demos[i].name));
            m_table->setItem(i, 1, new QTableWidgetItem(demos[i].dur));
            m_table->setItem(i, 2, new QTableWidgetItem(demos[i].res));
            m_table->setItem(i, 3, new QTableWidgetItem(QString::number(demos[i].fps)));
            m_table->setItem(i, 4, new QTableWidgetItem(demos[i].size));
            m_table->setItem(i, 5, new QTableWidgetItem(demos[i].date));
            m_table->setRowHeight(i, 44);
        }
        return;
    }

    m_table->setRowCount(history.size());
    for (int i = 0; i < history.size(); ++i) {
        auto& e = history[i];
        qint64 dur = e.duration;
        QString durStr = QString("%1:%2:%3")
            .arg(dur/3600,2,10,QChar('0'))
            .arg((dur%3600)/60,2,10,QChar('0'))
            .arg(dur%60,2,10,QChar('0'));

        double sizeGB = e.fileSize / 1073741824.0;
        QString sizeStr = sizeGB < 1.0 ?
            QString("%1 MB").arg(e.fileSize/1048576.0,0,'f',0) :
            QString("%1 GB").arg(sizeGB,0,'f',1);

        m_table->setItem(i, 0, new QTableWidgetItem(e.fileName));
        m_table->setItem(i, 1, new QTableWidgetItem(durStr));
        m_table->setItem(i, 2, new QTableWidgetItem(e.resolution));
        m_table->setItem(i, 3, new QTableWidgetItem(QString::number(e.fps)));
        m_table->setItem(i, 4, new QTableWidgetItem(sizeStr));
        m_table->setItem(i, 5, new QTableWidgetItem(e.startTime.toString("dd/MM/yyyy")));
        m_table->setRowHeight(i, 44);
    }
    m_emptyLabel->setVisible(history.isEmpty());
    m_table->setVisible(!history.isEmpty());
}

void RecordingsPage::refresh() { loadRecordings(); }

void RecordingsPage::openSelected() {
    int row = m_table->currentRow();
    if (row < 0) return;
    auto history = HistoryManager::instance().getHistory();
    if (row < history.size()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(history[row].filePath));
    }
}

void RecordingsPage::deleteSelected() {
    int row = m_table->currentRow();
    if (row < 0) return;
    auto reply = QMessageBox::question(this, "Delete Recording",
        "Are you sure you want to delete this recording?",
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        HistoryManager::instance().removeEntry(row);
        loadRecordings();
    }
}

void RecordingsPage::openInEditor() {
    // TODO: Open editor with selected file
}
