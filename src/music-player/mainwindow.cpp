#include "mainwindow.h"

#include <QGraphicsDropShadowEffect>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : BaseWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->minBtn, &QPushButton::clicked, this, &MainWindow::showMinimized);
    connect(ui->maxBtn, &QPushButton::clicked, this, &MainWindow::showMaximized);
    connect(ui->restoreBtn, &QPushButton::clicked, this, &MainWindow::showNormal);
    connect(ui->closeBtn, &QPushButton::clicked, this, &MainWindow::close);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(Qt::gray);
    shadow->setBlurRadius(32);
    ui->container->setGraphicsEffect(shadow);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::resizeEvent(QResizeEvent *ev) {
    Q_UNUSED(ev)
    ui->maxBtn->setHidden(isMaximized());
    ui->restoreBtn->setHidden(!isMaximized());
}
