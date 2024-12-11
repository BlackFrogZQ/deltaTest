#pragma once
#include "system/basic.h"
#include <QJsonObject>
#include <QJsonValue>
struct CField
{
    QString key;
    QString value;
    bool getValue(const QJsonObject &p_json);
    static bool getValue(const QString &p_key, QJsonValue &p_value,const QJsonObject &p_json);
};
