#pragma once
#include "../basic.h"
#include <Python.h>
#include <QImage>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <numpy/ndarrayobject.h>
using namespace cv;

namespace TIGER_ProcessTool
{
    class CPythonDL : public IProcessToolFun
    {
        Q_OBJECT
    public:
        CPythonDL();
        ~CPythonDL();

    signals:
        void sigDiscern(QImage p_image);
        void sigDiscernResult(QImage p_image, double p_x, double p_y, int p_num);

    public slots:
        int slotInitPy();
        bool slotDiscernPackage(QImage &p_image);
        bool slotDiscernPackageThread(QImage p_image);
        void slotEmitDiscernSig(QImage p_qImage);

    private:
        PyObject* m_pPyModule;
        PyObject* m_pPyRecognitionFun;
        PyObject* m_pPyRecognitionArg;
        PyObject* m_pPyRecognitionValue;
        PyObject* m_pPyRecognitionReturn;
        PyObject* m_pPyImageFun;
        PyObject* m_pPyImageReturn;
        PyArrayObject* m_pPyRetArray;

        bool m_pInitFlag;
        FileStorage m_pReadCameraPara;
		Mat m_pCameraMatrix;
		Mat m_pDistCoeffs;
    };
};