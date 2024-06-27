#include "favoritepage.h"

#include "ui_favoritepage.h"

FavoritePage::FavoritePage(QWidget *parent) : BasePage(parent), ui(new Ui::FavoritePage) {
    ui->setupUi(this);
}

FavoritePage::~FavoritePage() { delete ui; }

bool FavoritePage::init() { return true; }
