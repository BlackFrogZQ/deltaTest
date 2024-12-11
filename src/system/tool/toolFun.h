#pragma once
#include "system/basic.h"
#include <QPointF>
namespace TIGER_ToolFun
{
    void scaled(const QPointF &p_formSizeWH, QPointF &p_toSizeWH, Qt::AspectRatioMode p_aspectRatioMode = Qt::KeepAspectRatio);
};