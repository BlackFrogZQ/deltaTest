#include "ZAutoWorkAction.h"
#include "ZAutoWorkState.h"
#include "hal/vm.h"

CZAutoWorkAction::CZAutoWorkAction(CVM *p_vm): IZMotionAction(p_vm)
{
    m_idle = new CZAutoWorkIdleState(this);
    m_runWorkPlace = new CRunWrokPlace(this);
    m_runGrabPlace = new CRunGrabPlace(this);
    m_runStartGrabPlace = new CRunStartGrabPlace(this);
    m_runReleasePlace = new CRunReleasePlace(this);
}

CZAutoWorkAction::~CZAutoWorkAction()
{
    delPtr(m_idle);
    delPtr(m_runWorkPlace);
    delPtr(m_runGrabPlace);
    delPtr(m_runStartGrabPlace);
    delPtr(m_runReleasePlace);
}

void CZAutoWorkAction::start()
{
    IZMotionAction::start();
    m_runWorkPlace->run();
}

void CZAutoWorkAction::stop()
{
    IZMotionAction::stop();
}
