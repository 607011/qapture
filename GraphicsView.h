#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QPainter>
#include <QRectF>

class CGraphicsView : public QGraphicsView
{
public:
    CGraphicsView();

protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // GRAPHICSVIEW_H
