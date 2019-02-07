#include "gvgraph.h"
#include <QDebug>

/*! Dot uses a 72 DPI value for converting it's position coordinates from points to pixels while we display at 96 DPI on most operating systems. */
const qreal GVGraph::DotDefaultDPI=72.0;

GVGraph::GVGraph(QString name)
    : _context(gvContext())
    , _graph(_agopen(name, Agstrictdirected))
    , _layoutApplied(false)
{
//    //Set graph attributes
    _agset(_graph, "overlap", "prism");
    _agset(_graph, "splines", "true");
//    _agset(_graph, "pad", "0.2");
    _agset(_graph, "dpi", "96.0");
//    _agset(_graph, "margin", "0.4");
//    _agset(_graph, "nodesep", "5");
    _agset(_graph, "sep", "+20");

//    //Set default attributes for the future nodes
//    _agnodeattr(_graph, "fixedsize", "true");
//    _agnodeattr(_graph, "label", "");
//    _agnodeattr(_graph, "regular", "true");

//    //Divide the wanted width by the DPI to get the value in points
//    QString nodePtsWidth = QString("%1").arg(node_size/_agget(_graph, "dpi", "96.0").toDouble());
//    //GV uses , instead of . for the separator in floats
//    _agnodeattr(_graph, "width", nodePtsWidth.replace('.', ","));
}

GVGraph::~GVGraph()
{
    gvFreeLayout(_context, _graph);
    agclose(_graph);
    gvFreeContext(_context);
}

void GVGraph::setDefaultNodeAttributes(const NodeAttributes& attr)
{
    if(_layoutApplied)
    {
        gvFreeLayout(_context, _graph);
        _layoutApplied = false;
    }

    if(attr.type == NodeAttributes::Ellipse)
    {
        _agnodeattr(_graph, "shape", "ellipse");
    }
    else
    {
        _agnodeattr(_graph, "shape", "rectangle");
    }

    qreal dpi=_agget(_graph, "dpi", "96.0").toDouble();
    _agnodeattr(_graph, "penwidth", QString("%1").arg(attr.penpixel*(DotDefaultDPI/dpi)));
    _agnodeattr(_graph, "color", attr.pencolor.name());
    _agnodeattr(_graph, "fillcolor", attr.fillcolor.name());
    _agnodeattr(_graph, "fontcolor", attr.fontcolor.name());
    _agnodeattr(_graph, "fontname", attr.font.family());
    _agnodeattr(_graph, "fontsize", QString("%1").arg(attr.font.pointSizeF()));
    _agnodeattr(_graph, "style", "filled");
}

void GVGraph::setDefaultEdgeAttributes(const EdgeAttributes& attr)
{
    if(_layoutApplied)
    {
        gvFreeLayout(_context, _graph);
        _layoutApplied = false;
    }

    qreal dpi=_agget(_graph, "dpi", "96.0").toDouble();
    _agedgeattr(_graph, "penwidth", QString("%1").arg(attr.penpixel*(DotDefaultDPI/dpi)));
    _agedgeattr(_graph, "color", attr.pencolor.name());
}

void GVGraph::addNode(const QString& name, const std::function<QString()>& hoverFunc)
{
    if(_layoutApplied)
    {
        gvFreeLayout(_context, _graph);
        _layoutApplied = false;
    }
    if(_nodes.contains(name))
        removeNode(name);

    _nodes.insert(name, QPair<Agnode_t*, std::function<QString()>>(_agnode(_graph, name), hoverFunc));
}

//void GVGraph::addNodes(const QStringList& names)
//{
//    for(int i=0; i<names.size(); ++i)
//        addNode(names.at(i));
//}

void GVGraph::setNodeAttributes(const QString& name, const NodeAttributes& attr)
{
    if(_layoutApplied)
    {
        gvFreeLayout(_context, _graph);
        _layoutApplied = false;
    }

    Agnode_t * node = agnode(_graph, const_cast<char *>(qUtf8Printable(name)), 0);
    if(node)
    {
        if(attr.type == NodeAttributes::Ellipse)
        {
            _agset(node, "shape", "ellipse");
        }
        else
        {
            _agset(node, "shape", "rectangle");
        }

        qreal dpi=_agget(_graph, "dpi", "96.0").toDouble();
        _agset(node, "penwidth", QString("%1").arg(attr.penpixel*(DotDefaultDPI/dpi)));
        _agset(node, "color", attr.pencolor.name());
        _agset(node, "fillcolor", attr.fillcolor.name());
        _agset(node, "fontcolor", attr.fontcolor.name());
        _agset(node, "fontname", attr.font.family());
        _agset(node, "fontsize", QString("%1").arg(attr.font.pointSizeF()));
        _agset(node, "style", "filled");
    }
}

void GVGraph::removeNode(const QString& name)
{
    if(_layoutApplied)
    {
        gvFreeLayout(_context, _graph);
        _layoutApplied = false;
    }

    if(_nodes.contains(name))
    {
        agdelete(_graph, _nodes[name].first);
        _nodes.remove(name);

        QList<QPair<QString, QString> >keys=_edges.keys();
        for(int i=0; i<keys.size(); ++i)
            if(keys.at(i).first==name || keys.at(i).second==name)
                removeEdge(keys.at(i));
    }
}

void GVGraph::clearNodes()
{
    if(_layoutApplied)
    {
        gvFreeLayout(_context, _graph);
        _layoutApplied = false;
    }

    QList<QString> keys=_nodes.keys();

    for(int i=0; i<keys.size(); ++i)
        removeNode(keys.at(i));
}

void GVGraph::setRootNode(const QString& name)
{
    if(_layoutApplied)
    {
        gvFreeLayout(_context, _graph);
        _layoutApplied = false;
    }

    if(_nodes.contains(name))
        _agset(_graph, "root", name);
}

void GVGraph::addEdge(const QString &source, const QString &target)
{
    if(_layoutApplied)
    {
        gvFreeLayout(_context, _graph);
        _layoutApplied = false;
    }

    if(_nodes.contains(source) && _nodes.contains(target))
    {
        QPair<QString, QString> key(source, target);
        if(!_edges.contains(key))
            _edges.insert(key, agedge(_graph, _nodes[source].first, _nodes[target].first, const_cast<char *>(""), 1));
    }
}

void GVGraph::setEdgeaAttributes(const QString& src, const QString& target, const EdgeAttributes& attr)
{
    if(_layoutApplied)
    {
        gvFreeLayout(_context, _graph);
        _layoutApplied = false;
    }

    Agnode_t * node1 = agnode(_graph, const_cast<char *>(qUtf8Printable(src)), 0);
    Agnode_t * node2 = agnode(_graph, const_cast<char *>(qUtf8Printable(target)), 0);
    if(node1 && node2)
    {
        Agedge_t * edge = agedge(_graph, node1, node2, const_cast<char *>(""), 0);
        if(edge)
        {
            qreal dpi=_agget(_graph, "dpi", "96.0").toDouble();
            _agset(edge, "penwidth", QString("%1").arg(attr.penpixel*(DotDefaultDPI/dpi)));
            _agset(edge, "color", attr.pencolor.name());
        }
    }

}

void GVGraph::removeEdge(const QString &source, const QString &target)
{
    if(_layoutApplied)
    {
        gvFreeLayout(_context, _graph);
        _layoutApplied = false;
    }

    removeEdge(QPair<QString, QString>(source, target));
}

void GVGraph::removeEdge(const QPair<QString, QString>& key)
{
    if(_layoutApplied)
    {
        gvFreeLayout(_context, _graph);
        _layoutApplied = false;
    }

    if(_edges.contains(key))
    {
        agdelete(_graph, _edges[key]);
        _edges.remove(key);
    }
}

void GVGraph::setFont(QFont font)
{
    if(_layoutApplied)
    {
        gvFreeLayout(_context, _graph);
        _layoutApplied = false;
    }

    _font=font;

    _agset(_graph, "fontname", font.family());
    _agset(_graph, "fontsize", QString("%1").arg(font.pointSizeF()));

    _agnodeattr(_graph, "fontname", font.family());
    _agnodeattr(_graph, "fontsize", QString("%1").arg(font.pointSizeF()));

    _agedgeattr(_graph, "fontname", font.family());
    _agedgeattr(_graph, "fontsize", QString("%1").arg(font.pointSizeF()));
}

void GVGraph::applyLayout(const char * method)
{
    if(_layoutApplied)
    {
        gvFreeLayout(_context, _graph);
        _layoutApplied = false;
    }
    _gvLayout(_context, _graph, method);
    _layoutApplied = true;
//    gvRenderFilename(_context, _graph, "png", "test01.png");
}

QRectF GVGraph::boundingRect() const
{
    qreal dpi=_agget(_graph, "dpi", "96.0").toDouble();
//    qDebug() << "dpi: " << dpi;
    return QRectF(GD_bb(_graph).LL.x*(dpi/DotDefaultDPI), GD_bb(_graph).LL.y*(dpi/DotDefaultDPI), GD_bb(_graph).UR.x*(dpi/DotDefaultDPI), GD_bb(_graph).UR.y*(dpi/DotDefaultDPI));
}

QList<GVNode> GVGraph::nodes() const
{
    QList<GVNode> list;
    qreal dpi=_agget(_graph, "dpi", "96.0").toDouble();

    for(auto it=_nodes.begin(); it!=_nodes.end();++it)
    {
        Agnode_t *node=it.value().first;
        GVNode object;

        QString nodetype = _agget(node, "shape", "ellipse");
        if(nodetype == "ellipse")
            object.type = NodeAttributes::Ellipse;
        else
            object.type = NodeAttributes::Rectangle;
        object.penpixel = _agget(node, "penwidth", QString("%1").arg(1.0*(DotDefaultDPI/dpi))).toDouble()*(dpi/DotDefaultDPI);;
        object.pencolor = QColor(_agget(node, "color", QColor(Qt::black).name()));;
        object.fillcolor = QColor(_agget(node, "fillcolor", QColor(Qt::white).name()));;
        object.fontcolor = QColor(_agget(node, "fontcolor", QColor(Qt::black).name()));;
        object.font = QFont(_agget(node, "fontname", QFont().family()), _agget(node, "fontsize", QString("%1").arg(QFont().pointSizeF())).toInt());

        //Set the name of the node
        object.name = ND_label(node)->text;

        //Fetch the X coordinate, apply the DPI conversion rate (actual DPI / 72, used by dot)
        qreal x=ND_coord(node).x*(dpi/DotDefaultDPI);

        //Translate the Y coordinate from bottom-left to top-left corner
        qreal y=(GD_bb(_graph).UR.y - ND_coord(node).y)*(dpi/DotDefaultDPI);
        object.centerPos=QPointF(x, y);

        //Transform the width and height from inches to pixels
        object.height=ND_height(node)*dpi;
        object.width=ND_width(node)*dpi;

        list << object;
    }

    return list;
}

QList<GVEdge> GVGraph::edges() const
{
    QList<GVEdge> list;
    qreal dpi=_agget(_graph, "dpi", "96.0").toDouble();

    for(auto it=_edges.begin(); it!=_edges.end();++it)
    {
        Agedge_t *edge=it.value();
        GVEdge object;

        object.penpixel = _agget(edge, "penwidth", QString("%1").arg(1.0*(DotDefaultDPI/dpi))).toDouble()*(dpi/DotDefaultDPI);
        object.pencolor = QColor(_agget(edge, "color", QColor(Qt::black).name()));

        //Fill the source and target node names
        object.source = ND_label(agtail(edge))->text;
        object.target = ND_label(aghead(edge))->text;
//        object.source = AGDATA(agtail(edge))->name;
//        object.target = AGDATA(aghead(edge))->name;

        //Calculate the path from the spline (only one spline, as the graph is strict. If it
        //wasn't, we would have to iterate over the first list too)
        //Calculate the path from the spline (only one as the graph is strict)
        if((ED_spl(edge)->list!=0) && (ED_spl(edge)->list->size%3 == 1))
        {
            //If there is a starting point, draw a line from it to the first curve point
            if(ED_spl(edge)->list->sflag)
            {
                object.path.moveTo(ED_spl(edge)->list->sp.x*(dpi/DotDefaultDPI), (GD_bb(_graph).UR.y - ED_spl(edge)->list->sp.y)*(dpi/DotDefaultDPI));
                object.path.lineTo(ED_spl(edge)->list->list[0].x*(dpi/DotDefaultDPI), (GD_bb(_graph).UR.y - ED_spl(edge)->list->list[0].y)*(dpi/DotDefaultDPI));
            }
            else
                object.path.moveTo(ED_spl(edge)->list->list[0].x*(dpi/DotDefaultDPI), (GD_bb(_graph).UR.y - ED_spl(edge)->list->list[0].y)*(dpi/DotDefaultDPI));

            //Loop over the curve points
            for(int i=1; i<ED_spl(edge)->list->size; i+=3)
                object.path.cubicTo(ED_spl(edge)->list->list[i].x*(dpi/DotDefaultDPI), (GD_bb(_graph).UR.y - ED_spl(edge)->list->list[i].y)*(dpi/DotDefaultDPI), ED_spl(edge)->list->list[i+1].x*(dpi/DotDefaultDPI), (GD_bb(_graph).UR.y - ED_spl(edge)->list->list[i+1].y)*(dpi/DotDefaultDPI), ED_spl(edge)->list->list[i+2].x*(dpi/DotDefaultDPI), (GD_bb(_graph).UR.y - ED_spl(edge)->list->list[i+2].y)*(dpi/DotDefaultDPI));

            //If there is an ending point, draw a line to it
            if(ED_spl(edge)->list->eflag)
                object.path.lineTo(ED_spl(edge)->list->ep.x*(dpi/DotDefaultDPI), (GD_bb(_graph).UR.y - ED_spl(edge)->list->ep.y)*(dpi/DotDefaultDPI));
        }

        list << object;
    }
    return list;
}

std::function<QString()> GVGraph::getNodeHoverFunc(const QString& nodeName) const
{
    if(_nodes.contains(nodeName))
        return _nodes[nodeName].second;
    else
        return []()->QString{return QString();};
}
