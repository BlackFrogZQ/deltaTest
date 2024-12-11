
#pragma once
#include <QObject>
class CVM;

class IZMotionAction : public QObject
{
    Q_OBJECT
public:
    IZMotionAction(CVM *p_vm);
    virtual ~IZMotionAction();
    virtual void start();
    virtual void stop();

signals:
    void sigEnd();

protected:
    CVM *m_pVM;
    bool m_bStop;
};


class CZMotionActionCreater
{
public:
    CZMotionActionCreater(CVM *p_vm);
    IZMotionAction *resetAction();
    IZMotionAction *autoWorkAction();

protected:
    CVM *m_pVM;
};