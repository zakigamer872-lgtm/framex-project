#pragma once
#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>

class ScreenshotsPage : public QWidget {
    Q_OBJECT
public:
    explicit ScreenshotsPage(QWidget* parent = nullptr);
    void refresh();

private:
    void setupUI();
    void loadScreenshots();

    QGridLayout* m_gridLayout;
    QWidget* m_gridWidget;
    QLabel* m_emptyLabel;
    QLabel* m_countLabel;
};
