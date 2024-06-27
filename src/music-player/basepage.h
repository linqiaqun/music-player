#pragma once

#include <QWidget>

class BasePage : public QWidget {
    Q_OBJECT

public:
    BasePage(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags()) : QWidget(parent, f) {}
    virtual ~BasePage() {}

    virtual bool init() = 0;
};
