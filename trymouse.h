#ifndef TRYMOUSE_H
#define TRYMOUSE_H

#include <QGraphicsView>
#include <QMouseEvent>
#include"Shape.h"

class trymouse : public QGraphicsView
{
    Q_OBJECT
public:
    explicit trymouse(QWidget *parent = nullptr);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
signals:
private:

};

#endif // TRYMOUSE_H
