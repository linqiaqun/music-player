#pragma once

#include <QLabel>

class ClickableLabel : public QLabel {
    Q_OBJECT
public:
    ClickableLabel(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ClickableLabel(const QString &text, QWidget *parent = nullptr,
                   Qt::WindowFlags f = Qt::WindowFlags());

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *ev);
};
