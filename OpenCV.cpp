/*
 Original code by dmitry-rizshkov (http://appdeveloper.intel.com/en-us/blog/2011/03/09/augmented-reality-qt-part-1-capturing-video-webcam)
 Clarifications and optimizations (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
 All rights reserved.
*/

#include "OpenCV.h"
#include <QtDebug>

OpenCV::OpenCV(void)
    : mCamera(NULL)
    , mContours(NULL)
{
    // ...
}


OpenCV::~OpenCV()
{
    stopCapture();
}


bool OpenCV::startCapture(int desiredWidth, int desiredHeight, int fps, int cam)
{
    stopCapture();
    mCamera = cvCreateCameraCapture(cam);
    while (!isCapturing())
        /* wait ... */;
    cvSetCaptureProperty(mCamera, CV_CAP_PROP_FRAME_WIDTH, desiredWidth);
    cvSetCaptureProperty(mCamera, CV_CAP_PROP_FRAME_HEIGHT, desiredHeight);
    cvSetCaptureProperty(mCamera, CV_CAP_PROP_FPS, fps);
    if (getImageSize(desiredWidth, desiredHeight) == false)
        return false;
    mImage = QImage(desiredWidth, desiredHeight, QImage::Format_ARGB32);
    return true;
}


bool OpenCV::stopCapture(void)
{
    if (mCamera == NULL)
        return false;
    cvReleaseCapture(&mCamera);
    return true;
}


bool OpenCV::getImageSize(int& width, int& height) const
{
    if (!isCapturing())
        return false;
    const IplImage* cvimage = cvQueryFrame(mCamera);
    width = cvimage->width;
    height = cvimage->height;
    return true;
}


const QImage& OpenCV::getImage(void)
{
    if (!isCapturing())
        return mImage;
    const IplImage* iplImg = cvQueryFrame(mCamera);
    convertIplImageToQImage(iplImg, mImage);
    return mImage;
}


void OpenCV::convertIplImageToQImage(const IplImage* iplImg, QImage& image)
{
#pragma omp parallel for
    for (int y = 0; y < iplImg->height; ++y) {
        char* data = iplImg->imageData + y * iplImg->widthStep;
        uint *p = (uint*)image.scanLine(y);
        for (int x = 0; x < iplImg->width; ++x) {
            const int xoff = x * iplImg->nChannels;
            *p++ = qRgb(data[xoff+2], data[xoff+1], data[xoff]);
        }
    }
}
