#include "ZResetAction.h"
#include "ZResetState.h"
#include "hal/vm.h"

CZResetAction::CZResetAction(CVM *p_vm):IZMotionAction(p_vm)
{
    m_idle = new CZResetIdleState(this);
    m_waitFinised = new CZResetWaitFinisedState(this);
}

CZResetAction::~CZResetAction()
{
    delPtr(m_idle);
    delPtr(m_waitFinised);
}

void CZResetAction::start()
{
    IZMotionAction::start();
    m_waitFinised->run();
}