#pragma once
#include "../iZMotionActionState.h"
class CZResetAction;

class CZResetIdleState:public IZMotionActionState
{
    Q_OBJECT
public:
    CZResetIdleState(CZResetAction* p_resetAction);
    ~CZResetIdleState();
    virtual void run() override;
protected:
    CZResetAction* m_resetAction;
};

class CZResetWaitFinisedState:public CZResetIdleState
{
    Q_OBJECT
public:
    using CZResetIdleState::CZResetIdleState;
    virtual void run() override;
    virtual void runing() override;
};