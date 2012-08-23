#ifndef __OPENCVSCENE_H_
#define __OPENCVSCENE_H_

#include <QImage>

#pragma warning(disable: 4996)

#include <opencv/cv.h>
#include <opencv/highgui.h>

class OpenCV
{
public:
        OpenCV(void);
        ~OpenCV();

public:
        bool startCapture(int width, int height, int fps, int cam = 0);
        bool stopCapture(void);
        bool isCapturing() const { return (mCamera != NULL); }
        bool getImageSize(int& width, int& height) const;
        const QImage& getImage(void);

private:
        void convertIplImageToQImage(const IplImage* iplImg, QImage& image);
        CvCapture* mCamera;
        QImage mImage;
        CvSeq* mContours;
};

#endif // __OPENCVSCENE_H_
