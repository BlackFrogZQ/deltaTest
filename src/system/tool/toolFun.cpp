#include "toolFun.h"
namespace TIGER_ToolFun
{
    void scaled(const QPointF &p_formSizeWH, QPointF &p_toSizeWH, Qt::AspectRatioMode p_aspectRatioMode)
    {
        assert(p_formSizeWH.y() != 0 && p_toSizeWH.y() != 0);
        double formAspectRatio = p_formSizeWH.x() / p_formSizeWH.y();
        double toAspectRatio = p_toSizeWH.x() / p_toSizeWH.y();

        switch (p_aspectRatioMode)
        {
        case Qt::IgnoreAspectRatio:
            break;
        case Qt::KeepAspectRatio:
            formAspectRatio > toAspectRatio ? p_toSizeWH.setY(p_toSizeWH.x() / formAspectRatio) : p_toSizeWH.setX(p_toSizeWH.y() * formAspectRatio);
            break;
        case Qt::KeepAspectRatioByExpanding:
            formAspectRatio < toAspectRatio ? p_toSizeWH.setY(p_toSizeWH.x() / formAspectRatio) : p_toSizeWH.setX(p_toSizeWH.y() * formAspectRatio);
            break;
        default:
            break;
        }
    }
};