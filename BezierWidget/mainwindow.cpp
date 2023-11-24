#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <cmath>
int r = 10;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("BezierWigdet");
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
//    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    scene->setSceneRect(0, 0, 500, 500);

    controlPoint1= new Controller(0,0,20,20);
    controlPoint2= new Controller(0,0,20,20);
    controlPoint1->setPos(-r,500-r);
    controlPoint2->setPos(500-r,-r);

    rect = new QGraphicsRectItem(0,0,500,500);
    rect->setPen(QPen(Qt::black, 0.5));

    scene->addItem(controlPoint1);
    scene->addItem(controlPoint2);
    scene->addItem(rect);

    QPainterPath path;
    path.moveTo(0,500);
    // cubicTo()需要四个点，起始点，两个控制点，和结束点
    // 这里设定第一个控制点为(40,10)，第二个控制点为(10,40)，结束点为(40,40)
    path.cubicTo(0,500, 500,0, 500,0);
    pathItem = new QGraphicsPathItem(path);
    pathItem->setPen(QPen(Qt::black, 3));
    scene->addItem(pathItem);


    QPainterPath lineA;
    lineA.moveTo(0,0);
    lineA.lineTo(500,500);
    line1 =new QGraphicsPathItem(lineA);
    line1->setPen(QPen(Qt::black, 1));
    scene->addItem(line1);

    QPainterPath lineB;
    lineB.moveTo(500,0);
    lineB.lineTo(500,500);
    line2 =new QGraphicsPathItem(lineB);
    line2->setPen(QPen(Qt::black, 1));
    scene->addItem(line2);

    QPainterPath tLine;
    tLine.moveTo(0,0);
    tLine.lineTo(0,500);
    timeline =new timeLine(tLine);
    timeline->setPen(QPen(Qt::black, 0.5));
    timeline->setPos(0,0);
    qDebug()<<timeline->pos();
    scene->addItem(timeline);

    QPainterPath LineY;
    LineY.moveTo(0,0);
    LineY.lineTo(500,0);
    lineY =new QGraphicsPathItem(LineY);
    lineY->setPen(QPen(Qt::black, 0.5));
    lineY->setPos(0,500);
    scene->addItem(lineY);

    QGraphicsTextItem* textItem = new QGraphicsTextItem("动画进度");
    scene->addItem(textItem);
    QFont font("Arial", 10);  // 字体为Arial，大小为16
    textItem->setFont(font);
    textItem->setRotation(270);
    textItem->setPos(-20,280);

    QGraphicsTextItem* textItem2 = new QGraphicsTextItem("时间进度");
    scene->addItem(textItem2);
    textItem2->setFont(font);
    textItem2->setPos(220,500);

    timePer = new QLabel(this);
    AnimPer =new QLabel(this);
    ui->statusbar->addWidget(timePer);
    ui->statusbar->addWidget(AnimPer);
    timePer->setText("时间进度: 0%");
    AnimPer->setText("动画进度: 0%");

    ui->graphicsView->show();

    connect(controlPoint1,&Controller::positionChanged,this,&MainWindow::updatePath);
    connect(controlPoint2,&Controller::positionChanged,this,&MainWindow::updatePath);
    connect(timeline,&timeLine::timeChanged,this,&MainWindow::intersected);

    updatePath();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updatePath()
{

    QPainterPath path;
    path.moveTo(0,500);
    qreal x1 = controlPoint1->pos().x();
    qreal y1= controlPoint1->pos().y();
    qreal x2 = controlPoint2->pos().x();
    qreal y2= controlPoint2->pos().y();
//    qDebug()<<x1<<" "<<y1+500;
//    qDebug()<<x2+500<<" "<<y2;
    path.cubicTo(x1+r, y1+r, x2+r, y2+r, 500, 0);
    bezierPath = path;
    pathItem->setPath(path);

    QPainterPath lineA;
    lineA.moveTo(0,500);
    lineA.lineTo(x1+r, y1+r);
    line1->setPath(lineA);

    QPainterPath lineB;
    lineB.moveTo(500,0);
    lineB.lineTo(x2+r, y2+r);
    line2->setPath(lineB);

    qreal x = timeline->pos().x();
//    qDebug()<<x;
    bezierPoints.clear();
    qreal delta=10000;
    QPointF nearestPoint;
    const int numBezierPoints = 1000;  // 更改这个数，获得更准确或更高效的结果
        for (int i = 0; i <= numBezierPoints; ++i) {
            qreal percent = (qreal)i / numBezierPoints;
//            bezierPoints.append(bezierPath.pointAtPercent(percent));
            QPointF bezierPoint = bezierPath.pointAtPercent(percent);
            if (delta >  std::abs(x - bezierPoint.x()))
            {
               delta = std::abs(x - bezierPoint.x());
               nearestPoint = bezierPoint;
            }
        }

//    qDebug()<<nearestPoint.y();
    lineY->setPos(0,nearestPoint.y());

    qreal timepercent = x/500;
    qreal animpercent =1- nearestPoint.y()/500;
    timePer->setText("时间进度: "+QString::number(timepercent*100,'f', 1) +"%      ");
    AnimPer->setText("动画进度: "+QString::number(animpercent*100,'f', 1)+"%" );


    update();

}

void MainWindow::intersected()
{
    qreal x = timeline->pos().x();
//    qDebug()<<x;
    bezierPoints.clear();
    qreal delta=10000;
    QPointF nearestPoint;
    const int numBezierPoints = 1000;  // 更改这个数，获得更准确或更高效的结果
        for (int i = 0; i <= numBezierPoints; ++i) {
            qreal percent = (qreal)i / numBezierPoints;
//            bezierPoints.append(bezierPath.pointAtPercent(percent));
            QPointF bezierPoint = bezierPath.pointAtPercent(percent);
            if (delta >  std::abs(x - bezierPoint.x()))
            {
               delta = std::abs(x - bezierPoint.x());
               nearestPoint = bezierPoint;
            }
        }

    lineY->setPos(0,nearestPoint.y());

    qreal timepercent = x/500;
    qreal animpercent =1- nearestPoint.y()/500;
    timePer->setText("时间进度: "+QString::number(timepercent*100,'f', 1) +"%      ");
    AnimPer->setText("动画进度: "+QString::number(animpercent*100,'f', 1)+"%" );
}

