#ifndef BASE_H
#define BASE_H
// BaseShapeItem.h
#pragma once
#include <QObject>
#include <QGraphicsItem>

class BaseShapeItem : public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit BaseShapeItem(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~BaseShapeItem() = default;

signals:
    void positionChanged();  // 通用坐标变化信号
};
#endif // BASE_H
