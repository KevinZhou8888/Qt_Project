#ifndef TIMELINE_H
#define TIMELINE_H

#include <QGraphicsPathItem>

class timeLine : public QObject,public QGraphicsPathItem
{
    Q_OBJECT
public:
    timeLine(const QPainterPath &path, QGraphicsItem *parent = nullptr);
signals:
    void timeChanged();
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};

#endif // TIMELINE_H
