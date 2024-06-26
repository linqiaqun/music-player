#include <QApplication>
#include <QMessageBox>

#include "./configuration/configuration.h"
#include "./database/database.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // load global configuration file
    if (!CONFIG->readFile(a.applicationDirPath() + "/app.json", true)) {
        QMessageBox::critical(nullptr, QObject::tr("error"),
                              QObject::tr("load configuration file failed"));
        return 1;
    }

    // initialize database
    if (!DB->init(a.applicationDirPath() + "/main.db")) {
        QMessageBox::critical(nullptr, QObject::tr("error"),
                              QObject::tr("initialize database failed"));
        return 2;
    }
    qDebug() << DB->getPlaylistList();

    MainWindow w;
    w.show();
    return a.exec();
}
