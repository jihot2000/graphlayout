#include "pathitem.h"
#include <QGraphicsSceneHoverEvent>
#include <QDebug>

PathItem::PathItem(const QString& name, QGraphicsItem *parent)
    :QGraphicsPathItem(parent)
    ,m_name(name)
{
    this->setAcceptHoverEvents(true);
}

PathItem::PathItem(const QString& name, const QPainterPath &path, QGraphicsItem *parent)
    :QGraphicsPathItem(path, parent)
    ,m_name(name)
{
    this->setAcceptHoverEvents(true);
}

PathItem::~PathItem()
{

}

QString PathItem::getName() const
{
    return m_name;
}

void PathItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsPathItem::hoverEnterEvent(event);
    emit hoverEnter(m_name, event->scenePos());
}

void PathItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsPathItem::hoverEnterEvent(event);
    emit hoverLeave();
}

void PathItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsPathItem::hoverEnterEvent(event);
//    emit hover(m_name, event->scenePos());
}
