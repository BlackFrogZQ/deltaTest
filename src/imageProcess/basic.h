#pragma once
#include "system/basic.h"
#include "system/myMath.h"
#include <QObject>
class QImage;

namespace TIGER_ProcessTool
{
    class IProcessToolFun: public QObject
    {
    public:
        virtual ~IProcessToolFun(){};
        int getCoordinateX() const { return m_coordinateX; };
        int getCoordinateY() const { return m_coordinateY; };
        QString getErrorMsg() const { return m_error; };

    protected:
        int m_coordinateX;
        int m_coordinateY;
        QString m_error;
    };
};