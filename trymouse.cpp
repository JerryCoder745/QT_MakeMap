#include "trymouse.h"
#include "qgraphicsitem.h"
#include <QDebug>
#include <QtMath>

trymouse::trymouse(QWidget *parent)
    : QGraphicsView{parent}
{}

void trymouse::mouseMoveEvent(QMouseEvent *event)
{
    scene()->clear();
    QPointF scenePos=mapToScene(event->pos());
    double x=scenePos.x(),y=scenePos.y();
    QGraphicsView::mouseMoveEvent(event);
    QGraphicsLineItem*xaxis=scene()->addLine(-1000-qFabs(x),0,1000+qFabs(x),0,QPen(Qt::green));
    QGraphicsLineItem*yaxis=scene()->addLine(0,-1000-qFabs(y),0,1000+qFabs(y),QPen(Qt::green));
    QGraphicsEllipseItem*ell=scene()->addEllipse(scenePos.x()-50,scenePos.y()-50,100,100);
    ell->setPen(QPen(Qt::white));
    ell->setBrush(QBrush(Qt::yellow));
}

void trymouse::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        setMouseTracking(1);
}

void trymouse::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        setMouseTracking(0);
}
