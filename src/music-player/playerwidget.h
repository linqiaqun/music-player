#pragma once

#include <QWidget>

namespace Ui {
class PlayerWidget;
}

class PlayerWidget : public QWidget {
    Q_OBJECT

public:
    explicit PlayerWidget(QWidget *parent = nullptr);
    ~PlayerWidget();

    void init();

private slots:
    void modeBtnClicked();
    void previousBtnClicked();
    void playBtnClicked();
    void nextBtnClicked();
    void volumeBtnClicked();
    void playlistBtnClicked();

    void modeChanged(int);
    void positionChanged(qint64);
    void positionClicked(qint64);

private:
    void setSong(const QVariantMap &map);

private:
    Ui::PlayerWidget *ui;
    bool m_init = false;
};
