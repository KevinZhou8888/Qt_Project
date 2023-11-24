#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QGraphicsEllipseItem>
#include <QBrush>
class Controller : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    Controller(qreal x, qreal y, qreal width, qreal height);
signals:
    void positionChanged(qreal x, qreal y);
protected:
   QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
   void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};




#endif // CONTROLLER_H
