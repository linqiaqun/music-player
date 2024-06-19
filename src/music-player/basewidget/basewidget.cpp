#include "basewidget.h"

#include <QMouseEvent>

BaseWidget::BaseWidget(QWidget* parent) : QWidget{parent} {
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

void BaseWidget::mousePressEvent(QMouseEvent* ev) {
    if ((Qt::LeftButton == ev->button()) && (!isMaximized())) {
        m_pressed = true;
        m_pos = ev->pos();
    }

    QWidget::mousePressEvent(ev);
}

void BaseWidget::mouseMoveEvent(QMouseEvent* ev) {
    if (m_pressed) {
        move(ev->pos() - m_pos + pos());
    }

    QWidget::mouseMoveEvent(ev);
}

void BaseWidget::mouseReleaseEvent(QMouseEvent* ev) {
    m_pressed = false;
    QWidget::mouseReleaseEvent(ev);
}
