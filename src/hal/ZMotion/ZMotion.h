#pragma once
#include "zmotion.h"
#include "zauxdll2.h"
#include "ZMotionParaDef.h"
#include <QObject>
#include <QThread>
#include <QMutex>

class CZMotion : public QObject
{
    Q_OBJECT
public:
    CZMotion(QObject *parentc = nullptr);
    ~CZMotion();

public:
    void readCurrentCoordinates();
    void loadAxisParas();
    void positiveSolutionMode();
    void negativeSolutionMode();
    void backToZero();
    void moveToPoint(float p_targetX, float p_targetY, float p_targetZ);
    void conveyorBeltMoveDistance(float &p_targetX);

signals:
    void sigConnect();
    void sigConnected();
    void sigDisconnected();

public slots:
    void slotConnectHardware();
    void slotConnectDevice();
    void slotDisconnected();
    void slotUpdateMessage();
    void slotInit();
    void slotOpenEnable();
    void slotCloseEnable();

private:
    ZMC_HANDLE handle;
    int axisList[atAxisMax][aiAxisMax] = {{0, 1, 2, 3}, {4, 5, 6, 7}};
    bool m_bConnected;
    uint32_t m_reConnectTimes;
    float m_currentX, m_currentY, m_currentZ;
};

class IMegatronController : public QObject
{
    Q_OBJECT
    QThread workerThread;

public:
    IMegatronController(CZMotion *p_pMegatron)
    {
        p_pMegatron->moveToThread(&workerThread);
        connect(p_pMegatron, &CZMotion::sigConnect, p_pMegatron, &CZMotion::slotConnectDevice);
        // connect(p_pMegatron, &CZMotion::sigConnected, p_pMegatron, &CZMotion::slotConnected);
        connect(p_pMegatron, &CZMotion::sigDisconnected, p_pMegatron, &CZMotion::slotDisconnected);
        // connect(&workerThread, &QThread::started, p_pMegatron, &CZMotion::start);
        workerThread.start();
    };
    ~IMegatronController()
    {
        workerThread.quit();
        workerThread.wait();
    };
};

struct CZMotionState
{
    friend class CZMotion;
public:
    bool getMoveState() const;
    void setMoveState(bool p_isMove);
public:
    bool isMove = false;
    unsigned int isVacuum;
};

CZMotionState *ZMotionState();
