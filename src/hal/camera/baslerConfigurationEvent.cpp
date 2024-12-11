#include <QMutex>
#include "baslerConfigurationEvent.h"
using namespace Pylon;
namespace TIGER_BaslerCamera
{
    CConfigurationEvent::CConfigurationEvent()
        : m_pMutex(nullptr)
    //   m_state(ccsUnknown)
    {
        m_pMutex = new QMutex;
    }

    CConfigurationEvent::~CConfigurationEvent()
    {
        delPtr(m_pMutex);
    }

    // void CConfigurationEvent::OnOpened(CInstantCamera &camera)
    // {
    //     setState(ccsOpened, true);
    // }

    // void CConfigurationEvent::OnGrabStarted(CInstantCamera &camera)
    // {
    //     setState(ccsGrabbing, true);
    // }

    // void CConfigurationEvent::OnGrabStopped(CInstantCamera &camera)
    // {
    //     setState(ccsGrabbing, false);
    // }

    // void CConfigurationEvent::OnClosed(CInstantCamera &camera)
    // {
    //     setState(ccsOpened, false);
    // }

    void CConfigurationEvent::OnGrabError(CInstantCamera &camera, const char *errorMessage)
    {
        m_pMutex->lock();
        emit sigGrabError(QString("%1 Grab Error:%2").arg(QString(camera.GetDeviceInfo().GetAddress())).arg(errorMessage));
        m_pMutex->unlock();
    }

    void CConfigurationEvent::OnCameraDeviceRemoved(Pylon::CInstantCamera &camera)
    {
        m_pMutex->lock();
        // qInfo() << cnStr("camera(%2) %1 断开连接").arg(QString(camera.GetDeviceInfo().GetAddress())).arg(QString(camera.GetDeviceInfo().GetModelName()));
        m_pMutex->unlock();
        emit sigCameraDisconnected();
    }

    void CConfigurationEvent::OnAttached(Pylon::CInstantCamera &camera)
    {
        m_pMutex->lock();
        // qInfo() << cnStr("camera(%2) %1 已连接").arg(QString(camera.GetDeviceInfo().GetAddress())).arg(QString(camera.GetDeviceInfo().GetModelName()));
        m_pMutex->unlock();
        emit sigCameraConnected();
    }

    // void CConfigurationEvent::setState(CCameraState p_state, bool p_open)
    // {
    //     m_pMutex->lock();
    //     m_state = p_open ? (m_state | p_state) : (m_state & (~p_state));
    //     emit sigCameraStateChanged(m_state);
    //     m_pMutex->unlock();
    // }
};