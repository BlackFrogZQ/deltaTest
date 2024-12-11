#include "ZAutoWorkState.h"
#include "ZAutoWorkAction.h"
#include "hal/ZMotion/ZMotion.h"
#include "hal/vm.h"
#include "ui/mainWindow.h"
#include <QTimer>
#include <iostream>
#include <cstdlib>
#include <ctime>

CZAutoWorkIdleState::CZAutoWorkIdleState(CZAutoWorkAction *p_action) : m_action(p_action)
{
    assert(m_action != nullptr);
}
CZAutoWorkIdleState::~CZAutoWorkIdleState()
{
}
void CZAutoWorkIdleState::run()
{
    workTypeData()->workType = camsMax;
    myInfo << cnStr("自动工作结束");
    emit m_action->sigEnd();
}

void CRunWrokPlace::run()
{
    assert(ZMotionState()->getMoveState() == false);
    workTypeData()->workType = camsWorkPlace;
    ZMotionState()->setMoveState(true);
    vm()->sendCoordinate(ZMotionParas()->workX, ZMotionParas()->workY, ZMotionParas()->workZ);
    QTimer::singleShot(cSenMessageInterval,this,[this]{runing();});
}
void CRunWrokPlace::runing()
{
    if (m_action->m_bStop && ZMotionState()->getMoveState() == false)
    {
        changeState(m_action->m_idle);
        return;
    }

    if (ZMotionState()->getMoveState() == false)
    {
        changeState(m_action->m_runGrabPlace);
    }
    else
    {
        QTimer::singleShot(cSenMessageInterval,this,[this]{runing();});
    }
}

void CRunGrabPlace::run()
{
    assert(ZMotionState()->getMoveState() == false);
    workTypeData()->workType = camsGrabPlace;
    m_pSendGrabOrder = false;
    QTimer::singleShot(cSenMessageInterval,this,[this]{runing();});
}
void CRunGrabPlace::runing()
{
    if (m_action->m_bStop && ZMotionState()->getMoveState() == false)
    {
        changeState(m_action->m_idle);
        return;
    }

    if(mainWindow()->m_targetNum > 0 && m_pSendGrabOrder == false)
    {
        workTypeData()->p_worldX = mainWindow()->m_targetX;
        workTypeData()->p_worldY = mainWindow()->m_targetY;
        vm()->sendCoordinate(mainWindow()->m_targetX, mainWindow()->m_targetY, ZMotionParas()->grabZUp);
        ZMotionState()->setMoveState(true);
        m_pSendGrabOrder = true;
    }
    else if (ZMotionState()->getMoveState() == false && m_pSendGrabOrder == true)
    {
        changeState(m_action->m_runStartGrabPlace);
        return;
    }
    QTimer::singleShot(cSenMessageInterval,this,[this]{runing();});
}

void CRunStartGrabPlace::run()
{
    assert(ZMotionState()->getMoveState() == false);
    m_pRunCount = 0;
    workTypeData()->workType = camsStartGrabPlace;
    vm()->sendCoordinate(workTypeData()->p_worldX, workTypeData()->p_worldY, ZMotionParas()->grabZDown);
    ZMotionState()->setMoveState(true);
    QTimer::singleShot(cSenMessageInterval,this,[this]{runing();});
}
void CRunStartGrabPlace::runing()
{
    if (m_action->m_bStop && ZMotionState()->getMoveState() == false)
    {
        changeState(m_action->m_idle);
        return;
    }
    else if(m_pRunCount > 20)
    {
        changeState(m_action->m_runWorkPlace);
        return;
    }

    if (ZMotionState()->getMoveState() == false && ZMotionState()->isVacuum == true)
    {
        changeState(m_action->m_runReleasePlace);
    }
    else
    {
        QTimer::singleShot(cSenMessageInterval,this,[this]{runing();});
    }
    m_pRunCount++;
}

void CRunReleasePlace::run()
{
    assert(ZMotionState()->getMoveState() == false);
    workTypeData()->workType = camsReleasePlace;
    vm()->sendCoordinate(ZMotionParas()->placeX, ZMotionParas()->placeY, ZMotionParas()->placeZ);
    ZMotionState()->setMoveState(true);
    QTimer::singleShot(cSenMessageInterval,this,[this]{runing();});
}
void CRunReleasePlace::runing()
{
    if (m_action->m_bStop && ZMotionState()->getMoveState() == false)
    {
        changeState(m_action->m_idle);
        return;
    }

    if (ZMotionState()->getMoveState() == false)
    {
        changeState(m_action->m_runWorkPlace);
    }
    else
    {
        QTimer::singleShot(cSenMessageInterval,this,[this]{runing();});
    }
}
