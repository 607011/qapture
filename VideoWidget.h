/*
 Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
 All rights reserved.
*/

#ifndef __VIDEOWIDGET_H_
#define __VIDEOWIDGET_H_

#include <QWidget>
#include <QImage>
#include <QMouseEvent>
#include <QPoint>
#include <QVector>
#include <QTransform>

#include "OpenCV.h"


class VideoWidget : public QWidget
{
    Q_OBJECT

public:
    VideoWidget(QWidget* parent = NULL);
    ~VideoWidget();

public:
    void startCapture(void);
    void stopCapture(void);
    void resetTransform(void);
    QImage currentImage(void) const;
    const QTransform& transform(void) const { return mTransform; }
    void setTransform(const QTransform& transform);
    const QSize& polygonSize(void) const { return mPolygonSize; }

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);

private:
    void timerEvent(QTimerEvent* timer);
    void calcTransform(void);

private:
    OpenCV mOpenCV;
    int mCameraUpdateTimerId;
    QImage mImage;
    QPolygonF mPoints;
    QPolygonF mSelectedPolygon;
    QPolygonF mWindowPolygon;
    QTransform mTransform;
    int mPointUnderCursor;
    QSize mPolygonSize;
};

#endif // __VIDEOWIDGET_H_
