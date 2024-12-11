#include "vm.h"
#include "./ZMotionAction/iZMotionActionState.h"
#include <QTimer>
#include <QThread>

static CVM *g_Vm = nullptr;
CVM *vm()
{
    return g_Vm;
}

CVM::CVM(QObject *p)
    : QObject(p),
      m_pZMotion(nullptr),
      m_pResetAction(nullptr),
      m_pAutoWorkAction(nullptr),
      m_state(vmIdle)
{
    g_Vm = this;

    m_pZMotion = new CZMotion;
    // m_pZMotion = new CZMotion(this); // 无法使用多线程
    m_pHardwareControl = new IMegatronController(m_pZMotion);
    m_pZMotion->slotConnectHardware();

    CZMotionActionCreater creater(this);
    m_pResetAction = creater.resetAction();
    m_pAutoWorkAction = creater.autoWorkAction();
    connect(m_pResetAction, &IZMotionAction::sigEnd, [this]{
        if(m_state == vmReset)
        {
            changeState(vmIdle);
            workTypeData()->workType = camsIdle;
        } });
    connect(m_pAutoWorkAction, &IZMotionAction::sigEnd, [this]{
        if(m_state == vmAutoWork)
        {
            reset();
        } });
}

CVM::~CVM()
{
    delPtr(m_pZMotion);
    delPtr(m_pResetAction);
    delPtr(m_pAutoWorkAction);
}

CVMState CVM::vmState() const
{
    return m_state;
}

void CVM::sendCoordinate(const double p_x, const double p_y, const double p_z)
{
    m_pZMotion->moveToPoint(p_x, p_y, p_z);
}

void CVM::reset()
{
    workTypeData()->workType = camsReset;
    changeState(vmReset);
    m_pResetAction->start();
}
void CVM::stopReset()
{
    assert(m_state == vmReset);
    m_pResetAction->stop();
}

void CVM::autoWork()
{
    workTypeData()->workType = camsWorkPlace;
    changeState(vmAutoWork);
    m_pAutoWorkAction->start();
}
void CVM::stopWork()
{
    assert(m_state == vmAutoWork);
    m_pAutoWorkAction->stop();
}

void CVM::changeState(CVMState nextState)
{
    m_state = nextState;
    myDebug << cnStr("当前vm状态：") << nextState;
    emit sigVMStateUpdate();
}
