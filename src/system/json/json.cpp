#include "json.h"

bool CField::getValue(const QJsonObject &p_json)
{
    QJsonValue jsonValue = p_json.value(this->key);
    if (jsonValue.isUndefined())
    {
        return false;
    }
    this->value = jsonValue.toString();
    return true;
}

bool CField::getValue(const QString &p_key, QJsonValue &p_value, const QJsonObject &p_json)
{
    QJsonValue jsonValue = p_json.value(p_key);
    if (jsonValue.isUndefined())
    {
        return false;
    }
    p_value = jsonValue;
    return true;
}