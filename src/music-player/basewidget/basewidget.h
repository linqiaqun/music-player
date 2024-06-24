/*!
*    file: basewidget.h
*    author: linqiaqun@163.com
*    date: 2024-06-20 11:44
*    license: LGPL-3.0-only
*/

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
