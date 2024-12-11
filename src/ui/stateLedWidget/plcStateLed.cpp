#include "plcStateLed.h"
#include "ui/mainWindowDef.h"
#include "hal/vm.h"
#include "hal/ZMotion/ZMotion.h"
#include "hal/ZMotionAction/iZMotionActionState.h"
#include <QVBoxLayout>
#include <QTimer>

CPlcStateLed::CPlcStateLed(QWidget *parent) : QLabel(parent)
{
    this->setObjectName("PlcStateWidget");
    this->setStyleSheet(QString("#PlcStateWidget{%1};").arg(cStyleSheet ));

    auto getLed = [](QString name)
    {
        auto led = new QLabel(name);
        led->setStyleSheet("QLabel{background-color:rgb(0,255,0)}QLabel:disabled{background:transparent};");
        led->setFixedHeight(40);
        led->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        led->setAlignment(Qt::AlignCenter);
        return led;
    };

    m_states[reset] = getLed(cnStr("  复位  "));
    m_states[autoRun] = getLed(cnStr("自动运行"));
    m_states[grab] = getLed(cnStr("夹取包装"));
    m_states[release] = getLed(cnStr("释放包装"));
    initLayout();
    plcStateUpdate();
    connect(vm(), &CVM::sigVMStateUpdate, this, &CPlcStateLed::plcStateUpdate);
}

CPlcStateLed::~CPlcStateLed()
{
}

void CPlcStateLed::initLayout()
{
    //信号
    QWidget *plcStateWidget = new QWidget;
    QGridLayout *plcStateLayout = new QGridLayout;
    plcStateLayout->addWidget(m_states[reset], 0, 0, 1, 1);
    plcStateLayout->addWidget(m_states[autoRun], 0, 1, 1, 1);
    plcStateLayout->addWidget(m_states[grab], 1, 0, 1, 1);
    plcStateLayout->addWidget(m_states[release], 1, 1, 1, 1);
    plcStateLayout->setMargin(0);
    plcStateLayout->setVerticalSpacing(30);
    plcStateLayout->setSizeConstraint(QGridLayout::SetMinimumSize);
    plcStateWidget->setLayout(plcStateLayout);

    //标题
    QWidget *titleWidget = new QWidget;
    QLabel *titleLable = new QLabel(cnStr("指示信号"));
    titleLable->setStyleSheet("QLabel{background-color:rgb(255, 255, 255,15)};");
    QFont titleFont;
    titleFont.setPointSize(10);
    titleLable->setFont(titleFont);
    titleLable->setFixedHeight(30);
    titleLable->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    titleLable->setAlignment(Qt::AlignCenter);

    QHBoxLayout *pTitleLayout = new QHBoxLayout;
    pTitleLayout->addWidget(titleLable);
    pTitleLayout->setMargin(0);
    pTitleLayout->setSizeConstraint(QHBoxLayout::SetMinimumSize);
    titleWidget->setLayout(pTitleLayout);

    //汇总
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(titleWidget);
    pLayout->addWidget(plcStateWidget);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->setSizeConstraint(QVBoxLayout::SetMinimumSize);
    this->setMinimumSize(pLayout->sizeHint());
    this->setLayout(pLayout);
}

void CPlcStateLed::setAllLed(bool p_enable)
{
    for (size_t i = 0; i < cltMax; i++)
    {
        m_states[i]->setEnabled(p_enable);
    }
}

void CPlcStateLed::plcStateUpdate()
{
    switch (workTypeData()->workType)
    {
    case camsReset:
        setAllLed(false);
        m_states[reset]->setEnabled(true);
        break;
    case camsWorkPlace:
        setAllLed(false);
        m_states[autoRun]->setEnabled(true);
        m_states[grab]->setEnabled(false);
        m_states[release]->setEnabled(false);
        break;
    case camsGrabPlace:
    case camsStartGrabPlace:
        setAllLed(false);
        m_states[autoRun]->setEnabled(true);
        m_states[grab]->setEnabled(true);
        m_states[release]->setEnabled(false);
        break;
    case camsReleasePlace:
        setAllLed(false);
        m_states[autoRun]->setEnabled(true);
        m_states[grab]->setEnabled(false);
        m_states[release]->setEnabled(true);
        break;
    default:
        setAllLed(false);
        return;
        break;
    }
    QTimer::singleShot(100, this, &CPlcStateLed::plcStateUpdate);
}