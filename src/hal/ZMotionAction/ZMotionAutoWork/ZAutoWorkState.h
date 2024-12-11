#pragma once
#include "../iZMotionActionState.h"
class CZAutoWorkAction;

class CZAutoWorkIdleState: public IZMotionActionState
{
    Q_OBJECT
public:
    CZAutoWorkIdleState(CZAutoWorkAction* p_action);
    ~CZAutoWorkIdleState();
    virtual void run() override;

protected:
    CZAutoWorkAction *m_action;
};

class CRunWrokPlace: public CZAutoWorkIdleState
{
    Q_OBJECT
public:
    using CZAutoWorkIdleState::CZAutoWorkIdleState;
    virtual void run() override;
    virtual void runing() override;
};

class CRunGrabPlace: public CZAutoWorkIdleState
{
    Q_OBJECT
public:
    using CZAutoWorkIdleState::CZAutoWorkIdleState;
    virtual void run() override;
    virtual void runing() override;

private:
    bool m_pSendGrabOrder;
};

class CRunStartGrabPlace: public CZAutoWorkIdleState
{
    Q_OBJECT
public:
    using CZAutoWorkIdleState::CZAutoWorkIdleState;
    virtual void run() override;
    virtual void runing() override;

private:
    uint m_pRunCount;
};

class CRunReleasePlace: public CZAutoWorkIdleState
{
    Q_OBJECT
public:
    using CZAutoWorkIdleState::CZAutoWorkIdleState;
    virtual void run() override;
    virtual void runing() override;
};
