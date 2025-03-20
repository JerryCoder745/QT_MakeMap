// QuadrilateralItem.h
#pragma once
#include "BaseShapeItem.h"
#include <QPolygonF>

class QuadrilateralItem : public BaseShapeItem {
    Q_OBJECT
public:
    explicit QuadrilateralItem(const QPolygonF &points);

    // 必须实现的虚函数
    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    double calcArea() const override;

    // 顶点操作接口
    void updatePoint(int index, const QPointF& newPos);
    void emitUpdateSignal();
signals:
    void parametersChanged(const QPolygonF &scenePoints, double area); // 形状变化信号

protected:
    // 事件处理
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change,
                        const QVariant &value) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    // 顶点操作相关
    int findControlPoint(const QPointF& pos) const;
    void updateBoundingBox();

    QPolygonF m_points;         // 四个顶点（本地坐标系）
    int m_activePoint = -1;      // 当前拖动的顶点索引
    bool m_draggingAll = false;  // 是否整体拖动
    QPointF m_dragOffset;        // 拖拽偏移量
};
