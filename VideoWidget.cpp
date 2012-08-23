/*
 Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
 All rights reserved.
*/


#include "VideoWidget.h"
#include <qmath.h>
#include <QTime>
#include <QPainter>
#include <QtDebug>
#include <QPixmap>


VideoWidget::VideoWidget(QWidget* parent)
    : QWidget(parent)
    , mCameraUpdateTimerId(0)
    , mPointUnderCursor(-1)
{
    setMinimumSize(320, 240);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    resetTransform();
    startCapture();
    setCursor(Qt::CrossCursor);
    setMouseTracking(true);
    setFocus(Qt::MouseFocusReason);
}


VideoWidget::~VideoWidget()
{
    stopCapture();
}


void VideoWidget::startCapture()
{
    const int fps = 30;
    mOpenCV.startCapture(1920, 1080, fps);
    mCameraUpdateTimerId = startTimer(1000/fps);
}


void VideoWidget::stopCapture()
{
    mOpenCV.stopCapture();
    killTimer(mCameraUpdateTimerId);
}


void VideoWidget::resetTransform(void)
{
    mPoints.clear();
    mTransform.reset();
    mPolygonSize = QSize();
}


void VideoWidget::calcTransform(void)
{
    mSelectedPolygon = mPoints;
    mWindowPolygon.clear();
    mWindowPolygon << QPointF(0, 0) << QPointF(mImage.width(), 0) << QPointF(mImage.width(), mImage.height()) << QPointF(0, mImage.height());
    QTransform::quadToQuad(mSelectedPolygon, mWindowPolygon, mTransform);
}


void VideoWidget::setTransform(const QTransform& transform)
{
    mPoints.clear();
    mTransform = transform;
}


QImage VideoWidget::currentImage(void) const
{
    QImage img(mImage.size(), QImage::Format_ARGB32);
    QPainter painter(&img);
    painter.setTransform(mTransform);
    painter.drawImage(img.rect(), mImage);
    return img;
}


void VideoWidget::paintEvent(QPaintEvent*)
{
    if (mImage.isNull())
        return;
    const qreal sx = (qreal)width()  / (qreal)mImage.width();
    const qreal sy = (qreal)height() / (qreal)mImage.height();
    QPainter painter(this);
    painter.drawImage(rect(), currentImage());
    painter.setRenderHint(QPainter::Antialiasing);
    for (int i = 0; i < mPoints.size(); ++i) {
        const qreal r = 2 + 3 * qAbs(qCos(M_PI * 1e-3 * QTime::currentTime().msec()));
        if (i == mPointUnderCursor) {
            painter.setPen(QColor(255, 128, 255, 200));
            painter.setBrush(QColor(0, 255, 0, 128));
        }
        else {
            painter.setPen(QColor(128, 255, 128, 200));
            painter.setBrush(QColor(0, 0, 255, 128));
        }
        const QPointF p(mPoints.at(i).x() * sx, mPoints.at(i).y() * sy);
        painter.drawEllipse(p, r, r);
    }
}


void VideoWidget::timerEvent(QTimerEvent*)
{
    if (mOpenCV.isCapturing()) {
        mImage = mOpenCV.getImage();
        update();
    }
}


void VideoWidget::mousePressEvent(QMouseEvent* e)
{
    switch (e->button()) {
    case Qt::LeftButton:
        if (mPoints.size() < 4) {
            const qreal sx = (qreal)mImage.width()  / (qreal)width();
            const qreal sy = (qreal)mImage.height() / (qreal)height();
            mPoints << QPointF(sx * e->x(), sy * e->y());
        }
        if (mPoints.size() == 4) {
            mPolygonSize = mPoints.boundingRect().toRect().size();
            calcTransform();
            mPoints.clear();
        }
        break;
    case Qt::RightButton:
        if (mPointUnderCursor >= 0) {
            mPoints.resize(mPointUnderCursor);
            mPointUnderCursor = -1;
        }
        break;
    default:
        break;
    }
    e->accept();
}


void VideoWidget::mouseMoveEvent(QMouseEvent* e)
{
    mPointUnderCursor = -1;
    for (int i = 0; i < mPoints.size(); ++i) {
        const qreal sx = (qreal)width()  / (qreal)mImage.width();
        const qreal sy = (qreal)height() / (qreal)mImage.height();
        const QPointF& p = mPoints.at(i);
        const qreal dx = e->x() - sx * p.x();
        const qreal dy = e->y() - sy * p.y();
        const qreal distSquared = dx * dx + dy * dy;
        if (distSquared < 50.0) {
            mPointUnderCursor = i;
            break;
        }
    }
}
