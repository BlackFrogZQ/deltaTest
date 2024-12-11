#include "opencvTool.h"
#include <vector>
#include <QtMath>
#include <QImage>
#include <QPainterPath>
#include <QPainter>
#include "system/basic.h"
namespace TIGER_OpencvTool
{
    using namespace cv;
    using namespace std;
    cv::RotatedRect getSmallestRectangle(const cv::Mat &p_matRegion)
    {
        cv::RotatedRect minRect;
        try
        {
            vector<Point> contourslist;
            // 联合所有轮廓
            {
                vector<Vec4i> hierarchy;
                vector<vector<Point>> contours;
                findContours(p_matRegion, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
                for (int i = 0; i < contours.size(); i++)
                {
                    vector<Point> singContours;
                    singContours = contours[i];
                    contourslist.insert(contourslist.end(), singContours.begin(), singContours.end());
                }
            }
            if (!contourslist.empty())
            {
                minRect = minAreaRect(contourslist);
            }
        }
        catch (const std::exception &e)
        {
            myDebug << ("getSmallestRectangle error:" + QString(e.what()));
        }
        return minRect;
    }
}

namespace TIGER_OpencvTool
{
    bool qImage2cvImage(const QImage &p_qimg, Mat &p_mat, int channel)
    {
        try
        {
            if (p_qimg.isNull())
            {
                myDebug << QObject::tr("图像为空无法转换");
                return false;
            }
            switch (channel)
            {
            case 1:
            {

                QImage temp = (p_qimg.format() != QImage::Format_Grayscale8 ? p_qimg.convertToFormat(QImage::Format_Grayscale8) : p_qimg);
                p_mat = Mat(temp.height(),
                            temp.width(),
                            CV_8UC1)
                            .clone();
                uchar *pDest = p_mat.data;
                for (int row = 0; row < p_mat.rows; row++, pDest += p_mat.step)
                {
                    uchar *pSrc = temp.scanLine(row);
                    memcpy(pDest, pSrc, p_mat.cols);
                }
            }
            break;
            case 3:
            default:
            {
                QImage temp = p_qimg.format() != QImage::Format_RGB888 ? p_qimg.convertToFormat(QImage::Format_RGB888) : p_qimg;
                p_mat = Mat(temp.height(),
                            temp.width(),
                            CV_8UC3,
                            (uchar *)temp.bits(),
                            temp.bytesPerLine())
                            .clone();
            }
            break;
            }
            return true;
        }
        catch (cv::Exception &e)
        {
            myDebug << ("qImageToCVImage error:" + QString(e.what()));
        }
        return false;
    }

    bool cvImage2qImage(QImage &p_qimg, const cv::Mat &p_mat)
    {
        switch (p_mat.type())
        {
        case CV_8UC1:
            return cvImage2qImageGray(p_qimg, p_mat);
        case CV_8UC3:
            return cvImage2qImageRGB(p_qimg, p_mat);
            break;
        case CV_8UC4:
            return cvImage2qImageRGBA(p_qimg, p_mat);
        default:
            break;
        }
        return false;
    }

    bool cvImage2qImageGray(QImage &p_qimg, const cv::Mat &p_matGray)
    {
        assert(p_matGray.type() == CV_8UC1);
        p_qimg = QImage(p_matGray.cols, p_matGray.rows, QImage::Format_Grayscale8);
        uchar *pSrc = p_matGray.data;
        for (int row = 0; row < p_matGray.rows; row++, pSrc += p_matGray.step)
        {
            uchar *pDest = p_qimg.scanLine(row);
            memcpy(pDest, pSrc, p_matGray.cols);
        }
        return !p_qimg.isNull();
    }

    bool cvImage2qImageRGB(QImage &p_qimg, const cv::Mat &p_matBGR)
    {
        assert(p_matBGR.type() == CV_8UC3);
        const uchar *pSrc = (const uchar *)p_matBGR.data;
        p_qimg = QImage(pSrc, p_matBGR.cols, p_matBGR.rows, p_matBGR.step, QImage::Format_RGB888).rgbSwapped();
        return !p_qimg.isNull();
    }

    bool cvImage2qImageRGBA(QImage &p_qimg, const cv::Mat &p_matBGRA)
    {
        assert(p_matBGRA.type() == CV_8UC4);
        const uchar *pSrc = (const uchar *)p_matBGRA.data;
        // Format_ARGB32 实际是bgra顺序
        p_qimg = QImage(pSrc, p_matBGRA.cols, p_matBGRA.rows, p_matBGRA.step, QImage::Format_ARGB32);
        p_qimg = p_qimg.convertToFormat(QImage::Format_RGBA8888);
        return !p_qimg.isNull();
    }

    bool qRegion2cvRegion(const QSize &p_regionSize, const QPainterPath &p_path, cv::Mat &p_hRegion)
    {
        if (p_regionSize.isEmpty())
        {
            // traceT(QObject::tr("无有效区域,无法转换"));
            return false;
        }
        QImage qimage = QImage(p_regionSize.width(), p_regionSize.height(), QImage::Format_Grayscale8);
        qimage.fill(Qt::black);
        QPainter painter(&qimage);
        painter.setPen(Qt::white);
        painter.setBrush(Qt::white);
        painter.translate(qimage.width() * 0.5, qimage.height() * 0.5);
        painter.drawPath(p_path);
        painter.end();
        if (!qImage2cvImage(qimage, p_hRegion, 1))
        {
            // myInfo << QObject::tr("图像转换失败，无法识别处理");
            return false;
        }
        return true;
    }

    bool qRegion2MinRegion(const QSize &p_regionSize, const QPainterPath &p_path, cv::Mat &p_hRegion, cv::Rect &p_boundingRect)
    {
        auto qRect = p_path.boundingRect();
        auto regionRect = QRectF(QPoint(0, 0), p_regionSize);
        qRect = qRect.translated(regionRect.center());
        auto intersect = regionRect.intersected(qRect);
        if (!intersect.isValid())
        {
            // myTrace(QObject::tr("无有效区域,无法转换"));
            return false;
        }

        int x = int(intersect.left());
        int y = int(intersect.top());
        double offsetX = intersect.left() - x;
        double offsetY = intersect.top() - y;
        int w = qCeil(intersect.width() + offsetX);
        int h = qCeil(intersect.height() + offsetY);
        QImage qimage = QImage(w, h, QImage::Format_Grayscale8);
        qimage.fill(Qt::black);
        QPainter painter(&qimage);
        painter.setPen(Qt::white);
        painter.setBrush(Qt::white);
        painter.translate(regionRect.center() - intersect.topLeft());
        painter.drawPath(p_path);
        painter.end();
        if (!qImage2cvImage(qimage, p_hRegion, 1))
        {
            // traceT(QObject::tr("图像转换失败，无法识别处理"));
            return false;
        }
        p_boundingRect = Rect(x, y, w, h);
        return true;
    }

    cv::Mat getSmallestRectangleRegion(const cv::Mat &p_matRegion, bool p_fill)
    {
        try
        {
            Mat region = Mat::zeros(p_matRegion.rows, p_matRegion.cols, CV_8UC1);
            RotatedRect minRect = getSmallestRectangle(p_matRegion);
            vector<Point2f> boxPts(4);
            minRect.points(boxPts.data());

            if (p_fill)
            {
                vector<vector<Point2f>> poly;
                poly.emplace_back(boxPts);
                fillPoly(region, poly, Scalar(255));
            }
            else
            {
                vector<Point> poly;
                poly.reserve(4);
                for (const auto &p : boxPts)
                {
                    poly.emplace_back(Point(p.x, p.y));
                }
                polylines(region, poly, true, Scalar(255));
            }
            return region;
        }
        catch (cv::Exception &e)
        {
            myDebug << QString("getSmallestRectangleRegion error:").arg(QString(e.what()));
        }
        return Mat();
    }

    cv::Mat getSmallestEllipseRegion(const cv::Mat &p_matRegion, bool p_fill)
    {
        try
        {
            Mat region = Mat::zeros(p_matRegion.rows, p_matRegion.cols, CV_8UC1);
            RotatedRect minRect = getSmallestRectangle(p_matRegion);
            const float cMinR = 5;
            auto &width = minRect.size.width;
            auto &height = minRect.size.height;
            width = qMax(width, cMinR) + cMinR;
            height = qMax(height, cMinR) + cMinR;
            ellipse(region, minRect, Scalar(255), p_fill ? -1 : 1);
            return region;
        }
        catch (cv::Exception &e)
        {
            myDebug << QString("getSmallestEllipseRegion error:").arg(QString(e.what()));
        }
        return Mat();
    }

    cv::Mat getCrossContours(const QSize &p_size, const double &p_rows, const double &p_cols)
    {
        try
        {
            // 十字叉
            Mat cross = Mat::zeros(p_size.height(), p_size.width(), CV_8UC1);
            int halfLong = 5;
            line(cross, Point(p_cols - halfLong, p_rows), Point(p_cols + halfLong, p_rows), Scalar(0xff));
            line(cross, Point(p_cols, p_rows - halfLong), Point(p_cols, p_rows + halfLong), Scalar(0xff));
            cross.at<uchar>(p_rows, p_cols) = 0xff;
            // 旋转
            Mat rotateMatrix;
            const double cAngle = 60;
            Size dstSize(cross.cols, cross.rows);
            Point2f center(p_cols, p_rows);
            rotateMatrix = getRotationMatrix2D(center, cAngle, 1);
            warpAffine(cross, cross, rotateMatrix, dstSize);
            return cross;
        }
        catch (cv::Exception &e)
        {
            myDebug << QString("getCrossContours error:").arg(QString(e.what()));
        }
        return Mat();
    }

    cv::Mat LocalAffineEstimate(const std::vector<Point2f>& shape1, const std::vector<Point2f>& shape2, bool fullAfine)
    {
        Mat out(2, 3, CV_32F);
        int siz = 2 * (int)shape1.size();

        if (fullAfine)
        {
            Mat matM(siz, 6, CV_32F);
            Mat matP(siz, 1, CV_32F);
            int contPt = 0;
            for (int ii = 0; ii<siz; ii++)
            {
                Mat therow = Mat::zeros(1, 6, CV_32F);
                if (ii % 2 == 0)
                {
                    therow.at<float>(0, 0) = shape1[contPt].x;
                    therow.at<float>(0, 1) = shape1[contPt].y;
                    therow.at<float>(0, 2) = 1;
                    therow.row(0).copyTo(matM.row(ii));
                    matP.at<float>(ii, 0) = shape2[contPt].x;
                }
                else
                {
                    therow.at<float>(0, 3) = shape1[contPt].x;
                    therow.at<float>(0, 4) = shape1[contPt].y;
                    therow.at<float>(0, 5) = 1;
                    therow.row(0).copyTo(matM.row(ii));
                    matP.at<float>(ii, 0) = shape2[contPt].y;
                    contPt++;
                }
            }
            Mat sol;
            solve(matM, matP, sol, DECOMP_SVD);
            out = sol.reshape(0, 2);
        }
        else
        {
            Mat matM(siz, 4, CV_32F);
            Mat matP(siz, 1, CV_32F);
            int contPt = 0;
            for (int ii = 0; ii<siz; ii++)
            {
                Mat therow = Mat::zeros(1, 4, CV_32F);
                if (ii % 2 == 0)
                {
                    therow.at<float>(0, 0) = shape1[contPt].x;
                    therow.at<float>(0, 1) = shape1[contPt].y;
                    therow.at<float>(0, 2) = 1;
                    therow.row(0).copyTo(matM.row(ii));
                    matP.at<float>(ii, 0) = shape2[contPt].x;
                }
                else
                {
                    therow.at<float>(0, 0) = -shape1[contPt].y;
                    therow.at<float>(0, 1) = shape1[contPt].x;
                    therow.at<float>(0, 3) = 1;
                    therow.row(0).copyTo(matM.row(ii));
                    matP.at<float>(ii, 0) = shape2[contPt].y;
                    contPt++;
                }
            }
            Mat sol;
            solve(matM, matP, sol, DECOMP_SVD);
            out.at<float>(0, 0) = sol.at<float>(0, 0);
            out.at<float>(0, 1) = sol.at<float>(1, 0);
            out.at<float>(0, 2) = sol.at<float>(2, 0);
            out.at<float>(1, 0) = -sol.at<float>(1, 0);
            out.at<float>(1, 1) = sol.at<float>(0, 0);
            out.at<float>(1, 2) = sol.at<float>(3, 0);
        }
        return out;
    }

    void handEyeCalibration(double &p_pixelX, double &p_pixelY)
    {
        //开始标定
        std::vector<Point2f> pointsCamera;
        std::vector<Point2f> pointsRobot;

        pointsCamera.push_back(Point2f(726.0f, 582.0f));
        pointsCamera.push_back(Point2f(636.0f, 580.0f));
        pointsCamera.push_back(Point2f(548.0f, 578.0f));
        pointsCamera.push_back(Point2f(726.0f, 494.0f));
        pointsCamera.push_back(Point2f(638.0f, 492.0f));
        pointsCamera.push_back(Point2f(548.0f, 490.0f));
        pointsCamera.push_back(Point2f(728.0f, 404.0f));
        pointsCamera.push_back(Point2f(638.0f, 404.0f));
        pointsCamera.push_back(Point2f(550.0f, 402.0f));
        pointsCamera.push_back(Point2f(308.0f, 582.0f));
        pointsCamera.push_back(Point2f(220.0f, 582.0f));
        pointsCamera.push_back(Point2f(132.0f, 582.0f));
        pointsCamera.push_back(Point2f(306.0f, 492.0f));
        pointsCamera.push_back(Point2f(220.0f, 494.0f));
        pointsCamera.push_back(Point2f(130.0f, 494.0f));
        pointsCamera.push_back(Point2f(306.0f, 404.0f));
        pointsCamera.push_back(Point2f(218.0f, 406.0f));
        pointsCamera.push_back(Point2f(130.0f, 406.0f));

        pointsRobot.push_back(Point2f(-293.0f, -22.0f));
        pointsRobot.push_back(Point2f(-244.0f, -21.0f));
        pointsRobot.push_back(Point2f(-194.0f, -21.0f));
        pointsRobot.push_back(Point2f(-292.0f, 27.0f));
        pointsRobot.push_back(Point2f(-242.0f, 27.0f));
        pointsRobot.push_back(Point2f(-193.0f, 27.0f));
        pointsRobot.push_back(Point2f(-292.0f, 77.0f));
        pointsRobot.push_back(Point2f(-242.0f, 77.0f));
        pointsRobot.push_back(Point2f(-192.0f, 77.0f));
        pointsRobot.push_back(Point2f(-54.0f, -24.0f));
        pointsRobot.push_back(Point2f(-5.0f, -24.0f));
        pointsRobot.push_back(Point2f(45.0f, -24.0f));
        pointsRobot.push_back(Point2f(-53.0f, 26.0f));
        pointsRobot.push_back(Point2f(-3.0f, 26.0f));
        pointsRobot.push_back(Point2f(46.0f, 26.0f));
        pointsRobot.push_back(Point2f(-54.0f, 76.0f));
        pointsRobot.push_back(Point2f(-3.0f, 75.0f));
        pointsRobot.push_back(Point2f(46.0f, 75.0f));

        //变换成2*3矩阵
        Mat affine;
        estimateRigidTransform(pointsCamera, pointsRobot, true).convertTo(affine, CV_32F);
        if (affine.empty())
        {
            affine = LocalAffineEstimate(pointsCamera, pointsRobot, true);
        }
        float A, B, C, D, E, F;
        A = affine.at<float>(0, 0);
        B = affine.at<float>(0, 1);
        C = affine.at<float>(0, 2);
        D = affine.at<float>(1, 0);
        E = affine.at<float>(1, 1);
        F = affine.at<float>(1, 2);
        {
            Point2d cameraPoints(p_pixelX, p_pixelY);
            p_pixelX = (A * cameraPoints.x) + (B * cameraPoints.y) + C;
            p_pixelY = (D * cameraPoints.x) + (E * cameraPoints.y) + F;
        }
    }
}