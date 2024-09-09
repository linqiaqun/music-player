#pragma once

#include <QWidget>

namespace Ui {
class VolumeWidget;
}

class VolumeWidget : public QWidget {
    Q_OBJECT

public:
    explicit VolumeWidget(QWidget *parent = nullptr);
    ~VolumeWidget();

private:
    Ui::VolumeWidget *ui;
};
