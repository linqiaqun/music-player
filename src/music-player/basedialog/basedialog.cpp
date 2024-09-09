#include "basedialog.h"
#include <QMouseEvent>

BaseDialog::BaseDialog(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::CustomizeWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

void BaseDialog::mousePressEvent(QMouseEvent* ev) {
    if ((Qt::LeftButton == ev->button()) && (!isMaximized())) {
        m_pressed = true;
        m_pos = ev->pos();
    }

    QDialog::mousePressEvent(ev);
}

void BaseDialog::mouseMoveEvent(QMouseEvent* ev) {
    if (m_pressed) {
        move(ev->pos() - m_pos + pos());
    }

    QDialog::mouseMoveEvent(ev);
}

void BaseDialog::mouseReleaseEvent(QMouseEvent* ev) {
    m_pressed = false;
    QDialog::mouseReleaseEvent(ev);
}
