#pragma once

#include <QWidget>

class BaseWidget : public QWidget {
    Q_OBJECT
public:
    explicit BaseWidget(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;

private:
    QPoint m_pos;
    bool m_pressed = false;
};
