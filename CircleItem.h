#pragma once
#include "BaseShapeItem.h"
#include <QPointF>

class CircleItem : public BaseShapeItem {
    Q_OBJECT
public:
    explicit CircleItem(QPointF center = QPointF(0, 0), qreal radius = 50);

    // 必须实现的虚函数
    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    double calcArea() const override;

    // 属性访问
    QPointF center() const { return m_center; }
    qreal radius() const { return m_radius; }

signals:
    void parametersChanged(QPointF center, qreal radius, double area);

protected:
    // 事件处理
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change,
                        const QVariant &value) override;

private:
    enum DragMode { None, Move, Resize };

    QPointF m_center;    // 圆心（本地坐标系）
    qreal m_radius;      // 半径
    DragMode m_dragMode = None;
    QPointF m_clickOffset; // 点击偏移
};
