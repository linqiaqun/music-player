#include "logger.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QLoggingCategory>
#include <QMutex>
#include <QMutexLocker>
#include <QTextStream>

void logHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    // only output critical log under release mode
    if (QtCriticalMsg != type) return;

    // create log directory
    QString dirPath = QCoreApplication::applicationDirPath() + "/logs/";
    QDir dir(dirPath);
    if ((!dir.exists(dirPath)) && (!dir.mkpath(dirPath))) {
        return;
    }

    QString finalMsg = QString("[%1 %2 %3 %4] %6")
                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))
                           .arg(type)
                           .arg(context.file)
                           .arg(context.line)
                           .arg(msg);

    // output file
    QFile file(dirPath + QDateTime::currentDateTime().toString("yyMMddhh") + ".log");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream stream(&file);
    stream << finalMsg << "\r\n";
    file.flush();
    file.close();
}

void Logger::enable() {
#ifdef QT_NO_DEBUG
    qInstallMessageHandler(logHandler);
#else
    // enable critical category only
    QLoggingCategory::setFilterRules("*=true");

    // define log format
    qSetMessagePattern("[%{time yyyy-MM-dd hh:mm:ss:zzz} %{type} %{file} %{line}] %{message}");
#endif  // QT_NO_DEBUG
}
