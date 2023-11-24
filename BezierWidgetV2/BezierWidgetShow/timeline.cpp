#include "timeline.h"
#include <QGraphicsSceneMouseEvent>
timeLine::timeLine(const QPainterPath &path, QGraphicsItem *parent):QGraphicsPathItem(path)
{
    setFlags(QGraphicsItem::ItemIsMovable| QGraphicsItem::ItemSendsGeometryChanges);
}

void timeLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qreal dx = event->scenePos().x() - event->lastScenePos().x();
    qreal newX = pos().x() + dx;
    setPos(newX, pos().y());
}

void timeLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (pos().x()>500)
        setPos(500,pos().y());
    if (pos().x()<0)
        setPos(0,pos().y());
}

QVariant timeLine::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged) {
             emit timeChanged();
    }
    return QGraphicsPathItem::itemChange(change, value);
}
