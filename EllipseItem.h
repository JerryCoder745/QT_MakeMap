#ifndef ELLIPSEITEM_H
#define ELLIPSEITEM_H
#pragma once
#include "BaseShapeItem.h"
#include <QPointF>

class EllipseItem : public BaseShapeItem {
    Q_OBJECT
public:
    explicit EllipseItem(QPointF center = QPointF(0, 0),
                         qreal rx = 50,
                         qreal ry = 30);

    // 必须实现的虚函数
    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    double calcArea() const override;

    // 属性访问
    QPointF center() const { return m_center; }
    qreal horizontalRadius() const { return m_rx; }
    qreal verticalRadius() const { return m_ry; }

signals:
    void parametersChanged(QPointF center, qreal rx, qreal ry, double area);

protected:
    // 事件处理
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change,
                        const QVariant &value) override;

private:
    enum DragMode { None, Move, AdjustX, AdjustY };

    QPointF m_center;    // 椭圆中心（本地坐标系）
    qreal m_rx;          // 水平半径
    qreal m_ry;          // 垂直半径
    DragMode m_dragMode = None;
    QPointF m_clickOffset;

    void updateControlPoints();
    QPointF m_xCtrlPoint; // 水平半径控制点
    QPointF m_yCtrlPoint; // 垂直半径控制点
};
#endif // ELLIPSEITEM_H
