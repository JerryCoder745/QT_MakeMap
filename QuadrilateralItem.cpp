// QuadrilateralItem.cpp
#include "QuadrilateralItem.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <cmath>

// 控制点半径（像素）
constexpr double CONTROL_POINT_RADIUS = 5.0;

QuadrilateralItem::QuadrilateralItem(const QPolygonF &points) {
    Q_ASSERT_X(points.size() == 4, "QuadrilateralItem", "需要4个顶点");
    m_points = points;

    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
}
QVariant QuadrilateralItem::itemChange(QGraphicsItem::GraphicsItemChange change,
                                       const QVariant &value) {
    if (change == QGraphicsItem::ItemPositionHasChanged) {
        emitUpdateSignal();
    }
    return BaseShapeItem::itemChange(change, value);
}

void QuadrilateralItem::emitUpdateSignal()
{
    QPolygonF scenePoints;
    for (const auto& p : std::as_const(m_points)) {
        scenePoints << mapToScene(p);
    }
    Q_EMIT parametersChanged(scenePoints, calcArea());
}

QRectF QuadrilateralItem::boundingRect() const {
    return m_points.boundingRect().adjusted(-10, -10, 10, 10); // 扩大边界便于操作
}

void QuadrilateralItem::paint(QPainter *painter,
                              const QStyleOptionGraphicsItem *option,
                              QWidget *) {
    // 绘制多边形
    painter->setPen(QPen(Qt::blue, 2));
    painter->setBrush(QColor(100, 150, 255, 100));
    painter->drawPolygon(m_points);

    // 绘制控制点（选中时）
    if (isSelected()) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::red);
        for (const auto& p : std::as_const(m_points)) {
            painter->drawEllipse(p, CONTROL_POINT_RADIUS, CONTROL_POINT_RADIUS);
        }
    }
}

double QuadrilateralItem::calcArea() const {
    // 使用鞋带公式计算面积（本地坐标）
    qreal area = 0;
    for (int i = 0; i < 4; ++i) {
        const QPointF& p1 = m_points[i];
        const QPointF& p2 = m_points[(i+1)%4];
        area += p1.x() * p2.y() - p2.x() * p1.y();
    }
    return std::abs(area) / 2.0;
}

void QuadrilateralItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (isSelected()) {
        // 检测是否点击了控制点
        const QPointF pos = event->pos();
        m_activePoint = findControlPoint(pos);

        if (m_activePoint != -1) { // 顶点拖动模式
            m_dragOffset = pos - m_points[m_activePoint];
            event->accept();
            return;
        }
    }

    // 否则进入整体拖动模式
    m_draggingAll = true;
    QGraphicsItem::mousePressEvent(event);
}

void QuadrilateralItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (m_activePoint != -1) { // 顶点拖动
        prepareGeometryChange();

        // 计算新位置（限制在视图范围内）
        QPointF newPos = event->pos() - m_dragOffset;
        newPos.setX(qBound(-1000.0, newPos.x(), 1000.0));
        newPos.setY(qBound(-1000.0, newPos.y(), 1000.0));

        m_points[m_activePoint] = newPos;
        update();
        emitUpdateSignal();
        event->accept();
    } else if (m_draggingAll) { // 整体拖动
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void QuadrilateralItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    m_activePoint = -1;
    m_draggingAll = false;
    QGraphicsItem::mouseReleaseEvent(event);
}

int QuadrilateralItem::findControlPoint(const QPointF& pos) const {
    // 查找最近的顶点
    for (int i = 0; i < 4; ++i) {
        const QPointF delta = pos - m_points[i];
        const double distance = std::hypot(delta.x(), delta.y());
        if (distance <= CONTROL_POINT_RADIUS * 1.5) { // 检测半径扩大1.5倍
            return i;
        }
    }
    return -1;
}

void QuadrilateralItem::updatePoint(int index, const QPointF& newPos) {
    Q_ASSERT_X(index >=0 && index <4, "updatePoint", "索引越界");
    prepareGeometryChange();
    m_points[index] = newPos;
    update();
}
