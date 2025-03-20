#include "CircleItem.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <cmath>
#include<QCursor>
CircleItem::CircleItem(QPointF center, qreal radius)
    : m_center(center), m_radius(radius) {
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
}

QRectF CircleItem::boundingRect() const {
    return QRectF(m_center.x() - m_radius - 2,
                  m_center.y() - m_radius - 2,
                  2 * m_radius + 4,
                  2 * m_radius + 4);
}

void CircleItem::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *) {
    painter->setPen(QPen(Qt::darkBlue, 2));
    painter->setBrush(QColor(100, 150, 255, 150));
    painter->drawEllipse(m_center, m_radius, m_radius);

    if (isSelected()) {
        // 绘制控制点
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::red);
        painter->drawEllipse(m_center, 3, 3); // 中心点
        painter->drawEllipse(m_center + QPointF(m_radius, 0), 3, 3); // 右侧控制点
    }
}

double CircleItem::calcArea() const {
    return M_PI * m_radius * m_radius;
}

void CircleItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    const QPointF pos = event->pos();
    const qreal distanceToCenter = QLineF(pos, m_center).length();
    const qreal distanceToEdge = std::abs(distanceToCenter - m_radius);

    // 判断操作模式
    if (distanceToEdge < 5) { // 边缘5像素内
        m_dragMode = Resize;
        m_clickOffset = pos - (m_center + QPointF(m_radius, 0));
    } else if (distanceToCenter < m_radius) { // 内部
        m_dragMode = Move;
        setCursor(Qt::ClosedHandCursor);
    }

    QGraphicsItem::mousePressEvent(event);
}

void CircleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    switch (m_dragMode) {
    case Resize: {
        // 计算新半径
        QPointF edgePoint = event->pos() - m_clickOffset;
        m_radius = QLineF(m_center, edgePoint).length();
        prepareGeometryChange();
        update();
        Q_EMIT parametersChanged(mapToScene(m_center), m_radius, calcArea());
        break;
    }
    case Move:
        // 移动由基类处理
        QGraphicsItem::mouseMoveEvent(event);
        Q_EMIT parametersChanged(mapToScene(m_center), m_radius, calcArea());
        break;
    default:
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void CircleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    m_dragMode = None;
    setCursor(Qt::ArrowCursor);
    QGraphicsItem::mouseReleaseEvent(event);
}

QVariant CircleItem::itemChange(QGraphicsItem::GraphicsItemChange change,
                                const QVariant &value) {
    if (change == ItemPositionHasChanged) {
        Q_EMIT parametersChanged(mapToScene(m_center), m_radius, calcArea());
    }
    return BaseShapeItem::itemChange(change, value);
}
