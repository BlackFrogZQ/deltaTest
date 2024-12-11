#pragma once
#include "basic.h"

class ISystemService
{
    friend void closeSystemService();

public:
    virtual void printMsg(QString p_msg) = 0;
    virtual void updateUiImage(const QImage &p_image) = 0;
    virtual void setOnlyImage(bool p_isOnlyImage = true) = 0;
    virtual void restartSys() const = 0;

    virtual void load() = 0;
    virtual void save() = 0;

protected:
    virtual ~ISystemService(){};
};
ISystemService *sys();