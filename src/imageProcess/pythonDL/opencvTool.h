#pragma once
#include "system/myMath.h"
#include <opencv2/opencv.hpp>
#include "../basic.h"

class QSize;
class QImage;
class QPainterPath;
namespace TIGER_OpencvTool
{
    const int cDefHandleValue = -1;
    // 3通道图
    bool qImage2cvImage(const QImage &p_qimg, cv::Mat &p_mat,int channel = 3);
    bool cvImage2qImage(QImage &p_qimg, const cv::Mat &p_mat);
    bool cvImage2qImageGray(QImage &p_qimg, const cv::Mat &p_matGray);
    bool cvImage2qImageRGB(QImage &p_qimg, const cv::Mat &p_matBGR);
    bool cvImage2qImageRGBA(QImage &p_qimg, const cv::Mat &p_matBGRA);
    bool qRegion2cvRegion(const QSize &p_regionSize, const QPainterPath &p_path, cv::Mat &p_hRegion);
    bool qRegion2MinRegion(const QSize &p_regionSize, const QPainterPath &p_path, cv::Mat &p_hRegion, cv::Rect& p_boundingRect);
    cv::RotatedRect getSmallestRectangle(const cv::Mat &p_matRegion);
    void handEyeCalibration(double &p_pixelX, double &p_pixelY);

    cv::Mat getSmallestRectangleRegion(const cv::Mat &p_matRegion,bool p_fill = false);
    cv::Mat getSmallestEllipseRegion(const cv::Mat &p_matRegion,bool p_fill = false);
    cv::Mat getCrossContours(const QSize &p_size, const double &p_rows, const double &p_cols);
    bool twoLineIntersection(const CPoint2d &p_line1Start, const CPoint2d &p_line1End, const CPoint2d &p_line2Start, const CPoint2d &p_line2End, const QSize &p_size, CPoint2d &p_intersectionPoint);
};