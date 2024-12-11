#pragma once
#include "../iZMotionAction.h"
class IZMotionActionState;

class CZResetAction : public IZMotionAction
{
    friend class CZResetIdleState;
    friend class CZResetWaitFinisedState;
    Q_OBJECT
public:
    CZResetAction(CVM *p_vm);
    ~CZResetAction();
    virtual void start() override;
protected:
    IZMotionActionState* m_idle;
    IZMotionActionState* m_waitFinised;
};
