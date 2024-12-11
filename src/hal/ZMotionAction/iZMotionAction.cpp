#include "iZMotionAction.h"
#include "ZMotionReset/ZResetAction.h"
#include "ZMotionAutoWork/ZAutoWorkAction.h"
#include "../vm.h"

IZMotionAction::IZMotionAction(CVM *p_vm) : QObject(nullptr), m_pVM(p_vm), m_bStop(false)
{
    assert(m_pVM != nullptr);
}

IZMotionAction::~IZMotionAction()
{
}

void IZMotionAction::start()
{
    m_bStop = false;
}

void IZMotionAction::stop()
{
    m_bStop = true;
}


CZMotionActionCreater::CZMotionActionCreater(CVM *p_vm) : m_pVM(p_vm)
{
    assert(m_pVM != nullptr);
}

IZMotionAction * CZMotionActionCreater::resetAction()
{
    return new CZResetAction(m_pVM);
}

IZMotionAction *CZMotionActionCreater::autoWorkAction()
{
    return new CZAutoWorkAction(m_pVM);
}
