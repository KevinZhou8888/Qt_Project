#include "controller.h"

Controller::Controller(qreal x, qreal y, qreal width, qreal height)
    : QGraphicsEllipseItem(x, y, width, height)
{
    QBrush brush(Qt::white);
    //设置不透明度
    brush.setStyle(Qt::SolidPattern);
    setBrush(brush);
    setZValue(10);
    setFlags(QGraphicsItem::ItemIsMovable| QGraphicsItem::ItemSendsGeometryChanges);
}

QVariant Controller::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionHasChanged) {
             emit positionChanged(pos().x(),pos().y());
    }
    return QGraphicsEllipseItem::itemChange(change, value);
}

void Controller::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    int r = 10;
    QGraphicsEllipseItem::mouseReleaseEvent(event);
    if (pos().x()>500-r)
        setPos(500-r,pos().y());
    if (pos().x()<0-r)
        setPos(0-r,pos().y());
    if (pos().y()>500-r)
        setPos(pos().x(),500-r);
    if (pos().y()<0-r)
        setPos(pos().x(),0-r);

}

