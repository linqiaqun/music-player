#pragma once

#include <QProgressBar>

class ClickableProgressBar : public QProgressBar {
    Q_OBJECT
public:
    ClickableProgressBar(QWidget *parent = nullptr);

signals:
    void valueClicked(qint64);

protected:
    void mousePressEvent(QMouseEvent *ev);
};
