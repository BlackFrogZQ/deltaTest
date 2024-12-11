#pragma once
#include <QObject>
#include <QByteArray>
#include <chrono>
using namespace std;
constexpr int cSenMessageInterval = 50; // 硬件状态刷新间隔

class IZMotionActionState:public QObject
{
    Q_OBJECT
public:
    virtual ~IZMotionActionState();
    virtual void run() = 0;
    virtual void runing();

protected:
    void changeState(IZMotionActionState* p_state);
};


enum CWorkType : unsigned char
{
    camsIdle = 0,
    camsReset,
    camsWorkPlace,
    camsGrabPlace,
    camsStartGrabPlace,
    camsReleasePlace,
    camsMax
};

struct CWorkTypeData
{
public:
    CWorkType workType = camsIdle;
    double p_worldX;
    double p_worldY;
    double p_VisionStartTime;
    double p_VisionEndTime;
};

CWorkTypeData *workTypeData();