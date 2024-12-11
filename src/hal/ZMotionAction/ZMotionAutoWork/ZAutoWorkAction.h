#pragma once
#include "../iZMotionAction.h"
class IZMotionActionState;
class CZAutoWorkAction : public IZMotionAction
{
    friend class CZAutoWorkIdleState;
    friend class CRunWrokPlace;
    friend class CRunGrabPlace;
    friend class CRunStartGrabPlace;
    friend class CRunReleasePlace;

    Q_OBJECT
public:
    CZAutoWorkAction(CVM *p_vm);
    ~CZAutoWorkAction();

    virtual void start() override;
    virtual void stop() override;

protected:
    IZMotionActionState* m_idle;
    IZMotionActionState* m_runWorkPlace;
    IZMotionActionState* m_runGrabPlace;
    IZMotionActionState* m_runStartGrabPlace;
    IZMotionActionState* m_runReleasePlace;
};
