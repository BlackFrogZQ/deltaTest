#include "controlWidget.h"
#include "ui/mainWindow.h"
#include "ui/setParaWindow/setParaWindow.h"
#include "ui/inputDialog/inputDialog.h"
#include "hal/vm.h"
#include "hal/camera/baslerCamera.h"
#include "system/systemService.h"
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>

ControlWidget::ControlWidget(): QLabel(nullptr)
{
    this->setObjectName("ControlWidget");
    this->setStyleSheet(QString("#ControlWidget{%1};").arg(cStyleSheet ));
    initLayout();
    connect(vm(), &CVM::sigVMStateUpdate, this, &ControlWidget::vmStateUpdate);
    vmStateUpdate();
}

ControlWidget::~ControlWidget()
{
}

void ControlWidget::initLayout()
{
    auto setAttr = [](QWidget* p_w)
    {
        p_w->setFixedHeight(50);
        p_w->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    };
    auto getLed = [setAttr](QString name)
    {
        auto led = new QLabel(name);
        setAttr(led);
        led->setAlignment(Qt::AlignCenter);
        return led;
    };

    //运动控制
    m_reset = new QPushButton(cnStr("复位"));
    setAttr(m_reset);
    connect(m_reset, &QPushButton::clicked, this, [=](){vm()->reset(); });
    m_startRun = new QPushButton(cnStr("开始运行"));
    setAttr(m_startRun);
    connect(m_startRun, &QPushButton::clicked, this, [=](){
            mainWindow()->slotRecognitionPackage(true);
            vm()->autoWork();
        });
    m_crashStop = new QPushButton(cnStr("停止运行"));
    setAttr(m_crashStop);
    connect(m_crashStop, &QPushButton::clicked, this, [=](){
            mainWindow()->slotRecognitionPackage(false);
            if(vm()->vmState() == vmIdle){return;}
            vm()->stopWork();
        });

    //系统参数设置
    m_systemPara = new QPushButton(cnStr("系统参数设置"));
    setAttr(m_systemPara);
    connect(m_systemPara, &QPushButton::clicked, this, [=](){
        QString pwStr = CInputDialog::getText(this->windowTitle(), cnStr("请输入密码"), QLineEdit::Password,QString()).trimmed();
        if (pwStr.size() == 0)
        {
            return;
        }

        if (pwStr == cnStr("123"))
        {
            CSetParaWindow setParaWindow;
            setParaWindow.setShowNode(TIGER_ParaDef::paraRootNode());
            bool isSave = setParaWindow.isSave();
            isSave ? sys()->save() : sys()->load();
            if(isSave)
            {
                QMessageBox::information(this, cnStr("提示"), cnStr("请重启软件加载参数！"));
            }
        }
        else
        {
            showToolTip(this, cnStr("密码错误"));
        }
        });

    //计数
    QLabel *pLabelAll = new QLabel;
    pLabelAll = getLed(cnStr("分拣总数："));
    QLabel *pLabelOk = new QLabel;
    pLabelOk = getLed(cnStr("合格个数："));
    QLabel *pLabelNg = new QLabel;
    pLabelNg = getLed(cnStr("不合格数："));
    m_allCount = new QLabel("0");
    setAttr(m_allCount);
    m_allCount->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_okCount = new QLabel("0");
    setAttr(m_okCount);
    m_okCount->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_ngCount = new QLabel("0");
    setAttr(m_ngCount);
    m_ngCount->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QGridLayout *pLayoutButton = new QGridLayout();
    pLayoutButton->addWidget(m_reset, 0, 0, 1, 3);
    pLayoutButton->addWidget(m_startRun, 0, 3, 1, 3);
    pLayoutButton->addWidget(m_crashStop, 1, 0, 1, 3);
    pLayoutButton->addWidget(m_systemPara, 1, 3, 1, 3);
    // pLayoutButton->addWidget(pLabelAll, 2, 0, 1, 1);
    // pLayoutButton->addWidget(m_allCount, 2, 1, 1, 1);
    // pLayoutButton->addWidget(pLabelOk, 2, 2, 1, 1);
    // pLayoutButton->addWidget(m_okCount, 2, 3, 1, 1);
    // pLayoutButton->addWidget(pLabelNg, 2, 4, 1, 1);
    // pLayoutButton->addWidget(m_ngCount, 2, 5, 1, 1);
    pLayoutButton->setMargin(0);
    pLayoutButton->setSizeConstraint(QGridLayout::SetMinimumSize);
    this->setMinimumSize(pLayoutButton->sizeHint());
    this->setLayout(pLayoutButton);
}

void ControlWidget::vmStateUpdate()
{
    switch (vm()->vmState())
    {
    case vmIdle:
        m_reset->setEnabled(true);
        m_startRun->setEnabled(true);
        m_crashStop->setEnabled(true);
        break;
    case vmReset:
        m_reset->setEnabled(false);
        m_startRun->setEnabled(false);
        m_crashStop->setEnabled(false);
        break;
    case vmAutoWork:
        m_reset->setEnabled(false);
        m_startRun->setEnabled(false);
        m_crashStop->setEnabled(true);
        break;
    default:
        m_reset->setEnabled(false);
        m_startRun->setEnabled(false);
        m_crashStop->setEnabled(false);
        break;
    }
}
