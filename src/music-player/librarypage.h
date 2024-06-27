#pragma once

#include "basepage.h"

namespace Ui {
class LibraryPage;
}

class LibraryPage : public BasePage {
    Q_OBJECT

public:
    explicit LibraryPage(QWidget *parent = nullptr);
    ~LibraryPage();

    bool init() override;

private:
    Ui::LibraryPage *ui;
};
