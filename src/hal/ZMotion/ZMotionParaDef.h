#pragma once
#include "system/basic.h"
#include <QString>
#include <QStringList>

constexpr int cSendInterval = 1;           // 消息发送间隔
constexpr int cUpdateStatusInterval = 50; // 硬件状态刷新间隔
constexpr int cReconnectInterval = 1000;   // 硬件掉线后重连时间

#pragma region "基础轴定义"
    enum CAxisType : int
    {
        atJoint = 0,
        atVirtual,
        atAxisMax
    };

    enum CAxisID : int
    {
        aiX = 0,
        aiY,
        aiZ,
        aiA,
        aiAxisMax
    };

    enum CVirtualAxisID : int
    {
        vaiX = 4,
        vaiY,
        vaiZ,
        vaiA,
        vaiAxisMax
    };

    const QStringList CAxisTypeEn = {cnStr("Joint"), cnStr("Virtual")};
    const QStringList CAxisTypeStr = {cnStr("关节轴"), cnStr("虚拟轴")};
    const QStringList CAxisIDStr = {cnStr("X"), cnStr("Y"), cnStr("Z"), cnStr("A")};

    enum CAxisTypeID : int
    {
        atiJointX = 0,
        atiJointY,
        atiJointZ,
        atiJointA,
        atiVirtualX,
        atiVirtualY,
        atiVirtualZ,
        atiVirtualA,
        atiAxisMax
    };
    const QStringList CAxisTypeIDStr = {cnStr("关节轴X"), cnStr("关节轴Y"), cnStr("关节轴Z"), cnStr("关节轴A"), 
                                        cnStr("虚拟轴X"), cnStr("虚拟轴Y"), cnStr("虚拟轴Z"), cnStr("虚拟轴A")};
    const QStringList CAxisTypeIDEn = {cnStr("JointX"), cnStr("JointY"), cnStr("JointZ"), cnStr("JointA"), 
                                        cnStr("VirtualX"), cnStr("VirtualY"), cnStr("VirtualZ"), cnStr("VirtualA")};
#pragma endregion

#pragma region "参数"
    struct CTabelPara
    {
        float storageLocation = 0.0;            //存储转换参数Table的位置
        float quietPlatformR = 160.0;           //静平台半径
        float movePlatformL = 55.0;             //动平台长度
        float masterArmL = 250.0;               //主动臂长
        float slaveArmL = 600.0;                //从动臂长
        float axisPulse1 = 36000.0;              //关节轴1转一圈所需要的脉冲数
        float axisPulse2 = 36000.0;              //关节轴2转一圈所需要的脉冲数
        float axisPulse3 = 36000.0;              //关节轴3转一圈所需要的脉冲数
        float offsetX = 0.0;                    //末端与下面中心点的x偏移
        float offsetY = 0.0;                    //末端与下面中心点的y偏移
        float offsetZ = -100.0;                    //末端与下面中心点的z偏移
        float axisPulseA = 36000.0;                 // 旋转轴脉冲
    };
    struct CAxisPara
    {
        int setAtype = 7;             // 设置轴类型
        int setDpos = 0;              // 设置DPOS值
        int setDatumIn = 2;           // 设置原点信号
        int setUnits = 36000/360;     // 设置轴脉冲当量
        int setSpeed = 200;           // 设置轴最大速度
        int setAccel = 1000;          // 设置轴的加速度
        int setDecel = 1000;          // 设置轴的减速度
        int setSramp = 50;            // 设置轴的s曲线
        int setCreep = 10;            // 关节轴回爬速度
    };
    struct CZMotionParas
    {
        QString ip = "192.168.0.11";
        CTabelPara tabelPara;
        CAxisPara axisPara[atAxisMax][aiAxisMax];
        CAxisPara axisIdPara[atiAxisMax];
        double zeroX = 0.0, zeroY = 0.0, zeroZ = -583.709;
        double workX = 0.0, workY = 0.0, workZ = -630.0;
        double placeX = 50.0, placeY = 50.0, placeZ = -630.0;
        double grabZUp = -630, grabZDown = -650;
        uint conveyorBeltSpeed = 50;
    };
    CZMotionParas *ZMotionParas();
#pragma endregion
