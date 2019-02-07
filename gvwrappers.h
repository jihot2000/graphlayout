#ifndef GVWRAPPERS_H
#define GVWRAPPERS_H

#include <graphviz/gvc.h>
#include <QString>
#include <QDebug>

/// The agopen method for opening a graph
static inline Agraph_t* _agopen(const QString& name, const Agdesc_t& kind)
{
    return agopen(const_cast<char *>(qUtf8Printable(name)), kind, NULL);
}

/// Add an alternative value parameter to the method for getting an object's attribute
static inline QString _agget(void *object, const QString& attr, const QString& alt=QString())
{
    char * att = agget(object, const_cast<char *>(qUtf8Printable(attr)));
    if(!att)
    {
        qDebug() << QString("attribute %1 is not defined").arg(attr);
        return alt;
    }
    QString str = att;
    if(str.isEmpty())
    {
        qDebug() << QString("attribute %1 uses the default value").arg(attr);
        return alt;
    }
    else
        return str;
}

/// Directly use agsafeset which always works, contrarily to agset
static inline int _agset(void *object, const QString& attr, const QString& value)
{
    return agsafeset(object, const_cast<char *>(qUtf8Printable(attr)), const_cast<char *>(qUtf8Printable(value)), const_cast<char *>(qUtf8Printable(value)));
}

static inline Agsym_t * _agnodeattr(Agraph_t * g, const QString& name, const QString& value)
{
    return agattr(g, AGNODE, const_cast<char *>(qUtf8Printable(name)), const_cast<char *>(qUtf8Printable(value)));
}

static inline Agnode_t * _agnode(Agraph_t * g, const QString& name)
{
    return agnode(g, const_cast<char *>(qUtf8Printable(name)), 1);
}

static inline Agsym_t * _agedgeattr(Agraph_t * g, const QString& name, const QString& value)
{
    return agattr(g, AGEDGE, const_cast<char *>(qUtf8Printable(name)), const_cast<char *>(qUtf8Printable(value)));
}

static inline int _gvLayout(GVC_t * gvc, Agraph_t * g, const QString& engine)
{
    return gvLayout(gvc, g, qUtf8Printable(engine));
}
#endif // GVWRAPPERS_H
