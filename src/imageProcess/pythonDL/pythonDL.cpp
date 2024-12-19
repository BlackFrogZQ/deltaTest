#include "pythonDL.h"
#include "opencvTool.h"
#include "system/basic.h"
#include "ui/mainWindow.h"
#include "hal/ZMotionAction/iZMotionActionState.h"
#include <QImage>
#include <QThread>
#include <QTime>
#include <iostream>
using namespace std;
using namespace TIGER_OpencvTool;

namespace TIGER_ProcessTool
{
    CPythonDL::CPythonDL(): m_pInitFlag(true)
    {
        m_pReadCameraPara = FileStorage("./python/calibrationResults.yml", FileStorage::READ);
        if (m_pReadCameraPara.isOpened())
        {
            myInfo << cnStr("相机畸变矫正文件打开成功！");
            m_pCameraMatrix = Mat(3, 3, CV_32FC1);
            m_pReadCameraPara["cameraMatrix"] >> m_pCameraMatrix;
            m_pReadCameraPara["distCoeffs"] >> m_pDistCoeffs;
        }
        else
        {
            myInfo << cnStr("相机畸变矫正文件打开失败！");
        }
    }

    CPythonDL::~CPythonDL()
    {
        Py_Finalize();
        delPyPtr(m_pPyModule);
        delPyPtr(m_pPyRecognitionFun);
        delPyPtr(m_pPyRecognitionArg);
        delPyPtr(m_pPyRecognitionValue);
        delPyPtr(m_pPyRecognitionReturn);
        delPyPtr(m_pPyImageFun);
        delPyPtr(m_pPyImageReturn);
        delPyPtr(m_pPyRetArray);
        m_pReadCameraPara.release();
    }

    void CPythonDL::slotEmitDiscernSig(QImage p_qImage)
    {
        if(m_pReadCameraPara.isOpened() && !p_qImage.isNull() && workTypeData()->workType == camsGrabPlace && mainWindow()->m_isCorrect == false)
        {
            auto startTime = std::chrono::high_resolution_clock::now();
            workTypeData()->p_VisionStartTime = chrono::duration<double>(startTime.time_since_epoch()).count();
            Mat cvImage;
            qImage2cvImage(p_qImage, cvImage, p_qImage.format() == QImage::Format_RGB888 ? 3 : 1);
            Mat distortion = cvImage.clone();
            cv::undistort(cvImage, distortion, m_pCameraMatrix, m_pDistCoeffs);
            cvImage2qImage(p_qImage, distortion);
            mainWindow()->m_isCorrect = true;
        }
        emit sigDiscern(p_qImage);
    }

    int CPythonDL::slotInitPy()
    {
        Py_Initialize();
        if (!Py_IsInitialized())
        {
            myInfo << "python init fail";
            return 0;
        }
        import_array();
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('./python/script')");

        m_pPyModule = PyImport_ImportModule("detectObb");
        if (m_pPyModule == NULL)
        {
            myInfo << "not found module: detectObb";
            return 0;
        }

        m_pPyRecognitionFun = PyObject_GetAttrString(m_pPyModule, "packageRecognition");
        if (!m_pPyRecognitionFun || !PyCallable_Check(m_pPyRecognitionFun))
        {
            myInfo << "not found function: packageRecognition";
            return 0;
        }
        m_pPyRecognitionArg = PyTuple_New(1);

        m_pPyImageFun = PyObject_GetAttrString(m_pPyModule, "getResultImages");
        if (!m_pPyImageFun || !PyCallable_Check(m_pPyImageFun))
        {
            myInfo << "not found function: getResultImages";
            return 0;
        }
        return 0;
    }

    bool CPythonDL::slotDiscernPackage(QImage &p_image)
    {
        QTime pTime;
        pTime.start();

        Mat pImage;
        qImage2cvImage(p_image, pImage, 3);

        // 获得坐标
        m_pPyRecognitionValue = nullptr;
        npy_intp pDims[3] = {pImage.rows, pImage.cols, pImage.channels()};
        m_pPyRecognitionValue = (PyObject*)PyArray_SimpleNewFromData(3, pDims, NPY_UBYTE, (unsigned char *)pImage.data);
        PyTuple_SetItem(m_pPyRecognitionArg, 0, m_pPyRecognitionValue);
        myInfo << cnStr("用时：%1").arg(pTime.restart());

        int pPointX = -1, pPointY = -1, pNum = 0;
        m_pPyRecognitionReturn = nullptr;
        m_pPyRecognitionReturn = PyObject_CallObject(m_pPyRecognitionFun, m_pPyRecognitionArg);
        myInfo << cnStr("用时：%1").arg(pTime.restart());
        PyArg_ParseTuple(m_pPyRecognitionReturn, "i|i|i", &pPointX, &pPointY, &pNum);
        myInfo << cnStr("图像坐标:(%1，%2)").arg(pPointX).arg(pPointY);
        myInfo << cnStr("用时：%1").arg(pTime.restart());

        // 得到结果图像
        m_pPyImageReturn = nullptr;
        m_pPyImageReturn = PyObject_CallObject(m_pPyImageFun, nullptr);
        PyArrayObject* pRetArray;
        PyArray_OutputConverter(m_pPyImageReturn, &pRetArray); // 将Python对象转换为一个NumPy数组
        npy_intp* shape = PyArray_SHAPE(pRetArray);            // 返回一个指向NumPy数组形状（即每个维度大小）的指针
        Mat imgReturn(shape[0], shape[1], CV_8UC3, PyArray_DATA(pRetArray));
        cvImage2qImage(p_image, imgReturn);

        myInfo << cnStr("用时：%1").arg(pTime.elapsed());
        return pNum;
    }

    bool CPythonDL::slotDiscernPackageThread(QImage p_image)
    {
        if(m_pInitFlag)
        {
            Py_Initialize();
            if (!Py_IsInitialized())
            {
                myInfo << "python init fail";
                return false;
            }
            import_array();
            PyRun_SimpleString("import sys");
            PyRun_SimpleString("sys.path.append('./python/script')");

            m_pPyModule = PyImport_ImportModule("detectObb");
            if (m_pPyModule == NULL)
            {
                PyErr_Print();
                myInfo << "not found module: detectObb";
                return false;
            }

            m_pPyRecognitionFun = PyObject_GetAttrString(m_pPyModule, "packageRecognition");
            if (!m_pPyRecognitionFun || !PyCallable_Check(m_pPyRecognitionFun))
            {
                myInfo << "not found function: packageRecognition";
                return false;
            }
            m_pPyRecognitionArg = PyTuple_New(1);

            m_pPyImageFun = PyObject_GetAttrString(m_pPyModule, "getResultImages");
            if (!m_pPyImageFun || !PyCallable_Check(m_pPyImageFun))
            {
                myInfo << "not found function: getResultImages";
                return false;
            }
            m_pInitFlag = false;
            return true;
        }

        Mat pImage;
        qImage2cvImage(p_image, pImage, p_image.format() == QImage::Format_RGB888 ? 3 : 1);

        // 获得坐标
        m_pPyRecognitionValue = NULL;
        npy_intp pDims[3] = {pImage.rows, pImage.cols, pImage.channels()};
        m_pPyRecognitionValue = (PyObject*)PyArray_SimpleNewFromData(3, pDims, NPY_UBYTE, (unsigned char *)pImage.data);
        PyTuple_SetItem(m_pPyRecognitionArg, 0, m_pPyRecognitionValue);

        double pPointX, pPointY;
        int pNum = 0;
        m_pPyRecognitionReturn = NULL;
        m_pPyRecognitionReturn = PyObject_CallObject(m_pPyRecognitionFun, m_pPyRecognitionArg);
        PyArg_ParseTuple(m_pPyRecognitionReturn, "d|d|i", &pPointX, &pPointY, &pNum);
        handEyeCalibration(pPointX, pPointY);

        // 得到结果图像
        PyObject* pPyImageReturn = NULL;
        m_pPyRetArray = NULL;
        pPyImageReturn = PyObject_CallObject(m_pPyImageFun, nullptr);
        PyArray_OutputConverter(pPyImageReturn, &m_pPyRetArray);
        npy_intp* shape = PyArray_SHAPE(m_pPyRetArray);
        Mat imgReturn(shape[0], shape[1], CV_8UC3, PyArray_DATA(m_pPyRetArray));
        cvImage2qImage(p_image, imgReturn);
        Py_XDECREF(pPyImageReturn);

        emit sigDiscernResult(p_image, pPointX, pPointY, pNum);
    }
};
