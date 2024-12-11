#include "system/systemService.h"
#include "logSystem/logDef.h"
#include "ui/basic.h"
#include "para/para.h"
#include "hal/camera/baslerCamera.h"
#include "para/define/cameraDef.h"
#include "ui/mainWindow.h"
#include "hal/vm.h"
#include <QDateTime>
#include <QProcess>
#include <QApplication>

void logOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
class CVmControl : public ISystemService
{
public:
    CVmControl();
    void init();
    virtual void printMsg(QString p_msg);
    virtual void updateUiImage(const QImage &p_image);
    virtual void setOnlyImage(bool p_isOnlyImage = true);
    virtual void restartSys() const;

    void load();
    void save();

protected:
    ~CVmControl();

protected:
    QStringList m_msgBuffer;
    CVM* m_pVm;
};

CVmControl::CVmControl()
    : ISystemService()
{
}

CVmControl::~CVmControl()
{
    delPtr(m_pVm);
    qWarning() << cnStr("程序结束");
}

void CVmControl::init()
{
    m_pVm = new CVM;
    qWarning() << cnStr("程序开始");
}

void CVmControl::printMsg(QString p_msg)
{
    CLogDef::writeLog(CLogDef::getInfoLogName(), QDateTime::currentDateTime().toString("[hh.mm.ss]:%1").arg(p_msg));
    p_msg = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss]:%1").arg(p_msg);
    m_msgBuffer << p_msg;

    if (mainWindow() != nullptr)
    {
        for (const auto &msg : m_msgBuffer)
        {
            mainWindow()->printMsg(msg);
        }
        m_msgBuffer.clear();
    }
}

void CVmControl::updateUiImage(const QImage &p_image)
{
    if (mainWindow() != nullptr)
    {
        mainWindow()->slotUpdateImage(p_image);
    }
}

void CVmControl::setOnlyImage(bool p_isOnlyImage)
{
    if (mainWindow() != nullptr)
    {
        // mainWindow()->slotIsOnlyShowImage(p_isOnlyImage);
    }
}

void CVmControl::restartSys() const
{
    qApp->exit();
    QProcess::startDetached(qApp->applicationFilePath(), QStringList());
}

void CVmControl::load()
{
    paraService()->load();
}

void CVmControl::save()
{
    paraService()->save();
}

ISystemService *g_sysService = nullptr;
ISystemService *sys()
{
    return g_sysService;
};

void initSystemService()
{
    qInstallMessageHandler(logOutput);

    g_sysService = new CVmControl;
    sys()->load();
    ((CVmControl *)g_sysService)->init();

    baslerCamera()->setIP(TIGER_CameraDef::cameraParas()->ip);
    baslerCamera()->setExposureTime(TIGER_CameraDef::cameraParas()->exposureTime);
}

void closeSystemService()
{
    sys()->save();
    delete g_sysService;
    g_sysService = nullptr;
}
