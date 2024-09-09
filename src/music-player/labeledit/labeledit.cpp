#include "labeledit.h"

#include <QFocusEvent>

LabelEdit::LabelEdit(QWidget *parent) : QLineEdit(parent) {
    connect(this, &LabelEdit::editingFinished, this, [=]() { setReadOnly(true); });
}

LabelEdit::LabelEdit(const QString &contents, QWidget *parent) : QLineEdit(contents, parent) {}

void LabelEdit::mouseDoubleClickEvent(QMouseEvent *ev) {
    if (isReadOnly()) {
        setReadOnly(false);
        setFocus();
    }
    QLineEdit::mousePressEvent(ev);
}

void LabelEdit::focusOutEvent(QFocusEvent *ev) {
    if ((!isReadOnly()) && ev->lostFocus()) emit editingFinished();
}
