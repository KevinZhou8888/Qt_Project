#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QGraphicsRectItem>
#include "controller.h"
#include "timeline.h"
#include <QLabel>
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
    QVector<QPointF> bezierPoints;
    QLabel * timePer;
    QLabel * AnimPer;
public slots:
    void updatePath();
    void intersected();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
