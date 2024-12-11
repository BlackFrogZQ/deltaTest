#include "para/define/paraDef.h"
#include "para/define/paraNode.h"
#include "ZMotionDef.h"
#include "hal/ZMotion/ZMotionParaDef.h"

using namespace TIGER_ParaDef;

namespace TIGER_ZMotionDef
{
    class CZMotionParas : public CParas
    {
    public:
        CZMotionParas() : CParas(cnStr("ZMotionPara"), cnStr("运动控制卡参数"), true)
        {
            CParaNode *node;
            m_currentNode->appendNode({pntString, "ip", cnStr("ip"), true}, &(ZMotionParas()->ip));
            for (int axisType = 0; axisType < atAxisMax; axisType++)
            {
                for (int axisTypeId = 0; axisTypeId < aiAxisMax; axisTypeId++)
                {
                    _AddRootNode(m_currentNode, CAxisTypeEn[axisType].toLocal8Bit() + CAxisIDStr[axisTypeId].toLocal8Bit(), CAxisTypeStr[axisType].toLocal8Bit() + CAxisIDStr[axisTypeId].toLocal8Bit(), true, "");
                    node->appendNode({pntInt, "setAtype", cnStr("轴类型"), true, cnStr("设置轴类型")}, &(ZMotionParas()->axisPara[axisType][axisTypeId]).setAtype);
                    node->appendNode({pntInt, "setDpos", cnStr("DPOS值"), true, cnStr("设置DPOS值")}, &(ZMotionParas()->axisPara[axisType][axisTypeId]).setDpos);
                    node->appendNode({pntInt, "setDatumIn", cnStr("原点信号"), true, cnStr("设置原点信号")}, &(ZMotionParas()->axisPara[axisType][axisTypeId]).setDatumIn);
                    node->appendNode({pntInt, "setUnits", cnStr("轴脉冲当量"), true, cnStr("设置轴脉冲当量")}, &(ZMotionParas()->axisPara[axisType][axisTypeId]).setUnits);
                    node->appendNode({pntInt, "setSpeed", cnStr("轴最大速度"), true, cnStr("设置轴最大速度")}, &(ZMotionParas()->axisPara[axisType][axisTypeId]).setSpeed);
                    node->appendNode({pntInt, "setAccel", cnStr("轴的加速度"), true, cnStr("设置轴的加速度")}, &(ZMotionParas()->axisPara[axisType][axisTypeId]).setAccel);
                    node->appendNode({pntInt, "setDecel", cnStr("轴的减速度"), true, cnStr("设置轴的减速度")}, &(ZMotionParas()->axisPara[axisType][axisTypeId]).setDecel);
                    node->appendNode({pntInt, "setSramp", cnStr("轴的s曲线"), true, cnStr("设置轴的s曲线")}, &(ZMotionParas()->axisPara[axisType][axisTypeId]).setSramp);
                    node->appendNode({pntInt, "setCreep", cnStr("关节轴回爬速度"), true, cnStr("关节轴回爬速度")}, &(ZMotionParas()->axisPara[axisType][axisTypeId]).setCreep);
                }
            }
            _AddRootNode(m_currentNode, "zeroPoint", "原点", true, "");
            node->appendNode({pntDouble, "zeroX", cnStr("X"), true, cnStr("设置原点X坐标")}, &(ZMotionParas()->zeroX));
            node->appendNode({pntDouble, "zeroY", cnStr("Y"), true, cnStr("设置原点Y坐标")}, &(ZMotionParas()->zeroY));
            node->appendNode({pntDouble, "zeroZ", cnStr("Z"), true, cnStr("设置原点Z坐标")}, &(ZMotionParas()->zeroZ));
            _AddRootNode(m_currentNode, "workPoint", "工作点", true, "");
            node->appendNode({pntDouble, "workX", cnStr("X"), true, cnStr("设置工作点点X坐标")}, &(ZMotionParas()->workX));
            node->appendNode({pntDouble, "workY", cnStr("Y"), true, cnStr("设置工作点点Y坐标")}, &(ZMotionParas()->workY));
            node->appendNode({pntDouble, "workZ", cnStr("Z"), true, cnStr("设置工作点点Z坐标")}, &(ZMotionParas()->workZ));
            _AddRootNode(m_currentNode, "placePoint", "放置点", true, "");
            node->appendNode({pntDouble, "placeX", cnStr("X"), true, cnStr("设置位置点X坐标")}, &(ZMotionParas()->placeX));
            node->appendNode({pntDouble, "placeY", cnStr("Y"), true, cnStr("设置位置点Y坐标")}, &(ZMotionParas()->placeY));
            node->appendNode({pntDouble, "placeZ", cnStr("Z"), true, cnStr("设置位置点Z坐标")}, &(ZMotionParas()->placeZ));
            _AddRootNode(m_currentNode, "grabPoint", "抓取点", true, "");
            node->appendNode({pntDouble, "grabZUp", cnStr("ZUp"), true, cnStr("设置抓取点Z上坐标")}, &(ZMotionParas()->grabZUp));
            node->appendNode({pntDouble, "grabZDown", cnStr("ZDown"), true, cnStr("设置抓取点Z下坐标")}, &(ZMotionParas()->grabZDown));
            node->appendNode({pntUInt, "Speed", cnStr("Speed"), true, cnStr("设置传送带移动速度")}, &(ZMotionParas()->conveyorBeltSpeed));
        };
    };
    static CZMotionParas g_ZMotionParas;
};