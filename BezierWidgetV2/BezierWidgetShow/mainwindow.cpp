#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <cmath>
#include <QKeyEvent>

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




//设置graphicsView2
    scene2 = new QGraphicsScene(this);
    ui->graphicsView_2->setScene(scene2);
    ui->graphicsView_2->setMouseTracking(true);
    ui->graphicsView_2->setRenderHint(QPainter::Antialiasing);
     // ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    scene2->setSceneRect(0, 0, 250, 500);

    rectPos = new QGraphicsRectItem(0,0,30,30);
    rectRotation = new QGraphicsRectItem(0,0,30,30);
    rectRotation->setTransformOriginPoint(15,15);
    rectColor = new QGraphicsRectItem(0,0,30,30);
    rectPos->setPos(100,30);
    rectRotation->setPos(100,300);
    rectColor->setPos(100,400);
    scene2->addItem(rectPos);
    scene2->addItem(rectRotation);
    scene2->addItem(rectColor);

//播放设置
    connect(&timer,&QTimer::timeout,this,&MainWindow::play);
    periodTime=1.5;
    timer.start(Interval(periodTime));
    period = new QLabel("周期："+QString::number(periodTime)+"秒");
    ui->statusbar->addWidget(period);
    //按键说明
    KeyOperation = new QLabel(this);
    ui->statusbar->addWidget(KeyOperation);
    KeyOperation->setText("空格:播放/暂停 S:显示/隐藏导数 +/-键: 增加减少周期时间");
// 导数计算
    //采样

    derivative=new QGraphicsPathItem(derivativePath());
    derivative->setPen(QPen(Qt::red, 2));
    scene->addItem(derivative);


    updatePath();
}
MainWindow::~MainWindow()
{
    delete ui;
}

qreal MainWindow::Interval(qreal periodTime)
{
    //周期 秒 -->计算每一帧的间隔时间
    qreal interval;
    interval = periodTime*1000/500;
    return interval;
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
//    QPointF nearestPoint;

//    const int numBezierPoints = 300;  // 更改这个数，获得更准确或更高效的结果
//        for (int i = 0; i <= numBezierPoints; ++i) {
//            qreal percent = (qreal)i / numBezierPoints;
////            bezierPoints.append(bezierPath.pointAtPercent(percent));
//            QPointF bezierPoint = bezierPath.pointAtPercent(percent);
//            if (delta >  std::abs(x - bezierPoint.x()))
//            {
//               delta = std::abs(x - bezierPoint.x());
//               nearestPoint = bezierPoint;
//            }
//        }
    qreal tmax = 1;
    qreal tmin = 0;
    qreal tmid;
//使用二分法求Y值
    QPointF nearestPoint2;
        for(int i=0;i<10;i++){
            tmid=midpoint(tmax,tmin);
//            qDebug()<<tmid;
            QPointF bezierPoint = bezierPath.pointAtPercent(tmid);
            if(x>bezierPoint.x())
                tmin = tmid;
            else
                tmax = tmid;
        }
    nearestPoint2 = bezierPath.pointAtPercent(tmid);
//    qDebug()<<nearestPoint2 << nearestPoint;

//    qDebug()<<nearestPoint.y();
    lineY->setPos(0,nearestPoint2.y());

    qreal timepercent = x/500;
    qreal animpercent =1- nearestPoint2.y()/500;
    timePer->setText("时间进度: "+QString::number(timepercent*100,'f', 1) +"%      ");
    AnimPer->setText("动画进度: "+QString::number(animpercent*100,'f', 1)+"%" );

    derivative->setPath(derivativePath());
}

void MainWindow::intersected()
{
    qreal x = timeline->pos().x();
//    qDebug()<<x;
    bezierPoints.clear();
//    qreal delta=10000;
//    QPointF nearestPoint;
//    const int numBezierPoints = 300;  // 更改这个数，获得更准确或更高效的结果
//        for (int i = 0; i <= numBezierPoints; ++i) {
//            qreal percent = (qreal)i / numBezierPoints;
////            bezierPoints.append(bezierPath.pointAtPercent(percent));
//            QPointF bezierPoint = bezierPath.pointAtPercent(percent);
//            if (delta >  std::abs(x - bezierPoint.x()))
//            {
//               delta = std::abs(x - bezierPoint.x());
//               nearestPoint = bezierPoint;
//            }
//        }

        qreal tmax = 1;
        qreal tmin = 0;
        qreal tmid;
    //使用二分法求Y值
        QPointF nearestPoint2;
            for(int i=0;i<10;i++){
                tmid=midpoint(tmax,tmin);
    //            qDebug()<<tmid;
                QPointF bezierPoint = bezierPath.pointAtPercent(tmid);
                if(x>bezierPoint.x())
                    tmin = tmid;
                else
                    tmax = tmid;
            }
        nearestPoint2 = bezierPath.pointAtPercent(tmid);

    lineY->setPos(0,nearestPoint2.y());

    qreal timepercent = x/500;
    qreal animpercent =1- nearestPoint2.y()/500;
    timePer->setText("时间进度: "+QString::number(timepercent*100,'f', 1) +"%      ");
    AnimPer->setText("动画进度: "+QString::number(animpercent*100,'f', 1)+"%" );

    // X 0~500 --> rect.Y 30-->200 映射到位置

    qreal posDelta = 170*animpercent;
    qreal Y = 30+posDelta;
    // qDebug()<<Y ;
    rectPos->setPos(rectPos->pos().x(),Y);

    // X 0~500 --> 旋转 0~360 映射到旋转
    qreal angle = 360*animpercent;
    rectRotation->setRotation(angle);

   // X 0~500 --> 颜色色相0~1
    QColor color = getColorFromSpectrum(animpercent);
    QBrush brush(color);
    rectColor->setBrush(brush);
}

void MainWindow::play()
{
//    qDebug()<<"play";
    if(isPlay){
    qreal x = timeline->pos().x()+1;
    if(x<=500)
        timeline->setPos(x,timeline->pos().y());
    if(x>500)
        ++waitCounter;
    if(waitCounter>100){
        x=0;
        waitCounter=0;
        timeline->setPos(x,timeline->pos().y());
    qDebug()<<waitCounter;
    }
  }
}


void MainWindow::toZero()
{

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Space)
        isPlay = !isPlay;
    if(event->key()==Qt::Key_Minus ){
        if(periodTime<=0.1)
            return;
        periodTime-= 0.1;
        period->setText("周期："+QString::number(periodTime)+"秒") ;
        timer.start(Interval(periodTime));
    }
    if(event->key()==Qt::Key_Equal)
    {
        qDebug()<<periodTime;
        periodTime+= 0.1;
        period->setText("周期："+QString::number(periodTime)+"秒") ;
        timer.start(Interval(periodTime));
    }
    if(event->key()==Qt::Key_S)
    {
        bool isVisible = derivative->isVisible();
        derivative->setVisible(!isVisible);
    }
}


QColor MainWindow::getColorFromSpectrum(float value)
{     // Your value between 0 and 1
    int hue = static_cast<int>(value * 360);  // Convert the value to hue degree
    QColor color;
    color.setHsv(hue, 255, 255);  // Set the color with hue, maximum saturation and value
    return color;
}

QPainterPath MainWindow::derivativePath()
{
    const int numBezierPoints = 300;  // 更改这个数，获得更准确或更高效的结果
           for (int i = 0; i <= numBezierPoints; ++i) {
               qreal percent = (qreal)i / numBezierPoints;
               bezierPoints.append(bezierPath.pointAtPercent(percent));
           }

    QPainterPath derivativePath;
    QPointF point1;
    QPointF point2;
    derivativePath.moveTo(0,500);
    for (int i = 0; i <= numBezierPoints-1; ++i){
        point1 = bezierPoints[i];
        point2 = bezierPoints[i+1];
        qreal k = (point1.y()-point2.y())/(point2.x()-point1.x());
        derivativePath.lineTo(point1.x(),500-k*30);
    }
    return derivativePath;
}



