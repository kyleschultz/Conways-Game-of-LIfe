/*
Kyle Schultz and Liz Parker
Homework 4
life.h holds our field and method declarations for the Life class
*/

#ifndef LIFE_H
#define LIFE_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QColor>
#include <QGraphicsItem>

class Life : public QObject, public QGraphicsItem {

    Q_OBJECT

public:
    Life(int x, int y, QColor color);
    bool is_alive() {return is_alive_;}
    void set_alive(bool alive) {is_alive_ = alive;}
    void setColor(QColor color) {color_ = color;}
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
signals:
    void SquareClicked(int x, int y);
    void SquareRightClicked(int x, int y);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
private:
    int x_;
    int y_;
    bool is_alive_;
    static const int width_ = 20;
    static const int height_ = 20;
    QColor color_;


};

#endif // LIFE_H
