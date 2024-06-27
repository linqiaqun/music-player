#include "homepage.h"

#include "ui_homepage.h"

HomePage::HomePage(QWidget *parent) : BasePage(parent), ui(new Ui::HomePage) { ui->setupUi(this); }

HomePage::~HomePage() { delete ui; }

bool HomePage::init() { return true; }
