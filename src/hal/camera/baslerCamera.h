#pragma once
#include "src/system/sigleClass.h"
#include "src/system/basic.h"
#include <QObject>
#include <QImage>
namespace Pylon
{
    class CBaslerGigEInstantCamera;
};

namespace TIGER_BaslerCamera
{

    class CBaslerCamera : public QObject
    {
        Q_OBJECT
        SINGLETON(CBaslerCamera)

    public:
        void setIP(const QString &p_ip = "169.254.169.99");
        QString getIp() const;
        QString getLastError() const;
        QImage getLastImage() const;

        void setHeartbeatTimeout(const int &p_dMillisecond);
        int getHeartbeatTimeout() const;
        void setExposureTime(const double &p_time);
        double getExposureTime() const;
        bool isAcquire();
        bool connected();
        bool singleFrame();
        bool acquireChange();

    signals:
        void sigGrabImage(QImage p_image);
        void sigAcquireStateChanged();

    protected slots:
        void slotPrintError(const QString &p_msg);
        void slotGrabImage(QImage p_image);
        void slotDisconnected();

    protected:
        bool openCamera();
        bool closeCamera();

    protected:
        int m_heartbeatTimeout;
        double m_exposureTime;
        bool m_isConnected;
        QString m_error;
        QImage m_lastImage;
        QString m_ip;
        Pylon::CBaslerGigEInstantCamera *m_pGigECamera;
    };
};
TIGER_BaslerCamera::CBaslerCamera *baslerCamera();
