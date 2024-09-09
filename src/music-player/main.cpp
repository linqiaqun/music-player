#include <QApplication>
#include <QMessageBox>

#include "./configuration/configuration.h"
#include "./database/database.h"
#include "./util/util.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // load global configuration file
    if (!CONFIG->readFile(app.applicationDirPath() + "/app.json", true)) {
        QMessageBox::critical(nullptr, QObject::tr("error"),
                              QObject::tr("load configuration file failed"));
        return 1;
    }

    // set qss
    if (!Util::setQss(CONFIG->value("theme_list").toList(),
                      CONFIG->value("current_theme").toInt())) {
        QMessageBox::warning(nullptr, QObject::tr("warning"), QObject::tr("set qss failed"));
    }

    // initialize database
    if (!DB->init(app.applicationDirPath() + "/main.db")) {
        QMessageBox::critical(nullptr, QObject::tr("error"),
                              QObject::tr("initialize database failed"));
        return 2;
    }

    MainWindow w;
    w.show();
    return app.exec();
}
