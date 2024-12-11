#include "src/system/basic.h"
#include <QObject>
#include <pylon/PylonIncludes.h>
#include <pylon/gige/BaslerGigEInstantCamera.h>
// CBaslerCamera 是多线程采图，所以他的回调函数需要加锁
class QMutex;
namespace TIGER_BaslerCamera
{
    enum CCameraState : uint8_t
    {
        ccsUnknown = 0,
        ccsOpened = 0b01,
        ccsGrabbing = 0b10,
    };

    class CConfigurationEvent : public QObject, public Pylon::CConfigurationEventHandler
    {
        Q_OBJECT
    public:
        CConfigurationEvent();
        ~CConfigurationEvent();

    signals:
        // void sigCameraStateChanged(int p_state);
        void sigGrabError(QString p_error);
        void sigCameraDisconnected();
        void sigCameraConnected();

    protected:
        // virtual void OnOpened(Pylon::CInstantCamera &camera);
        // virtual void OnGrabStarted(Pylon::CInstantCamera &camera);
        // virtual void OnGrabStopped(Pylon::CInstantCamera &camera);
        // virtual void OnClosed(Pylon::CInstantCamera &camera);
        virtual void OnAttached(Pylon::CInstantCamera &camera);
        virtual void OnCameraDeviceRemoved(Pylon::CInstantCamera &camera);
        virtual void OnGrabError(Pylon::CInstantCamera &camera, const char *errorMessage);

        // protected:
        //     void setState(CCameraState p_state, bool p_open);

    protected:
        // int m_state;
        QMutex *m_pMutex;
    };
};