#pragma once

#include <QDialog>

class BaseDialog : public QDialog {
    Q_OBJECT
public:
    BaseDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;

private:
    QPoint m_pos;
    bool m_pressed = false;
};
