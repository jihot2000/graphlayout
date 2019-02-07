#ifndef GVGRAPH_H
#define GVGRAPH_H

#include "gvwrappers.h"
#include <QFont>
#include <QMap>
#include <QRectF>
#include <QPainterPath>
#include <QColor>
#include <functional>

struct NodeAttributes
{
    enum NodeType {Ellipse, Rectangle};
    NodeAttributes()
        :type(Ellipse)
        ,penpixel(1.0)
        ,pencolor(Qt::black)
        ,fillcolor(Qt::white)
        ,fontcolor(Qt::black)
        ,font(QFont())
    {

    }
    NodeType type;
    qreal penpixel;
    QColor pencolor;
    QColor fillcolor;
    QColor fontcolor;
    QFont font;
};

struct EdgeAttributes
{
    EdgeAttributes()
        :penpixel(1.0)
        ,pencolor(Qt::black)
    {

    }
    qreal penpixel;
    QColor pencolor;
};

/// A struct containing the information for a GVGraph's node
struct GVNode
{
    /// The unique identifier of the node in the graph
    QString name;

    /// The position of the center point of the node from the top-left corner
    QPointF centerPos;

    /// The size of the node in pixels
    double height, width;

    NodeAttributes::NodeType type;
    qreal penpixel;
    QColor pencolor;
    QColor fillcolor;
    QColor fontcolor;
    QFont font;
};

/// A struct containing the information for a GVGraph's edge
struct GVEdge
{
    /// The source and target nodes of the edge
    QString source;
    QString target;

    /// Path of the edge's line
    QPainterPath path;

    qreal penpixel;
    QColor pencolor;
};

/// An object containing a libgraph graph and its associated nodes and edges
class GVGraph
{
public:
    /// Default DPI value used by dot (which uses points instead of pixels for coordinates)
    static const qreal DotDefaultDPI;

    /*!
     * \brief Construct a Graphviz graph object
     * \param name The name of the graph, must be unique in the application
     * \param font The font to use for the graph
     * \param node_size The size in pixels of each node
     */
    GVGraph(QString name);
    ~GVGraph();

    void setDefaultNodeAttributes(const NodeAttributes& attr);
    void setDefaultEdgeAttributes(const EdgeAttributes& attr);

    /// Add and remove nodes
    void addNode(const QString& name, const std::function<QString()>& hoverFunc);
    void setNodeAttributes(const QString& name, const NodeAttributes& attr);
//    void addNodes(const QStringList& names);
    void removeNode(const QString& name);
    void clearNodes();

    /// Add and remove edges
    void addEdge(const QString& source, const QString& target);
    void setEdgeaAttributes(const QString& src, const QString& target, const EdgeAttributes& attr);
    void removeEdge(const QString& source, const QString& target);
    void removeEdge(const QPair<QString, QString>& key);

    void setRootNode(const QString& name);
    QRectF boundingRect() const;

    QList<GVNode> nodes() const;
    QList<GVEdge> edges() const;
    std::function<QString()> getNodeHoverFunc(const QString& nodeName) const;

    void applyLayout(const char * method);

private:
    /// Set the font to use in all the labels
    void setFont(QFont font);

private:
    GVC_t *_context;
    Agraph_t *_graph;
    QFont _font;
    QMap<QString, QPair<Agnode_t*, std::function<QString()>>> _nodes;
    QMap<QPair<QString, QString>, Agedge_t*> _edges;
    bool _layoutApplied;
};

#endif // GVGRAPH_H
