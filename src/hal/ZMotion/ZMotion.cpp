#include "ZMotion.h"
#include "string.h"
#include "ZMotionParaDef.h"
#include "hal/ZMotionAction/iZMotionActionState.h"
#include <QTimer>

CZMotion::CZMotion(QObject *parent): QObject(parent), handle(NULL), m_bConnected(false), m_reConnectTimes(0)
{
}

CZMotion::~CZMotion()
{
    if(m_bConnected == true)
    {
        slotCloseEnable();
        ZAux_Close(handle);
    }
}

void CZMotion::slotConnectHardware()
{
    if (m_bConnected)
    {
        return;
    }
    emit sigConnect();
}

void CZMotion::slotConnectDevice()
{
    if (m_bConnected)
    {
        return;
    }
    QByteArray ipBytes = ZMotionParas()->ip.toUtf8();
    char* pIp = ipBytes.data();
    m_bConnected = ZAux_OpenEth(pIp, &handle) == 0 ? true : false;
    if(m_bConnected)
    {
        myInfo << cnStr("控制卡：%1已连接").arg(ZMotionParas()->ip);
        slotInit();
        emit sigConnected();
        return;
    }
    emit sigDisconnected();
}

void CZMotion::slotDisconnected()
{
    m_bConnected = false;
    if (m_reConnectTimes % 10 == 0)
    {
        // myInfo << cnStr("控制卡：%1已断开连接").arg(ZMotionParas()->ip) + cnStr("。尝试第%1次重连").arg((m_reConnectTimes == 0) ? 1 : m_reConnectTimes); // 子线程中尝试直接操作GUI(如访问QWidget或其他UI组件),则可能会导致跨线程访问问题。
        qDebug() << cnStr("控制卡：%1已断开连接").arg(ZMotionParas()->ip) + cnStr("。尝试第%1次重连").arg((m_reConnectTimes == 0) ? 1 : m_reConnectTimes);
    }
    m_reConnectTimes++;
    QTimer::singleShot(cReconnectInterval, this, &CZMotion::slotConnectDevice);
}

void CZMotion::slotUpdateMessage()
{
    if (m_bConnected == false || handle == NULL)
    {
        return;
    }
    // 读DI状态
    ZAux_Direct_GetIn(handle, 0, &ZMotionState()->isVacuum);

    // 读DO状态

    // 读虚拟轴运动状态
    bool allAxesIdle = true;
    bool allBuffersEmpty = true;
    for (int axisId = 0; axisId < atAxisMax; axisId++)
    {
        int runStatus, bufferStatus;
        CAxisType pAxisType = workTypeData()->workType == camsReset ? atJoint : atVirtual;
        ZAux_Direct_GetIfIdle(handle, axisList[pAxisType][axisId], &runStatus);
        ZAux_Direct_GetLoaded(handle, axisList[pAxisType][axisId], &bufferStatus);
        allAxesIdle &= (runStatus == -1);
        allBuffersEmpty &= (bufferStatus == -1);
    }
    ZMotionState()->setMoveState(!(allAxesIdle && allBuffersEmpty));

    if(ZMotionState()->getMoveState() == true)
    {
        QTimer::singleShot(cUpdateStatusInterval, this, &CZMotion::slotUpdateMessage);
    }
    else if(workTypeData()->workType == camsStartGrabPlace && ZMotionState()->isVacuum == false)
    {
        QTimer::singleShot(cUpdateStatusInterval, this, &CZMotion::slotUpdateMessage);
    }
    else if(workTypeData()->workType == camsReset)
    {
        negativeSolutionMode();
        ZAux_Direct_SetOp(handle, 3, 0);
        ZAux_Direct_SetOp(handle, 4, 0);
    }
    ZAux_Direct_SetOp(handle, 4, 0);
}

void CZMotion::slotInit()
{
    loadAxisParas();
    slotOpenEnable();
    negativeSolutionMode();
    readCurrentCoordinates();
    // slotUpdateMessage();
}

void CZMotion::loadAxisParas()
{
    float parameterTableList[12] = {ZMotionParas()->tabelPara.storageLocation,
                      ZMotionParas()->tabelPara.quietPlatformR,
                      ZMotionParas()->tabelPara.movePlatformL,
                      ZMotionParas()->tabelPara.masterArmL,
                      ZMotionParas()->tabelPara.slaveArmL,
                      ZMotionParas()->tabelPara.axisPulse1,
                      ZMotionParas()->tabelPara.axisPulse2,
                      ZMotionParas()->tabelPara.axisPulse3,
                      ZMotionParas()->tabelPara.offsetX,
                      ZMotionParas()->tabelPara.offsetY,
                      ZMotionParas()->tabelPara.offsetZ,
                      ZMotionParas()->tabelPara.axisPulseA};
    ZAux_Direct_SetTable(handle, 0, 12, parameterTableList);

    for (size_t axisType = 0; axisType < atAxisMax; axisType++)
    {
        for (size_t axisId = 0; axisId < aiAxisMax; axisId++)
        {
            int axisIdSize = axisList[axisType][axisId];
            ZAux_Direct_SetAtype(handle, axisList[axisType][axisId], ZMotionParas()->axisPara[axisType][axisId].setAtype);
            // ZAux_Direct_SetDpos(handle, axisList[axisType][axisId], ZMotionParas()->axisPara[axisType][axisId].setDpos);
            // ZAux_Direct_SetDatumIn(handle, axisList[axisType][axisId],  ZMotionParas()->axisPara[axisType][axisId].setDatumIn);
            ZAux_Direct_SetUnits(handle, axisList[axisType][axisId], ZMotionParas()->axisPara[axisType][axisId].setUnits);
            ZAux_Direct_SetSpeed(handle, axisList[axisType][axisId], ZMotionParas()->axisPara[axisType][axisId].setSpeed);
            ZAux_Direct_SetAccel(handle, axisList[axisType][axisId], ZMotionParas()->axisPara[axisType][axisId].setAccel);
            ZAux_Direct_SetDecel(handle, axisList[axisType][axisId], ZMotionParas()->axisPara[axisType][axisId].setDecel);
            ZAux_Direct_SetSramp(handle, axisList[axisType][axisId], ZMotionParas()->axisPara[axisType][axisId].setSramp);
            ZAux_Direct_SetCreep(handle, axisList[axisType][axisId], ZMotionParas()->axisPara[axisType][axisId].setCreep);
        }
    }
}

void CZMotion::slotOpenEnable()
{
    ZAux_Direct_SetOp(handle, 5, 1);       //关闭抱闸
    ZAux_Direct_SetOp(handle, 12, 1);      //axis1 打开使能
    ZAux_Direct_SetOp(handle, 13, 1);      //axis2 打开使能
    ZAux_Direct_SetOp(handle, 14, 1);      //axis3 打开使能
}

void CZMotion::slotCloseEnable()
{
    ZAux_Direct_SetOp(handle, 5, 0);       //打开抱闸
    ZAux_Direct_SetOp(handle, 12, 0);      //axis1 关闭使能
    ZAux_Direct_SetOp(handle, 13, 0);      //axis2 关闭使能
    ZAux_Direct_SetOp(handle, 14, 0);      //axis3 关闭使能
}

void CZMotion::readCurrentCoordinates()
{
    float dpos[4];
    for (size_t axisType = 0; axisType < atAxisMax; axisType++)
    {
        for (size_t axisId = 0; axisId < aiAxisMax; axisId++)
        {
            ZAux_Direct_GetDpos(handle, axisList[axisType][axisId], &dpos[axisId]);
        }
        if(axisType == atJoint)
        {
            // myInfo << cnStr("关节轴坐标：0=%1,1=%2,2=%3,3=%4").arg(dpos[0]).arg(dpos[1]).arg(dpos[2]).arg(dpos[3]);
        }
        else
        {
            m_currentX = dpos[0];
            m_currentY = dpos[1];
            m_currentZ = dpos[2];
            // myInfo << cnStr("虚拟轴坐标：x=%1,y=%2,z=%3,a=%4").arg(dpos[0]).arg(dpos[1]).arg(dpos[2]).arg(dpos[3]);
        }
    }
}

void CZMotion::positiveSolutionMode()
{
    ZAux_Direct_Connreframe(handle, 4, axisList[atVirtual], 14, 1, 4, axisList[atJoint]);
    int axistype[4]={0, 0, 0, 0};
    for(int i = 0; i < 4; i++)
    {
        ZAux_Direct_GetMtype(handle, i+4, &axistype[i]);
    }
    if(axistype[0] == 34 && axistype[1] == 34 && axistype[2] == 34 && axistype[3] == 34)
    {
        for(int i = 0; i < 4; i++)
        {
            myDebug << cnStr("Mtype；").arg(axistype[i]);
        }
    }
    else
    {
        myDebug << cnStr("mtype转换不成功");
    }
}

void CZMotion::negativeSolutionMode()
{
    ZAux_Direct_Connframe(handle, 4, axisList[atJoint], 14, 1, 4, axisList[atVirtual]);
    int axistype[4] = {0 , 0, 0, 0};
    for(int i = 0; i < 4; i++)
    {
        ZAux_Direct_GetMtype(handle, i, &axistype[i]);
    }
    if(axistype[0] == 33 && axistype[1] == 33 && axistype[2] == 33 && axistype[3] == 33)
    {
        for(int i = 0; i < 4; i++)
        {
            myDebug << cnStr("Mtype：").arg(axistype[i]);
        }
    }
    else
    {
        myDebug << cnStr("mtype转换不成功");
    }
}

void CZMotion::backToZero()
{
    if (m_bConnected == false || handle == NULL)
    {
        return;
    }
    int virtualAxisType[4];
    float jointDpos[4];
    for(int i=0; i<4; i++)
    {
        ZAux_Direct_GetMtype(handle, axisList[atVirtual][i], &virtualAxisType[i]);
        ZAux_Direct_GetDpos(handle, axisList[atJoint][i], &jointDpos[i]);
    }
    if(virtualAxisType[0]==34 && virtualAxisType[1]==34 && virtualAxisType[2]==34 && virtualAxisType[3]==34)
    {
        for (int i=0; i<4; i++)
        {
            ZAux_Direct_Single_Move(handle, i, -1 * jointDpos[i]);
        }
    }
    else
    {
        positiveSolutionMode();
        backToZero();
    }
}

void CZMotion::moveToPoint(float p_targetX, float p_targetY, float p_targetZ)
{
    if (m_bConnected == false || handle == NULL)
    {
        return;
    }
    ZMotionState()->setMoveState(true);

    readCurrentCoordinates();
    switch (workTypeData()->workType)
    {
    case camsReset:
        backToZero();
        break;
    case camsStartGrabPlace:
    {
        ZAux_Direct_SetOp(handle, 3, 1); // 电磁阀1为1抽真空
        ZAux_Direct_Single_Move(handle, axisList[atVirtual][aiZ], ZMotionParas()->grabZDown - ZMotionParas()->grabZUp);
        break;
    }
    case camsGrabPlace:
    case camsWorkPlace:
    {
        ZAux_Direct_SetOp(handle, 3, 0);
        ZAux_Direct_SetOp(handle, 4, workTypeData()->workType == camsWorkPlace ? 1 : 0); // 电磁阀2为1破真空
        if(workTypeData()->workType == camsGrabPlace)
        {
            conveyorBeltMoveDistance(p_targetX);
        }
        float moveDistance[3] = {p_targetX - m_currentX, p_targetY - m_currentY, p_targetZ - m_currentZ};
        ZAux_Direct_Move(handle, 3, axisList[atVirtual], moveDistance);
        break;
    }
    case camsReleasePlace:
    {
        float middleX, middleY, middleZ;
        float parameterList1[3];
        float parameterList2[3];
        middleX = (p_targetX + m_currentX)/2.0;
        middleY = (p_targetY + m_currentY)/2.0;
        middleZ = (p_targetZ + m_currentZ)/2+fabs(p_targetZ - m_currentZ) + 30.0;
        parameterList1[0] = p_targetX - m_currentX;
        parameterList1[1] = p_targetY - m_currentY;
        parameterList1[2] = p_targetZ - m_currentZ;
        parameterList2[0] = middleX - m_currentX;
        parameterList2[1] = middleY - m_currentY;
        parameterList2[2] = middleZ - m_currentZ;
        ZAux_Direct_MSpherical(handle, 3, axisList[atVirtual], parameterList1[0], parameterList1[1], parameterList1[2], parameterList2[0], parameterList2[1], parameterList2[2], 0, 0, 0);
        break;
    }
    default:
        break;
    }
    QTimer::singleShot(cUpdateStatusInterval, this, &CZMotion::slotUpdateMessage);
}

void CZMotion::conveyorBeltMoveDistance(float &p_targetX)
{
    float moveDistance = abs(p_targetX - m_currentX);
    int pSpeed = ZMotionParas()->axisPara[atVirtual][atiJointX].setSpeed;
    int pAccel = ZMotionParas()->axisPara[atVirtual][atiJointX].setAccel;
    float acceAndDeceDistance = pow(pSpeed, 2) / (2 * pAccel);
    float pMoveDistance, pMoveTime;
    if(moveDistance > 2 * acceAndDeceDistance)
    {
        pMoveTime = (pow(pSpeed, 2) + (pAccel * moveDistance)) / (pAccel * pSpeed);
    }
    else
    {
        pMoveTime = 2 * sqrt(moveDistance / pAccel);
    }
    pMoveDistance = round((pMoveTime * ZMotionParas()->conveyorBeltSpeed) * 1000 ) / 1000;
    myInfo << cnStr("运动时间：") << pMoveTime;
    p_targetX = abs(p_targetX - pMoveDistance) > 300 ? p_targetX : p_targetX - pMoveDistance;
}


CZMotionState *ZMotionState()
{
    static CZMotionState p_ZMotionState;
    return &p_ZMotionState;
}

bool CZMotionState::getMoveState() const
{
    return isMove;
}

void  CZMotionState::setMoveState(bool p_isMove)
{
    isMove = p_isMove;
}