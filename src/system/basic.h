#pragma once
#include <QDebug>

#define iToStr(num) QString::number(num)
#define cnStr(str) QString::fromLocal8Bit(str)
#define cnHStr(str) QString::fromLocal8Bit(str).toStdString().data()
#define qToHStr(str) (str).toStdString().data()
#define hToQStr(str) (str).S().Text()
#define cToHStr(str) (str).toStdString().c_str()

#define myDebug qDebug().noquote()
#define myInfo qInfo().noquote()

#ifdef Trace
#define traceT(qDebugMsg) qDebug() << "trace:" << qDebugMsg
#else
#define traceT(qDebugMsg)
#endif

#define delPtr(ptr)     \
    if (ptr != nullptr) \
    {                   \
        delete ptr;     \
        ptr = nullptr;  \
    }

#define delPyPtr(ptr)     \
    if (ptr != nullptr) \
    {                   \
        Py_DECREF(ptr);     \
    }