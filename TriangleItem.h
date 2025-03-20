#pragma once
#include "BaseShapeItem.h"
#include <QPolygonF>

class TriangleItem : public BaseShapeItem {
    Q_OBJECT
public:
    explicit TriangleItem(const QPolygonF &points = defaultTriangle());

    // 必须实现的虚函数
    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    double calcArea() const override;

    // 属性访问
    QPolygonF points() const { return m_points; }
    static QPolygonF defaultTriangle();

signals:
    void parametersChanged(const QPolygonF &scenePoints, double area);

protected:
    // 事件处理
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change,
                        const QVariant &value) override;

private:
    enum DragMode { None, Move, VertexDrag };

    QPolygonF m_points;      // 本地坐标系顶点（3个点）
    int m_activeVertex = -1; // 当前拖动的顶点索引
    DragMode m_dragMode = None;
    QPointF m_clickOffset;   // 点击偏移量

    bool findActiveVertex(const QPointF &pos);
    void emitUpdateSignal();
};
