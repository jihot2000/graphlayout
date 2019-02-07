#include "textitem.h"
#include <QGraphicsSceneHoverEvent>
#include <QDebug>

TextItem::TextItem(QGraphicsItem* parent)
    : QGraphicsTextItem(parent)
    , alignment_(Qt::AlignLeft)
{
    init();
    this->setAcceptHoverEvents(false);
}

TextItem::TextItem(const QString& text, QGraphicsItem* parent)
    : QGraphicsTextItem(text, parent)
    , alignment_(Qt::AlignLeft)
{
    init();
    this->setAcceptHoverEvents(false);
}

void TextItem::init()
{
    updateGeometry();
    connect(document(), SIGNAL(contentsChange(int, int, int)),
            this, SLOT(updateGeometry(int, int, int)));
}

//void TextItem::setAlignment(Qt::Alignment alignment)
//{
//    alignment_ = alignment;
//    QTextBlockFormat format;
//    format.setAlignment(alignment);
//    QTextCursor cursor = textCursor();
//    cursor.select(QTextCursor::Document);
//    cursor.mergeBlockFormat(format);
//    cursor.clearSelection();
//    setTextCursor(cursor);
//}

void TextItem::updateGeometry(int, int, int)
{
    updateGeometry();
}

//void TextItem::updateGeometry()
//{
//    setTextWidth(-1);
//    setTextWidth(boundingRect().width());
//    setAlignment(alignment_);
//}

int TextItem::type() const
{
    return Type;
}

void TextItem::updateGeometry()
{
    QPointF topRightPrev = boundingRect().topRight();
    setTextWidth(-1);
    setTextWidth(boundingRect().width());
    setAlignment(alignment_);
    QPointF topRight = boundingRect().topRight();

    if (alignment_ & Qt::AlignRight)
    {
        setPos(pos() + (topRightPrev - topRight));
    }
}

void TextItem::setAlignment(Qt::Alignment alignment)
{
    alignment_ = alignment;
    QTextBlockFormat format;
    format.setAlignment(alignment);
    QTextCursor cursor = textCursor();      // save cursor position
    int position = textCursor().position();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    cursor.setPosition(position);           // restore cursor position
    setTextCursor(cursor);
}

//void TextItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
//{
//    QGraphicsTextItem::hoverEnterEvent(event);
//}

//void TextItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
//{
//    QGraphicsTextItem::hoverEnterEvent(event);
//}

//void TextItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
//{
//    QGraphicsTextItem::hoverEnterEvent(event);
//    qDebug() << "text item" << event->lastScenePos();
//}
