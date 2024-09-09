#pragma once

#include <QLineEdit>
#include <QObject>

class LabelEdit : public QLineEdit {
    Q_OBJECT

public:
    LabelEdit(QWidget *parent = nullptr);
    LabelEdit(const QString &contents, QWidget *parent = nullptr);

protected:
    void mouseDoubleClickEvent(QMouseEvent *ev) override;
    void focusOutEvent(QFocusEvent *ev);
};
