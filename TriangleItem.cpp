#include "TriangleItem.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QLineF>
#include<QCursor>
constexpr double CONTROL_RADIUS = 5.0;

TriangleItem::TriangleItem(const QPolygonF &points) {
    Q_ASSERT_X(points.size() == 3, "TriangleItem", "需要3个顶点");
    m_points = points;

    setFlag(ItemIsMovable);//设置图形可移动
    setFlag(ItemIsSelectable);//设置图形可选中
    setFlag(ItemSendsGeometryChanges);//图形项在位置或变换发生变化时发送通知
}

//向各个方向扩展5个单位长度,扩大可以选择的范围,方便图形操作
QRectF TriangleItem::boundingRect() const {
    return m_points.boundingRect().adjusted(-5, -5, 5, 5);
}

void TriangleItem::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *) {
    painter->setPen(QPen(Qt::darkGreen, 2));
    painter->setBrush(QColor(150, 255, 150, 100));
    painter->drawPolygon(m_points);//绘制

    //如果被选中,绘制红色控制点,通过它们来调整图形项的形状或位置
    if (isSelected()) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::red);
        for (const auto& p : std::as_const(m_points)) {
            painter->drawEllipse(p, CONTROL_RADIUS, CONTROL_RADIUS);
        }
    }
}

//计算面积
double TriangleItem::calcArea() const {
    const QPointF &a = m_points[0];
    const QPointF &b = m_points[1];
    const QPointF &c = m_points[2];
    return std::abs((a.x()*(b.y()-c.y()) +
                    b.x()*(c.y()-a.y()) +
                    c.x()*(a.y()-b.y())) / 2.0);
}

// 当鼠标按下事件发生时触发
    void TriangleItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    // 获取鼠标点击位置相对于图形项的坐标
    const QPointF pos = event->pos();

    // 检查点击位置是否在某个顶点附近，优先处理顶点拖动
    if (findActiveVertex(pos)) {
        m_dragMode = VertexDrag;  // 设置为顶点拖拽模式
        m_clickOffset = pos - m_points[m_activeVertex];  // 计算点击偏移量
        event->accept();  // 接受事件，表示已经处理了该事件
        return;
    }

    // 如果没有在顶点附近点击，则检查是否点击在三角形内部
    if (QPolygonF(m_points).containsPoint(pos, Qt::OddEvenFill)) {
        m_dragMode = Move;  // 设置为移动整个图形项模式
        setCursor(Qt::ClosedHandCursor);  // 改变鼠标指针样式
        event->accept();  // 接受事件
    }
}

// 当鼠标移动事件发生时触发
void TriangleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    switch (m_dragMode) {
    case VertexDrag: {  // 处理顶点拖拽
        prepareGeometryChange();  // 准备进行几何变化
        m_points[m_activeVertex] = event->pos() - m_clickOffset;  // 更新顶点位置
        update();  // 请求重绘
        emitUpdateSignal();  // 发出更新信号
        break;
    }
    case Move:  // 处理整个图形项的移动
        QGraphicsItem::mouseMoveEvent(event);  // 调用基类实现来移动图形项
        emitUpdateSignal();  // 发出更新信号
        break;
    default:;
    }
}

// 当鼠标释放事件发生时触发
void TriangleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    m_dragMode = None;  // 重置拖动模式
    m_activeVertex = -1;  // 重置活动顶点索引
    setCursor(Qt::ArrowCursor);  // 恢复默认鼠标指针样式
    QGraphicsItem::mouseReleaseEvent(event);  // 调用基类实现
}

// 当图形项属性发生变化时触发
QVariant TriangleItem::itemChange(QGraphicsItem::GraphicsItemChange change,
                                  const QVariant &value) {
    if (change == ItemPositionHasChanged) {  // 如果位置发生了变化
        emitUpdateSignal();  // 发出更新信号
    }
    return BaseShapeItem::itemChange(change, value);  // 返回基类的处理结果
}

// 查找并返回活动的顶点（如果有的话）
bool TriangleItem::findActiveVertex(const QPointF &pos) {
    for (int i = 0; i < 3; ++i) {  // 遍历三个顶点
        if (QLineF(pos, m_points[i]).length() <= CONTROL_RADIUS*1.5) {  // 如果点击位置靠近某个顶点
            m_activeVertex = i;  // 设置活动顶点
            return true;
        }
    }
    return false;  // 如果没有找到靠近的顶点
}

// 发出包含参数变化的信号
void TriangleItem::emitUpdateSignal() {
    QPolygonF scenePoints;
    for (const auto& p : std::as_const(m_points)) {
        scenePoints << mapToScene(p);  // 将本地坐标转换为场景坐标
    }
    Q_EMIT parametersChanged(scenePoints, calcArea());  // 发出参数变化信号
}

// 返回一个默认的三角形
QPolygonF TriangleItem::defaultTriangle() {
    return QPolygonF() << QPointF(-50, 50)
    << QPointF(0, -50)
    << QPointF(50, 50);
}
