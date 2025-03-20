#include "EllipseItem.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QtMath>
#include<QCursor>
constexpr double CTRL_POINT_RADIUS = 5.0;

EllipseItem::EllipseItem(QPointF center, qreal rx, qreal ry)
    : m_center(center), m_rx(rx), m_ry(ry) {
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    updateControlPoints();
}

QRectF EllipseItem::boundingRect() const {
    return QRectF(m_center.x() - m_rx - 5,
                  m_center.y() - m_ry - 5,
                  2 * m_rx + 10,
                  2 * m_ry + 10);
}

void EllipseItem::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        QWidget *) {
    painter->setPen(QPen(Qt::darkMagenta, 2));
    painter->setBrush(QColor(200, 150, 255, 150));
    painter->drawEllipse(m_center, m_rx, m_ry);

    if (isSelected()) {
        // 绘制控制点
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::red);
        painter->drawEllipse(m_center, CTRL_POINT_RADIUS, CTRL_POINT_RADIUS);
        painter->drawEllipse(m_xCtrlPoint, CTRL_POINT_RADIUS, CTRL_POINT_RADIUS);
        painter->drawEllipse(m_yCtrlPoint, CTRL_POINT_RADIUS, CTRL_POINT_RADIUS);
    }
}

double EllipseItem::calcArea() const {
    return M_PI * m_rx * m_ry;
}

void EllipseItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    const QPointF pos = event->pos();

    if (QLineF(pos, m_xCtrlPoint).length() < CTRL_POINT_RADIUS*2) {
        m_dragMode = AdjustX;
        m_clickOffset = pos - m_xCtrlPoint;
    } else if (QLineF(pos, m_yCtrlPoint).length() < CTRL_POINT_RADIUS*2) {
        m_dragMode = AdjustY;
        m_clickOffset = pos - m_yCtrlPoint;
    } else if (QRectF(m_center.x() - m_rx, m_center.y() - m_ry,
                      2*m_rx, 2*m_ry).contains(pos)) {
        m_dragMode = Move;
        setCursor(Qt::ClosedHandCursor);
    }

    event->accept();
}

void EllipseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    prepareGeometryChange();

    switch (m_dragMode) {
    case AdjustX: {
        QPointF newPos = event->pos() - m_clickOffset;
        m_rx = qMax(10.0, newPos.x() - m_center.x());
        updateControlPoints();
        break;
    }
    case AdjustY: {
        QPointF newPos = event->pos() - m_clickOffset;
        m_ry = qMax(10.0, newPos.y() - m_center.y());
        updateControlPoints();
        break;
    }
    case Move:
        QGraphicsItem::mouseMoveEvent(event);
        break;
    default:;
    }

    Q_EMIT parametersChanged(mapToScene(m_center), m_rx, m_ry, calcArea());
    update();
}

void EllipseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    m_dragMode = None;
    setCursor(Qt::ArrowCursor);
    QGraphicsItem::mouseReleaseEvent(event);
}

QVariant EllipseItem::itemChange(QGraphicsItem::GraphicsItemChange change,
                                 const QVariant &value) {
    if (change == ItemPositionHasChanged) {
        Q_EMIT parametersChanged(mapToScene(m_center), m_rx, m_ry, calcArea());
    }
    return BaseShapeItem::itemChange(change, value);
}

void EllipseItem::updateControlPoints() {
    m_xCtrlPoint = m_center + QPointF(m_rx, 0);
    m_yCtrlPoint = m_center + QPointF(0, m_ry);
}
