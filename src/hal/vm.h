#pragma once
#include "vmDef.h"
#include "ZMotion/ZMotion.h"
#include "ZMotionAction/iZMotionAction.h"
#include <QObject>

class CVM:public QObject
{
    friend class CVmControl;
    Q_OBJECT
public:
    CVMState vmState() const;
    void sendCoordinate(const double p_x, const double p_y, const double p_z);
    void reset();
    void stopReset();
    void autoWork();
    void stopWork();

signals:
    void sigVMStateUpdate();

protected:
    CVM(QObject* p = nullptr);
    ~CVM();
    void changeState(CVMState nextState);

protected:
    CZMotion* m_pZMotion;
    IMegatronController *m_pHardwareControl;
    IZMotionAction* m_pResetAction;
    IZMotionAction* m_pAutoWorkAction;
    CVMState m_state;
};
CVM* vm();