#include <QApplication>
#include <QMessageBox>

#include "./configuration/configuration.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    if (!CONFIG->readFile(a.applicationDirPath() + "/app.json", true)) {
        QMessageBox::critical(nullptr, QObject::tr("error"), QObject::tr("load configuration file failed"));
        return 1;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
