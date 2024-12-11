#include "src/system/basic.h"
#include <QObject>
#include <QImage>
#include <pylon/PylonIncludes.h>
#include <pylon/gige/BaslerGigEInstantCamera.h>
// CBaslerCamera 是多线程采图，所以他的回调函数需要加锁
class QMutex;
namespace TIGER_BaslerCamera
{
    class CImageEvent : public QObject, public Pylon::CImageEventHandler
    {
        Q_OBJECT
    public:
        CImageEvent();
        ~CImageEvent();

    signals:
        void sigGrabImage(QImage p_image);

    protected:
        virtual void OnImageGrabbed(Pylon::CInstantCamera &camera, const Pylon::CGrabResultPtr &ptrGrabResult);
        virtual void OnImagesSkipped(Pylon::CInstantCamera &camera, size_t countOfSkippedImages);

    protected:
        QMutex *m_pMutex;
    };
};