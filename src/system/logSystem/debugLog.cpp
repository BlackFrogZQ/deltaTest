#include "system/systemService.h"
#include <QString>
#include <QDateTime>
#include <iostream>

void logOutput(QtMsgType p_type, const QMessageLogContext &p_context, const QString &p_msg)
{
    QString msgType("");
    QString msg = p_msg;
    msg.replace('\"', "");
    QString wirteMsg = QString::fromLocal8Bit("[%1]:%2").arg(QDateTime::currentDateTime().toString("hh.mm.ss")).arg(msg);
    switch (p_type)
    {
    case QtInfoMsg:
        msgType = QString("Info");
        sys()->printMsg(msg);
        break;
    case QtDebugMsg:
        msgType = QString("Debug");
        break;
    case QtWarningMsg:
        msgType = QString("Warning");
        break;
    case QtCriticalMsg:
        msgType = QString("Critical");
        break;
    case QtFatalMsg:
        msgType = QString("Fatal");
        break;
    default:
        msgType = QString("Unknown Type");
        break;
    }
    wirteMsg = QString::fromLocal8Bit("%1消息--%2,\nfile:%3,line:%4").arg(msgType, 12, ' ').arg(wirteMsg).arg(p_context.file).arg(p_context.line);
    // 设置输出信息格式
    std::cout << wirteMsg.toLocal8Bit().data() << std::endl;
};