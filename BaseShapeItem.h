#ifndef BASESHAPEITEM_H
#define BASESHAPEITEM_H
#pragma once
#include <QObject>
#include <QGraphicsItem>

class BaseShapeItem : public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit BaseShapeItem(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~BaseShapeItem() = default;

    // 公共接口（例如计算面积）
    virtual double calcArea() const = 0;

signals:
    void positionChanged(QPointF pos);  // 统一信号
};
#endif // BASESHAPEITEM_H
