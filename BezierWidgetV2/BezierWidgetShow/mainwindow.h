#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QGraphicsRectItem>
#include "controller.h"
#include "timeline.h"
#include <QLabel>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QGraphicsScene* scene;
    QGraphicsPathItem* pathItem;
    QGraphicsPathItem* line1;
    QGraphicsPathItem* line2;
     QGraphicsPathItem* lineY;
    QGraphicsRectItem* rect;
    timeLine * timeline;
    QPoint control1;
    QPoint control2;
    Controller * controlPoint1;
    Controller * controlPoint2;
    QPainterPath bezierPath;
    QLabel * timePer;
    QLabel * AnimPer;
    QLabel * KeyOperation;
//演示窗口
    QGraphicsScene* scene2;
    QGraphicsRectItem* rectPos;
    QGraphicsRectItem* rectRotation;
    QGraphicsRectItem* rectColor;
    QGraphicsTextItem* textTime;
//播放
    QTimer timer; // 计时触发器
    bool isPlay=false;
    qreal periodTime;
    qreal Interval(qreal periodTime);
    QLabel * period;
    int waitCounter=0;
//颜色光谱 0~1 渐变映射到光谱的颜色
    QColor getColorFromSpectrum(float value);
// 导数图像
    QVector<QPointF> bezierPoints;
    QGraphicsPathItem* derivative;
    QPainterPath derivativePath();
public slots:
    void updatePath();
    void intersected();
    void play();
    void toZero();

private:
    Ui::MainWindow *ui;
    qreal midpoint(qreal t1, qreal t2){
        return (t1+t2)/2;
    }
    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
};
#endif // MAINWINDOW_H
