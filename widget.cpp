// Widget.cpp
#include "Widget.h"
#include "EllipseItem.h"
#include "TriangleItem.h"
#include "ui_widget.h"
#include "CircleItem.h"
#include "QuadrilateralItem.h"
#include<QLineEdit>
#include<qmessagebox.h>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    //
    m_scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    m_scene->addLine(-1000,0,1000,0);
    m_scene->addLine(0,-1000,0,1000);
    m_scene->addText("0",QFont(0,0));
    for (int i = -1000; i < 1000; i+=100) {
        m_scene->addEllipse(i-5,-5,10,10,QPen(Qt::green),QBrush(Qt::green));
        m_scene->addEllipse(-5,i-5,10,10,QPen(Qt::green),QBrush(Qt::green));
        m_scene->addLine(1000,i,-1000,i);
        m_scene->addLine(i,1000,i,-1000);
    }
    //

}

void Widget::updateEllipseInfo(QPointF sceneCenter, qreal rx, qreal ry, double area) {
    QString info = QString("%1").arg(area/10000.0, 0, 'f', 2);
    Clear();
    ui->CenterlineEdit_3->setText(QString::number(sceneCenter.toPoint().x()/100.0)+"cm");
    ui->CenterlineEdit_4->setText(QString::number(-sceneCenter.toPoint().y()/100.0)+"cm");
    ui->alineEdit_2->setText(QString::number(rx/100.0)+"cm");
    ui->blineEdit_2->setText(QString::number(ry/100.0)+"cm");
    ui->slineEdit->setText(info+"cm^2");
}

void Widget::updateQuadrilateralitemInfo(const QPolygonF &scenePoints, double area) {
    QString   info = QString("%1").arg(area/10000.0, 0, 'f', 2);
    Clear();
    ui->x1lineidt_2->setText(QString::number(scenePoints.at(0).x()/100.0)+"cm");
    ui->y1lineEdit_2->setText(QString::number(-scenePoints.at(0).y()/100.0)+"cm");
    ui->x2lineEdit_2->setText(QString::number(scenePoints.at(1).x()/100.0)+"cm");
    ui->y2lineEdit_2->setText(QString::number(-scenePoints.at(1).y()/100.0)+"cm");
    ui->x3lineEdit_2->setText(QString::number(scenePoints.at(2).x()/100.0)+"cm");
    ui->y3lineEdit_2->setText(QString::number(-scenePoints.at(2).y()/100.0)+"cm");
    ui->x4lineEdit_2->setText(QString::number(scenePoints.at(3).x()/100.0)+"cm");
    ui->y4lineEdit_2->setText(QString::number(scenePoints.at(3).y()/100.0)+"cm");
    ui->slineEdit ->setText(info+"cm^2");
}

void Widget::updateTriangleInfo(const QPolygonF &scenePoints, double area) {
    QString   info = QString("%1").arg(area/10000.0, 0, 'f', 2);
    Clear();
    ui->x1lineidt_2->setText(QString::number(scenePoints.at(0).x()/100.0)+"cm");
    ui->y1lineEdit_2->setText(QString::number(-scenePoints.at(0).y()/100.0)+"cm");
    ui->x2lineEdit_2->setText(QString::number(scenePoints.at(1).x()/100.0)+"cm");
    ui->y2lineEdit_2->setText(QString::number(-scenePoints.at(1).y()/100.0)+"cm");
    ui->x3lineEdit_2->setText(QString::number(scenePoints.at(2).x()/100.0)+"cm");
    ui->y3lineEdit_2->setText(QString::number(-scenePoints.at(2).y()/100.0)+"cm");
    ui->slineEdit ->setText(info+"cm^2");
}

void Widget::updateCircleInfo(QPointF sceneCenter, qreal radius, double area) {
    QString info = QString("%1").arg(area/10000.0, 0, 'f', 2);
    Clear();
    ui->x1lineidt_2->setText(QString::number(sceneCenter.toPoint().x()/100.0)+"cm");
    ui->y1lineEdit_2->setText(QString::number(-sceneCenter.toPoint().y()/100.0)+"cm");
    ui->rlineEdit_2->setText(QString("%1").arg(radius/100.0,0,'f',4)+"cm");
    ui->slineEdit->setText(info+"cm^2");
}

void Widget::on_pushButton_clicked()
{
    if(is_addMode==false)
    {
        if(choice==4)
        {
            if(Item!=nullptr)
            {
                m_scene->removeItem(Item);
                delete Item;
                Item=nullptr;
            }
            Item=new EllipseItem(QPointF(ui->CenterlineEdit->text().toDouble(),ui->CenterlineEdit_2->text().toDouble())
                                   ,ui->alineEdit->text().toDouble(),ui->blineEdit->text().toDouble() );
            m_scene->addItem(Item);
            EllipseItem* ell=qobject_cast<EllipseItem*>(Item);
            connect(ell,&EllipseItem::parametersChanged,this,&Widget::updateEllipseInfo);
        }
        else if(choice==3)
        {
            if(Item!=nullptr)
            {
                m_scene->removeItem(Item);
                delete Item;
                Item=nullptr;
            }
            Item=new CircleItem(QPointF(ui->lineEdit->text().toDouble(),-ui->lineEdit_2->text().toDouble()),ui->rlineEdit->text().toDouble());
            m_scene->addItem(Item);
            CircleItem* circle = qobject_cast<CircleItem*>(Item);
            connect(circle,&CircleItem::parametersChanged,this,&Widget::updateCircleInfo);
        }else if(choice==2)
        {
            if(Item!=nullptr)
            {
                m_scene->removeItem(Item);
                delete Item;
                Item=nullptr;
            }
            Item=new TriangleItem(QPolygonF()<<QPointF(ui->x1lineidt->text().toDouble(),-ui->y1lineEdit->text().toDouble())
                                                <<QPointF(ui->x2lineEdit->text().toDouble(),-ui->y2lineEdit->text().toDouble())
                                                <<QPointF(ui->x3lineEdit->text().toDouble(),-ui->y3lineEdit->text().toDouble()));
            m_scene->addItem(Item);
            TriangleItem* tri=qobject_cast<TriangleItem*>(Item);
            connect(tri,&TriangleItem::parametersChanged,this,&Widget::updateTriangleInfo);
        }else if(choice==1)
        {
            if(Item!=nullptr)
            {
                m_scene->removeItem(Item);
                delete Item;
                Item=nullptr;
            }
            Item=new QuadrilateralItem(QPolygonF()<<QPointF(ui->x1lineidt->text().toDouble(),-ui->y1lineEdit->text().toDouble())
                                                <<QPointF(ui->x2lineEdit->text().toDouble(),-ui->y2lineEdit->text().toDouble())
                                                <<QPointF(ui->x3lineEdit->text().toDouble(),-ui->y3lineEdit->text().toDouble())
                                                     <<QPointF(ui->x4lineEdit->text().toDouble(),-ui->y4lineEdit->text().toDouble()));
            m_scene->addItem(Item);
            QuadrilateralItem* qua=qobject_cast<QuadrilateralItem*>(Item);
            connect(qua,&QuadrilateralItem::parametersChanged,this,&Widget::updateQuadrilateralitemInfo);
        }
    }else
    {
        if(choice==4)
        {
            Item=new EllipseItem(QPointF(ui->CenterlineEdit->text().toDouble(),ui->CenterlineEdit_2->text().toDouble())
                                   ,ui->alineEdit->text().toDouble(),ui->blineEdit->text().toDouble() );
            m_scene->addItem(Item);
            EllipseItem* ell=qobject_cast<EllipseItem*>(Item);
            m_shapes.push_back(ell);
            connect(ell,&EllipseItem::parametersChanged,this,&Widget::updateEllipseInfo);
        }
        else if(choice==3)
        {
            Item=new CircleItem(QPointF(ui->lineEdit->text().toDouble(),-ui->lineEdit_2->text().toDouble()),ui->rlineEdit->text().toDouble());
            m_scene->addItem(Item);
            CircleItem* circle = qobject_cast<CircleItem*>(Item);
            m_shapes.push_back(circle);
            connect(circle,&CircleItem::parametersChanged,this,&Widget::updateCircleInfo);
        }else if(choice==2)
        {
            Item=new TriangleItem(QPolygonF()<<QPointF(ui->x1lineidt->text().toDouble(),-ui->y1lineEdit->text().toDouble())
                                                <<QPointF(ui->x2lineEdit->text().toDouble(),-ui->y2lineEdit->text().toDouble())
                                                <<QPointF(ui->x3lineEdit->text().toDouble(),-ui->y3lineEdit->text().toDouble()));
            m_scene->addItem(Item);
            TriangleItem* tri=qobject_cast<TriangleItem*>(Item);
            m_shapes.push_back(tri);
            connect(tri,&TriangleItem::parametersChanged,this,&Widget::updateTriangleInfo);
        }else if(choice==1)
        {
            Item=new QuadrilateralItem(QPolygonF()<<QPointF(ui->x1lineidt->text().toDouble(),-ui->y1lineEdit->text().toDouble())
                                                     <<QPointF(ui->x2lineEdit->text().toDouble(),-ui->y2lineEdit->text().toDouble())
                                                     <<QPointF(ui->x3lineEdit->text().toDouble(),-ui->y3lineEdit->text().toDouble())
                                                     <<QPointF(ui->x4lineEdit->text().toDouble(),-ui->y4lineEdit->text().toDouble()));
            m_scene->addItem(Item);
            QuadrilateralItem* qua=qobject_cast<QuadrilateralItem*>(Item);
            m_shapes.push_back(qua);
            connect(qua,&QuadrilateralItem::parametersChanged,this,&Widget::updateQuadrilateralitemInfo);
        }}

}

Widget::~Widget(){
    // 释放内存
    qDeleteAll(m_shapes);
    delete ui;
}

void Widget::on_comboBox_currentIndexChanged(int index)
{
    if(index==2){
        // ui->rlineEdit->setEnabled(true);
        // ui->x1lineidt->setEnabled(true);
        // ui->y1lineEdit->setEnabled(true);
        // ui->x2lineEdit->setEnabled(false);
        // ui->y2lineEdit->setEnabled(false);
        // ui->x3lineEdit->setEnabled(false);
        // ui->y3lineEdit->setEnabled(false);
        // ui->x4lineEdit->setEnabled(false);
        // ui->y4lineEdit->setEnabled(false);
        // ui->CenterlineEdit->setEnabled(false);
        // ui->alineEdit->setEnabled(false);
        // ui->blineEdit->setEnabled(false);
        // ui->CenterlineEdit_2->setEnabled(false);
        choice=3;
    }else if(index==1)
    {
        // ui->rlineEdit->setEnabled(false);
        // ui->x1lineidt->setEnabled(true);
        // ui->y1lineEdit->setEnabled(true);
        // ui->x2lineEdit->setEnabled(true);
        // ui->y2lineEdit->setEnabled(true);
        // ui->x3lineEdit->setEnabled(true);
        // ui->y3lineEdit->setEnabled(true);
        // ui->x4lineEdit->setEnabled(false);
        // ui->y4lineEdit->setEnabled(false);
        // ui->CenterlineEdit->setEnabled(false);
        // ui->alineEdit->setEnabled(false);
        // ui->blineEdit->setEnabled(false);
        // ui->CenterlineEdit_2->setEnabled(false);
        choice=2;
    }else if(index==0)
        {
            // ui->rlineEdit->setEnabled(false);
            // ui->x1lineidt->setEnabled(true);
            // ui->y1lineEdit->setEnabled(true);
            // ui->x2lineEdit->setEnabled(true);
            // ui->y2lineEdit->setEnabled(true);
            // ui->x3lineEdit->setEnabled(true);
            // ui->y3lineEdit->setEnabled(true);
            // ui->x4lineEdit->setEnabled(true);
            // ui->y4lineEdit->setEnabled(true);
            // ui->CenterlineEdit->setEnabled(false);
            // ui->alineEdit->setEnabled(false);
            // ui->blineEdit->setEnabled(false);
            // ui->CenterlineEdit_2->setEnabled(false);
            choice=1;
    }else if(index==3)
    {
        // ui->rlineEdit->setEnabled(false);
        // ui->x1lineidt->setEnabled(false);
        // ui->y1lineEdit->setEnabled(false);
        // ui->x2lineEdit->setEnabled(false);
        // ui->y2lineEdit->setEnabled(false);
        // ui->x3lineEdit->setEnabled(false);
        // ui->y3lineEdit->setEnabled(false);
        // ui->x4lineEdit->setEnabled(false);
        // ui->y4lineEdit->setEnabled(false);
        // ui->CenterlineEdit->setEnabled(true);
        // ui->alineEdit->setEnabled(true);
        // ui->blineEdit->setEnabled(true);
        // ui->CenterlineEdit_2->setEnabled(true);
        choice=4;
    }
}


void Widget::on_checkBox_stateChanged(int arg1)
{
    if(arg1==0)
    {
        this->is_addMode=false;
    }else
    {
        if(Item!=nullptr)
        {
            m_shapes.push_back(Item);
        }
        this->is_addMode=true;
    }
}


void Widget::on_deletepushButton_clicked()
{
    Clear();
    if(is_addMode==1){
    if(m_shapes.isEmpty())
    {
        QMessageBox::warning(this, "警告", "该列表中已经没有元素了!");
        return;
    }
    BaseShapeItem* item = m_shapes.first();
    m_scene->removeItem(item);
    delete item;  // 释放内存
    // 从列表移除A
    m_shapes.pop_front();
    }
    else
    {
        if(Item==nullptr)
        {
            QMessageBox::warning(this, "警告", "都没了还删!蠢猪");
            return;
        }
        m_scene->removeItem(Item);
        Clear();
        Item=nullptr;
    }
}

void Widget::Clear()
{
    ui->CenterlineEdit_3->clear();
    ui->CenterlineEdit_4->clear();
    ui->x2lineEdit_2->clear();
    ui->x1lineidt_2->clear();
    ui->x3lineEdit_2->clear();
    ui->x4lineEdit_2->clear();
    ui->y1lineEdit_2->clear();
    ui->y2lineEdit_2->clear();
    ui->y3lineEdit_2->clear();
    ui->y4lineEdit_2->clear();
    ui->alineEdit_2->clear();
    ui->blineEdit_2->clear();
    ui->rlineEdit_2->clear();
    ui->slineEdit->clear();
}
