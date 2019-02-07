#ifndef GRAPHLAYOUT_H
#define GRAPHLAYOUT_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointer>
#include <QTimer>
#include <functional>
#include "gvgraph.h"
#include "textitem.h"

//struct
class GraphLayout : public QObject
{
    Q_OBJECT
public:
    enum LayoutMethod { DOT, NEATO };
    explicit GraphLayout(QGraphicsView * view, QObject *parent = nullptr);
    virtual ~GraphLayout();
    void setLayoutMethod(LayoutMethod method);
    void setDefaultNodeAttributes(const NodeAttributes& attr);
    void setDefaultEdgeAttributes(const EdgeAttributes& attr);
    void addNode(const QString& name, const NodeAttributes& attr, const std::function<QString()>& hoverFunc = []()->QString{return QString();});
    void addEdge(const QString& src, const QString& dst, const EdgeAttributes& attr);
    void removeNode(const QString& name);
    void removeEdge(const QString& src, const QString& dst);
    void setHoverInfoVisible(bool visible);

private:
    void addGVItem(QGraphicsItem * item);
    void destroyAllGVItems();

signals:

public slots:
    void updateLayout();

private slots:
    void showHoverInfo(const QString& nodeName, const QPointF& scenePos);
    void hideHoverInfo();

private:
    QPointer<QGraphicsView> m_view;
    QGraphicsScene * m_scene;
    GVGraph * m_graph;
    bool m_showHoverInfo;
    QGraphicsRectItem * m_hoverInfoItem;
    TextItem * m_hoverInfoTextItem;
    QVector<QGraphicsItem *> m_gvItemVector;
    LayoutMethod m_method;
};

#endif // GRAPHLAYOUT_H
