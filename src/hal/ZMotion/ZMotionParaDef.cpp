#include "ZMotionParaDef.h"

CZMotionParas *ZMotionParas()
{
    static CZMotionParas g_ZMotionParas;
    return &g_ZMotionParas;
}