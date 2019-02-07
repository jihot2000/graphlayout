#ifndef PATHITEM_H
#define PATHITEM_H

#include <QGraphicsPathItem>
class PathItem : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    PathItem(const QString& name, QGraphicsItem *parent = Q_NULLPTR);
    PathItem(const QString& name, const QPainterPath &path, QGraphicsItem *parent = Q_NULLPTR);
    virtual ~PathItem();
    QString getName() const;

signals:
    void hoverEnter(const QString& name, const QPointF& scenePos);
    void hoverLeave();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;

private:
    QString m_name;
};

#endif // PATHITEM_H
