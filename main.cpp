#include <QApplication>
#include <QGraphicsView>
#include <QDebug>
#include "graphlayout.h"
#include <QTimer>

//int main()
//{
//    Agraph_t * G;
//    GVC_t * gvc;
//    //Create Context
//    gvc = gvContext();

//    //Create Graph
//    //G = createGraph();
//    //TODO:...
//    const char * graphname = "network";
//    G = agopen(const_cast<char *>(graphname), Agstrictdirected, NULL);

//    const char * nodename1 = "node1";
//    const char * nodename2 = "node2";
//    Agnode_t * node1 = agnode(G, const_cast<char *>(nodename1), 1);
//    Agnode_t * node2 = agnode(G, const_cast<char *>(nodename2), 1);

//    const char * edgename = "edge_1_to_2";
//    Agedge_t * edge = agedge(G, node1, node2, const_cast<char *>(edgename), 1);

//    //set default node color
//    agattr(G, AGNODE, "color", "blue");
//    agattr(G, AGEDGE, "penwidth", "3");

//    //set the color of node 2 to red
//    printf("%d\n", agset(node2, "color", "red"));
//    printf("%d\n", agset(node1, "label", "node1 label"));
//    printf("%d\n", agset(node2, "label", "node2 标签"));

//    //printf("%d\n", agset(edge, "penwidth", "72"));

//    gvLayout(gvc, G, "dot");

//    //Draw Graph
//    //drawGraph(G);
//    //TODO:...
//    gvRenderFilename(gvc, G, "png", "gvtest01.png");

//    gvFreeLayout(gvc, G);
//    agclose(G);

//    //Free Context
//    gvFreeContext(gvc);
//}

//int main(int argc, char **argv)
//{
//    QApplication a(argc, argv);

//    QGraphicsView * view = new QGraphicsView();
//    view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

//    GraphLayout * graphlayout = new GraphLayout(view);
//    graphlayout->setLayoutMethod(GraphLayout::NEATO);
//    NodeAttributes nodeDefaultAttributes;
//    EdgeAttributes edgeDefaultAttributes;
//    NodeAttributes node2attr;
//    node2attr.pencolor = Qt::red;
//    node2attr.fontcolor = Qt::cyan;
//    QFont font;
//    font.setPointSize(32);
//    node2attr.font = font;
//    NodeAttributes node1attr;
//    node1attr.type = NodeAttributes::Rectangle;
//    node1attr.penpixel = 3;
//    node1attr.fillcolor = Qt::yellow;
//    EdgeAttributes edgeAttr;
//    edgeAttr.pencolor = Qt::blue;
//    edgeAttr.penpixel = 3;
//    graphlayout->setDefaultNodeAttributes(nodeDefaultAttributes);
//    graphlayout->setDefaultEdgeAttributes(edgeDefaultAttributes);
//    graphlayout->addNode("节点1\nwindows", node1attr, []{return "我是节点1";});
//    graphlayout->addNode("节点2.1", node2attr);
//    graphlayout->addNode("节点2.2", node2attr);
//    graphlayout->addNode("节点3", nodeDefaultAttributes);
//    graphlayout->addNode("节点4", nodeDefaultAttributes);
//    graphlayout->addEdge("节点1\nwindows", "节点2.1", edgeDefaultAttributes);
//    graphlayout->addEdge("节点1\nwindows", "节点2.2", edgeDefaultAttributes);
//    graphlayout->addEdge("节点2.1", "节点3", edgeDefaultAttributes);
//    graphlayout->addEdge("节点2.2", "节点3", edgeDefaultAttributes);
//    graphlayout->addEdge("节点1\nwindows", "节点3", edgeAttr);
//    graphlayout->updateLayout();
//    graphlayout->setHoverInfoVisible(true);
//    QTimer::singleShot(3000, [&](){
//        graphlayout->addNode("节点5", nodeDefaultAttributes);
//        graphlayout->addEdge("节点3", "节点5", edgeDefaultAttributes);
//        graphlayout->updateLayout();
//    });

//    view->show();
//    return a.exec();
//}

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    QGraphicsView * view = new QGraphicsView();
    view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    GraphLayout * graphlayout = new GraphLayout(view);
    graphlayout->setLayoutMethod(GraphLayout::DOT);
    NodeAttributes nodeDefaultAttributes;
    EdgeAttributes edgeDefaultAttributes;
    NodeAttributes node2attr;
    node2attr.pencolor = Qt::red;
    node2attr.fontcolor = Qt::cyan;
    QFont font;
    font.setPointSize(32);
    node2attr.font = font;
    NodeAttributes node1attr;
    node1attr.type = NodeAttributes::Rectangle;
    node1attr.penpixel = 3;
    node1attr.fillcolor = Qt::yellow;
    EdgeAttributes edgeAttr;
    edgeAttr.pencolor = Qt::blue;
    edgeAttr.penpixel = 3;
    graphlayout->setDefaultNodeAttributes(nodeDefaultAttributes);
    graphlayout->setDefaultEdgeAttributes(edgeDefaultAttributes);

    graphlayout->addNode("ClientRemotePipelineInterface", nodeDefaultAttributes);
    graphlayout->addNode("ClientRemoteStubInterface", nodeDefaultAttributes);
    graphlayout->addNode("CallPipeline", nodeDefaultAttributes);
    graphlayout->addNode("ClientCallBackStep", nodeDefaultAttributes);
    graphlayout->addNode("ClientLocalSyncStubInterface", nodeDefaultAttributes);
    graphlayout->addNode("ClientLocalSyncStubInterfaceBase", nodeDefaultAttributes);
    graphlayout->addNode("ClientCallForwardStep", nodeDefaultAttributes);
    graphlayout->addNode("ClientLocalAsyncStubInterface", nodeDefaultAttributes);
    graphlayout->addNode("ClientSignalEmitter", nodeDefaultAttributes);

    graphlayout->addEdge("CallPipeline", "ClientRemotePipelineInterface", edgeDefaultAttributes);
    graphlayout->addEdge("ClientRemoteStubInterface", "ClientRemotePipelineInterface", edgeDefaultAttributes);
    graphlayout->addEdge("ClientCallBackStep", "ClientRemoteStubInterface", edgeDefaultAttributes);
    graphlayout->addEdge("ClientLocalSyncStubInterfaceBase", "ClientLocalSyncStubInterface", edgeDefaultAttributes);
    graphlayout->addEdge("ClientCallForwardStep", "ClientLocalSyncStubInterfaceBase", edgeDefaultAttributes);
    graphlayout->addEdge("ClientCallForwardStep", "ClientLocalAsyncStubInterface", edgeDefaultAttributes);
    graphlayout->addEdge("ClientLocalAsyncStubInterface", "ClientSignalEmitter", edgeDefaultAttributes);

    graphlayout->updateLayout();
    graphlayout->setHoverInfoVisible(true);

    view->show();
    return a.exec();
}
