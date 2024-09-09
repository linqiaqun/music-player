#include "clickablelabel.h"

#include <QMouseEvent>

ClickableLabel::ClickableLabel(QWidget *parent, Qt::WindowFlags f) : QLabel(parent, f) {}

ClickableLabel::ClickableLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
    : QLabel(text, parent, f) {}

void ClickableLabel::mousePressEvent(QMouseEvent *ev) {
    emit clicked();
    QLabel::mousePressEvent(ev);
}
