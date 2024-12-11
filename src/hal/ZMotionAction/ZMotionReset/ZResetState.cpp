#include "ZResetState.h"
#include "ZResetAction.h"
#include "hal/ZMotion/ZMotion.h"
#include "hal/vm.h"
#include <QTimer>

CZResetIdleState::CZResetIdleState(CZResetAction *p_resetAction) : m_resetAction(p_resetAction)
{
    assert(m_resetAction != nullptr);
}
CZResetIdleState::~CZResetIdleState()
{
}
void CZResetIdleState::run()
{
    myInfo << cnStr("复位结束") ;
    emit m_resetAction->sigEnd();
}

void CZResetWaitFinisedState::run()
{
    assert(ZMotionState()->getMoveState() == false);
    myInfo << cnStr("开始复位，等待复位完成");
    workTypeData()->workType = camsReset;
    ZMotionState()->setMoveState(true);
    vm()->sendCoordinate(ZMotionParas()->zeroX, ZMotionParas()->zeroY, ZMotionParas()->zeroZ);
    QTimer::singleShot(cSenMessageInterval,this,[this]{runing();});
}
void CZResetWaitFinisedState::runing()
{
    if (m_resetAction->m_bStop)
    {
        changeState(m_resetAction->m_idle);
        return;
    }

    if (ZMotionState()->getMoveState() == false)
    {
        changeState(m_resetAction->m_idle);
    }
    else
    {
        QTimer::singleShot(cSenMessageInterval,this,[this]{runing();});
    }
}