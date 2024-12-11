#pragma once
#include <QString>
#include <QColor>
class CLogDef
{
public:
    static QString getInfoLogName();
    static QString getDateLogName();
    static void writeLog(QString p_logName, QString p_msg);
};