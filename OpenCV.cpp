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
    // IplImage* edges = cvCreateImage(cvSize(iplImg->width, iplImg->height), IPL_DEPTH_8U, 1);
    // CvMemStorage* storage = cvCreateMemStorage(0);
    // cvAdaptiveThreshold(iplImg, edges, 255);
    // cvFindContours(edges, storage, &mContours, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
    // cvDrawContours(iplImg, mContours, CV_RGB(255,0,0), CV_RGB(0,255,0), 7, 3, CV_AA, cvPoint(0,0));
    // cvReleaseMemStorage(&storage);
    // cvReleaseImage(&edges);
    convertIplImageToQImage(iplImg, mImage);
    return mImage;
}


void OpenCV::convertIplImageToQImage(const IplImage* iplImg, QImage& image)
{
    const int w = iplImg->width;
    const int h = iplImg->height;
#pragma omp parallel for
    for (int y = 0; y < h; ++y) {
        char* data = iplImg->imageData + y * iplImg->widthStep;
        uint *p = (uint*)image.scanLine(y);
        for (int x = 0; x < w; ++x) {
            const int xoff = x * iplImg->nChannels;
            *p++ = qRgb(data[xoff+2], data[xoff+1], data[xoff]);
        }
    }
}
