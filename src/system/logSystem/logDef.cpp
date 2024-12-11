#include "logDef.h"
#include "system/tool/fileTool.h"
#include <QDebug>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QMutex>

QString CLogDef::getInfoLogName()
{
    TIGER_FlieTool::createDir(QDir::currentPath() + "/log");
    return QString("./log/%1_infoLog.txt").arg(QDateTime::currentDateTime().toString("yyyy.MM.dd"));
}

QString CLogDef::getDateLogName()
{
    TIGER_FlieTool::createDir(QDir::currentPath() + "/log");
    return QString("./log/%1_dataLog.txt").arg(QDateTime::currentDateTime().toString("yyyy.MM.dd"));
}

void CLogDef::writeLog(QString p_logName, QString p_msg)
{
    static QMutex mutex;
    mutex.lock();
    QFile file(p_logName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        file.write(p_msg.toStdString().data());
        file.write("\n");
        file.flush();
        file.close();
    }
    mutex.unlock();
}