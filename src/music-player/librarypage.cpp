#include "librarypage.h"

#include "ui_librarypage.h"

LibraryPage::LibraryPage(QWidget *parent) : BasePage(parent), ui(new Ui::LibraryPage) {
    ui->setupUi(this);
}

LibraryPage::~LibraryPage() { delete ui; }

bool LibraryPage::init() { return true; }
