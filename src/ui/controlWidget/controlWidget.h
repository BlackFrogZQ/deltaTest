#pragma once
#include "ui/basic.h"
#include "ui/mainWindowDef.h"
#include "system/basic.h"
#include <QLabel>
class QPushButton;

class ControlWidget : public QLabel
{
    Q_OBJECT

public:
    ControlWidget();
    virtual ~ControlWidget();

protected:
    void initLayout();

protected slots:
    void vmStateUpdate();

protected:
    QPushButton *m_reset;
    QPushButton *m_startRun;
    QPushButton *m_crashStop;
    QPushButton *m_systemPara;

    QLabel *m_allCount;
    QLabel *m_okCount;
    QLabel *m_ngCount;
};