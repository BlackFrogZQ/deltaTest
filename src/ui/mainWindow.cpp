#include "mainWindow.h"
#include "hal/camera/baslerCamera.h"
#include "hal/ZMotion/ZMotionParaDef.h"
#include "hal/ZMotionAction/iZMotionActionState.h"
#include "para/define/cameraDef.h"
#include "imageProcess/pythonDL/pythonDL.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QPainter>
#include <QMessageBox>
#include <QMutex>
#include <QThread>
#include <QTimer>
using namespace TIGER_ProcessTool;

CMainWindow *g_pMainWindow = nullptr;
CMainWindow *mainWindow()
{
    return g_pMainWindow;
};

CMainWindow::CMainWindow(QWidget *parent)
    : QWidget(parent), m_targetImage(NULL), m_startRecognition(false), m_targetNum(0), m_isCorrect(false)
{
    g_pMainWindow = this;

    m_pControlWidget = new ControlWidget;
    m_pLedStateWidget = new CPlcStateLed;
    initLayout();
    slotVideoImage(baslerCamera()->getExposureTime());
    connect(baslerCamera(), &TIGER_BaslerCamera::CBaslerCamera::sigGrabImage, this, &CMainWindow::slotUpdateImage);

    m_pPythonDL = new CPythonDL;
    m_pPythonDL->moveToThread(&m_visionThread);
    connect(m_pPythonDL, &CPythonDL::sigDiscern, m_pPythonDL, &CPythonDL::slotDiscernPackageThread);
    connect(m_pPythonDL, &CPythonDL::sigDiscernResult, this, [](QImage p_image, double p_x, double p_y, int p_num)
    {
        mainWindow()->slotRecognitionResult(p_image, p_x, p_y, p_num);
    });
    m_visionThread.start();
    m_pPythonDL->slotEmitDiscernSig(baslerCamera()->getLastImage());
}

CMainWindow::~CMainWindow()
{
    m_visionThread.quit();
    m_visionThread.wait();
    delPtr(m_pControlWidget);
    delPtr(m_pLedStateWidget);
}

void CMainWindow::printMsg(QString p_msg)
{
    m_pOutMsg->append(p_msg);
    m_pOutMsg->moveCursor(m_pOutMsg->textCursor().End);
}

void CMainWindow::initLayout()
{
    int imageW = 1626 / 2, imageH = 1236 / 2, controlWidgetH = 540 / 2;
    //图像窗口
    m_pImageLabel = new QLabel;
    m_pImageLabel->setFixedSize(imageW, imageH);
    m_pImageLabel->setStyleSheet(cStyleSheet);

    //输入输出窗口
    m_pOutMsg = new QTextBrowser;
    m_pOutMsg->setOpenLinks(false);
    m_pOutMsg->setOpenExternalLinks(false);
    m_pOutMsg->setStyleSheet(cStyleSheet);
    m_pOutMsg->document()->setMaximumBlockCount(300);

    //按钮及指示灯窗口
    QWidget *controlWidget = new QWidget;
    QVBoxLayout *pControLayout = new QVBoxLayout;
    pControLayout->addWidget(m_pControlWidget);
    pControLayout->addWidget(m_pLedStateWidget);
    pControLayout->setMargin(0);
    pControLayout->setSpacing(2);
    controlWidget->setLayout(pControLayout);
    controlWidget->setFixedSize(imageW/2, controlWidgetH);

    QWidget *bottomWidget = new QWidget;
    QHBoxLayout *pBottomLayout = new QHBoxLayout;
    pBottomLayout->addWidget(m_pOutMsg);
    pBottomLayout->addWidget(controlWidget);
    pBottomLayout->setMargin(0);
    pBottomLayout->setSpacing(2);
    bottomWidget->setLayout(pBottomLayout);

    QVBoxLayout *pLayoutMainWindow = new QVBoxLayout;
    pLayoutMainWindow->addWidget(m_pImageLabel);
    pLayoutMainWindow->addWidget(bottomWidget);
    pLayoutMainWindow->setMargin(10);
    pLayoutMainWindow->setSpacing(2);
    this->setFixedSize(imageW + 20, imageH + controlWidgetH + 22);
    this->setLayout(pLayoutMainWindow);
}

void CMainWindow::slotVideoImage(double p_exposureTime)
{
    baslerCamera()->setExposureTime(p_exposureTime);
    if (!(baslerCamera()->connected() && baslerCamera()->acquireChange()))
    {
        qInfo() << cnStr("相机打开失败：%1，地址：%2").arg(baslerCamera()->getLastError().arg(TIGER_CameraDef::cameraParas()->ip));
    }
    else
    {
        myInfo << cnStr("相机打开成功：%1").arg(TIGER_CameraDef::cameraParas()->ip);
    }
}

void CMainWindow::slotUpdateImage(QImage p_image)
{
    if(m_startRecognition)
    {
        QPainter imagepainter(&p_image);
        QRect targetRect(p_image.width()/2, p_image.height()/2, p_image.width()/2, p_image.height()/2);
        imagepainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        imagepainter.drawImage(targetRect, m_targetImage);
        imagepainter.end();
    }
    m_pImageLabel->setPixmap(QPixmap::fromImage(p_image).scaled(m_pImageLabel->size(), Qt::KeepAspectRatio));
}

void CMainWindow::slotRecognitionPackage(bool p_start)
{
    m_startRecognition = p_start;
    p_start == true ? m_pPythonDL->slotEmitDiscernSig(baslerCamera()->getLastImage()) : m_targetImage = QImage();
}

void CMainWindow::slotRecognitionResult(QImage p_image, double p_x, double p_y, int p_num)
{
    if(m_startRecognition)
    {
        if(m_isCorrect == true && m_targetNum == 0)
        {
            auto endTime = std::chrono::high_resolution_clock::now();
            workTypeData()->p_VisionEndTime = chrono::duration<double>(endTime.time_since_epoch()).count();
            double pVisionTime = workTypeData()->p_VisionEndTime - workTypeData()->p_VisionStartTime;
            double pWordX = round((p_x - pVisionTime * ZMotionParas()->conveyorBeltSpeed) * 1000 ) / 1000;
            double pWordY = round((p_y) * 1000 ) / 1000;
            myInfo << cnStr("识别时间：") << pVisionTime;

            m_targetX = pWordX;
            m_targetY = pWordY;
            m_targetNum = p_num;
        }
        if(workTypeData()->workType != camsGrabPlace || m_targetNum == 0 || abs(m_targetX) > 300)
        {
            m_targetNum = 0;
            m_isCorrect = false;
        }
        m_targetImage = p_image;
        m_pPythonDL->slotEmitDiscernSig(baslerCamera()->getLastImage());
    }
}