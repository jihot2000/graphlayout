#include "graphlayout.h"
#include <QPainterPath>
#include <QGraphicsPathItem>
#include "pathitem.h"
#include <QDebug>

GraphLayout::GraphLayout(QGraphicsView * view, QObject *parent)
    : QObject(parent)
    , m_scene(new QGraphicsScene(this))
    , m_graph(new GVGraph("graph"))
    , m_showHoverInfo(false)
    , m_hoverInfoItem(new QGraphicsRectItem())
    , m_hoverInfoTextItem(new TextItem(m_hoverInfoItem))
    , m_method(DOT)
{
    if(view)
    {
        m_view = view;
        m_view->setScene(m_scene);
    }
    else
    {
        m_view = nullptr;
        qDebug() << "[Warning] GraphLayout::GraphLayout# view is null";
    }
    if(m_view)
    {
        m_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    }
    m_hoverInfoItem->setVisible(false);
    m_hoverInfoItem->setBrush(QBrush(Qt::white));
    m_scene->addItem(m_hoverInfoItem);
    m_hoverInfoItem->setZValue(1);
}

GraphLayout::~GraphLayout()
{
    delete m_graph;
}

void GraphLayout::setLayoutMethod(LayoutMethod method)
{
    m_method = method;
}

void GraphLayout::setDefaultNodeAttributes(const NodeAttributes& attr)
{
    m_graph->setDefaultNodeAttributes(attr);
}

void GraphLayout::setDefaultEdgeAttributes(const EdgeAttributes& attr)
{
    m_graph->setDefaultEdgeAttributes(attr);
}

void GraphLayout::addNode(const QString& name, const NodeAttributes& attr, const std::function<QString()>& hoverFunc)
{
    m_graph->addNode(name, hoverFunc);
    m_graph->setNodeAttributes(name, attr);
}

void GraphLayout::addEdge(const QString& src, const QString& dst, const EdgeAttributes& attr)
{
    m_graph->addEdge(src, dst);
    m_graph->setEdgeaAttributes(src, dst, attr);
}

void GraphLayout::removeNode(const QString& name)
{
    m_graph->removeNode(name);
}

void GraphLayout::removeEdge(const QString& src, const QString& dst)
{
    m_graph->removeEdge(src, dst);
}

void GraphLayout::updateLayout()
{
    destroyAllGVItems();
    if(m_method == NEATO)
    {
        m_graph->applyLayout("neato");
    }
    else
    {
        m_graph->applyLayout("dot");
    }
//    QRectF sceneRect = m_graph->boundingRect();
//    qDebug() << "sceneRect: " << sceneRect;
//    scene->setSceneRect(sceneRect);
    for(auto it : m_graph->edges())
    {
//        qDebug() << "sourcename: " << it.source << ", targetname: " << it.target << ", path: \n" << it.path;
        QPen pen;
        pen.setColor(it.pencolor);
        pen.setWidthF(it.penpixel);
        qreal angle = it.path.angleAtPercent(1.0);
        QGraphicsPathItem * pathItem = new QGraphicsPathItem(it.path);
        pathItem->setPen(pen);
        addGVItem(pathItem);

        QPointF finalLocation = it.path.pointAtPercent(1.0);
        QPainterPath triPath(QPointF(0.0, 0.0));
        qreal alpha = 5;
        triPath.lineTo(alpha*1.732*it.penpixel, -alpha*0.5*it.penpixel);
        triPath.lineTo(alpha*1.732*it.penpixel, alpha*0.5*it.penpixel);
        triPath.lineTo(QPointF(0.0, 0.0));
        QGraphicsPathItem * triItem = new QGraphicsPathItem(triPath);
        triItem->setPen(pen);
        triItem->setBrush(QBrush(it.pencolor));
        triItem->setRotation(180-angle);
        triItem->moveBy(finalLocation.x(), finalLocation.y());
        addGVItem(triItem);
    }
    for(auto it : m_graph->nodes())
    {
//        qDebug() << "nodename: " << it.name << ", centerPos: " << it.centerPos << ", height: " << it.height << ", width: " << it.width;
        QPen pen;
        if(it.penpixel == 0)
        {
            pen = QPen(Qt::NoPen);
        }
        else
        {
            pen.setWidthF(it.penpixel);
            pen.setColor(it.pencolor);
        }
        QBrush brush;
        brush.setColor(it.fillcolor);
        brush.setStyle(Qt::SolidPattern);
        QPainterPath path;
        if(it.type == NodeAttributes::Ellipse)
        {
            path.addEllipse(it.centerPos.x()-it.width/2.0, it.centerPos.y()-it.height/2.0, it.width, it.height);
        }
        else
        {
            path.addRoundedRect(it.centerPos.x()-it.width/2.0, it.centerPos.y()-it.height/2.0, it.width, it.height, 5, 5);
        }
        PathItem * nodeItem = new PathItem(it.name, path);
        nodeItem->setPen(pen);
        nodeItem->setBrush(brush);
        connect(nodeItem, SIGNAL(hoverEnter(QString,QPointF)), this, SLOT(showHoverInfo(QString,QPointF)));
        connect(nodeItem, SIGNAL(hoverLeave()), this, SLOT(hideHoverInfo()));

        TextItem * textItem = new TextItem(it.name, nodeItem);
        textItem->setFont(it.font);
        textItem->setDefaultTextColor(it.fontcolor);
        textItem->setAlignment(Qt::AlignCenter);
        textItem->setPos(it.centerPos.x() - textItem->boundingRect().width()/2.0, it.centerPos.y() - textItem->boundingRect().height()/2.0);

        addGVItem(nodeItem);
    }
}

void GraphLayout::setHoverInfoVisible(bool visible)
{
    m_showHoverInfo = visible;
}

void GraphLayout::showHoverInfo(const QString& nodeName, const QPointF& scenePos)
{
    if(m_showHoverInfo)
    {
        auto hoverFunc = m_graph->getNodeHoverFunc(nodeName);
        QString text = hoverFunc();
        if(!text.isNull())
        {
            m_hoverInfoTextItem->setPlainText(hoverFunc());
            m_hoverInfoTextItem->setAlignment(Qt::AlignLeft);
            m_hoverInfoTextItem->updateGeometry();

            QRectF brRect = m_hoverInfoTextItem->boundingRect();
            m_hoverInfoItem->setRect(scenePos.x(), scenePos.y(), brRect.width()+2, brRect.height()+2);
            m_hoverInfoTextItem->setPos(scenePos.x()+1, scenePos.y()+1);

            m_hoverInfoItem->setVisible(true);
        }
    }
}

void GraphLayout::hideHoverInfo()
{
    if(m_showHoverInfo)
    {
        m_hoverInfoItem->setVisible(false);
    }
}

void GraphLayout::addGVItem(QGraphicsItem * item)
{
    m_scene->addItem(item);
    m_gvItemVector.append(item);
}

void GraphLayout::destroyAllGVItems()
{
    for(auto it : m_gvItemVector)
        delete it;
    m_gvItemVector.clear();
}

