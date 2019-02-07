#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QTextDocument>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QGraphicsTextItem>

class TextItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 1 };

    TextItem(QGraphicsItem* parent = 0);
    TextItem(const QString& text, QGraphicsItem* parent = 0);
    void setAlignment(Qt::Alignment alignment);
    virtual int type() const;

//protected:
//    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
//    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
//    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;

public slots:
    void updateGeometry(int, int, int);
    void updateGeometry();

private:
    void init();

private:
    Qt::Alignment alignment_;
};

#endif // TEXTITEM_H
