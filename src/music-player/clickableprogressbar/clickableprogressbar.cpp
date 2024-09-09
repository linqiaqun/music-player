#include "clickableprogressbar.h"

#include <QMouseEvent>

ClickableProgressBar::ClickableProgressBar(QWidget *parent) : QProgressBar(parent) {
    setMouseTracking(true);
    setCursor(QCursor(Qt::PointingHandCursor));
}

void ClickableProgressBar::mousePressEvent(QMouseEvent *ev) {
    qint64 value = ev->pos().x() * maximum() / width();
    if (3 >= (width() - ev->pos().x())) value = maximum();
    setValue(value);
    emit valueClicked(value);
    ev->accept();
}
