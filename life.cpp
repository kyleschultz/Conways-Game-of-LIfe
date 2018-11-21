/*
Kyle Schultz and Liz Parker
Homework 4
life.cpp sets up methods for the Life class. We use objects of type life for each of our 250 Life boxes.
*/

#include "life.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QtWidgets>

Life::Life(int x, int y, QColor color)
{
    /*
        Initialize fields in the constructor
        @param x, y, and color are the initial values for Life fields
    */
    x_ = x;
    y_ = y;
    is_alive_ = false;
    this->color_ = color;
}

QRectF Life::boundingRect() const
{
    /*
        Builds a QRectF with xy position, length, and width determined by Life fields
        @return the built QRectF
    */
    return QRectF(x_, y_, width_, height_);
}

QPainterPath Life::shape() const
{
    /*
        Builds a QPainterPath and adds rectangle with xy position, length, and width determined by Life fields
        @return the built path
    */
    QPainterPath path;
    path.addRect(x_, y_, width_, height_);
    return path;
}

void Life::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /*
        Sets up QBrush and painter to paint UI as desired
        @param painter, option, and widget are QT objects that hold and perform our UI preferences
    */
    Q_UNUSED(widget);

    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_.dark(option->state & QStyle::State_Sunken ? 100 : 100)));

    painter->drawRect(QRect(this->x_, this->y_, this->width_, this->height_));
    painter->setBrush(b);
}

void Life::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    /*
        Routes event to emit signals
        @param is the event done by user that causes mousePressEvent to be called
    */
    if(event->buttons() == Qt::LeftButton)
    {
        emit SquareClicked(this->x_, this->y_);
    }
    else if(event->button() == Qt::RightButton)
    {
        emit SquareRightClicked(this->x_, this->y_);
    }

}
