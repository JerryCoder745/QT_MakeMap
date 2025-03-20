// Widget.h
#pragma once
#include <QWidget>
#include <QList>
#include "BaseShapeItem.h"
#include "qgraphicsscene.h"

namespace Ui { class Widget; }

class Widget : public QWidget {
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void updateCircleInfo(QPointF sceneCenter, qreal radius, double area) ;
    void updateTriangleInfo(const QPolygonF &scenePoints, double area);
    void updateEllipseInfo(QPointF sceneCenter, qreal rx, qreal ry, double area);
    void updateQuadrilateralitemInfo(const QPolygonF &scenePoints, double area);
    void Clear();
private slots:
     // 添加图形按钮

    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_checkBox_stateChanged(int arg1);

    void on_deletepushButton_clicked();

private:
    Ui::Widget *ui;
    QGraphicsScene *m_scene = nullptr;
    QList<BaseShapeItem*> m_shapes;  // 存储所有图形项
    int choice=0;
    BaseShapeItem *Item=nullptr;
    bool is_addMode=false;
};
