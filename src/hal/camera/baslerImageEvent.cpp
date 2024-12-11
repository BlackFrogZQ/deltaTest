#include <QMutex>
#include "baslerImageEvent.h"
using namespace Pylon;
namespace TIGER_BaslerCamera
{
    CImageEvent::CImageEvent() : m_pMutex(nullptr)
    {
        m_pMutex = new QMutex;
    }
    CImageEvent::~CImageEvent()
    {
        delPtr(m_pMutex);
    }

    void CImageEvent::OnImageGrabbed(CInstantCamera &camera, const CGrabResultPtr &ptrGrabResult)
    {
        m_pMutex->lock();
        if (ptrGrabResult == NULL || !(ptrGrabResult->GrabSucceeded() && ptrGrabResult.IsValid()))
        {
            m_pMutex->unlock();
            return;
        }
        CPylonImage bitmap;
        bitmap.CopyImage(ptrGrabResult);
        // bitmap.GetPixelType()
        assert(bitmap.IsSupportedPixelType(PixelType_RGB8packed));

        CImageFormatConverter converter;
        converter.OutputPixelFormat = PixelType_RGB8packed;
        converter.OutputBitAlignment = OutputBitAlignment_MsbAligned;

        CPylonImage img;
        converter.Convert(img, bitmap);

        int w = img.GetWidth();
        int h = img.GetHeight();
        QImage image(w, h, QImage::Format_RGB888);
        uchar *rgb = image.bits();
        uchar *imgd = (uchar *)img.GetBuffer();
        for (int i = 0; i < h; i++)
        {
            uchar *rgb = image.scanLine(i);
            memcpy(rgb, imgd, 3 * w);
            imgd += 3 * w;
        }
        if (!image.isNull())
        {
            emit sigGrabImage(image);
        }
        m_pMutex->unlock();
    }

    void CImageEvent::OnImagesSkipped(CInstantCamera &camera, size_t countOfSkippedImages)
    {
        // qDebug() <<  "CImageEventHandler::OnImagesSkipped called.";
    }
};