#pragma once
#include <QLabel>
#include <QVector>

class CPlcStateLed:public QLabel
{
    Q_OBJECT
public:
    CPlcStateLed(QWidget *parent=nullptr);
    ~CPlcStateLed();

public slots:
    void plcStateUpdate();

protected:
    enum CLedType
    {
        reset,
        autoRun,
        grab,
        release,
        cltMax
    };
    void initLayout();
    void setAllLed(bool p_enable);
protected:
    QLabel* m_states[cltMax];
};
