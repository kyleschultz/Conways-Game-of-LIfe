/*
Kyle Schultz and Liz Parker
Homework 4
lifewindow.h holds our field and method declarations for the LifeWindow class
*/

#ifndef LIFEWINDOW_H
#define LIFEWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <vector>
#include <QObject>
#include <QTimer>
#include "life.h"

namespace Ui {
class LifeWindow;
}

class LifeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LifeWindow(QWidget *parent = nullptr);
    void StartGame(QGraphicsView * view);
    int CheckNeighbors(int i, int j);
    void TakeTurn();
    ~LifeWindow();

private slots:
    void on_resetbutton_clicked();

    void on_stepButton_clicked();

    void SquareClickedSlot(int x_, int y_);

    void SquareRightClickedSlot(int x_, int y_);

    void on_playButton_clicked();

    void on_pauseButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void Play();

private:
    Ui::LifeWindow *ui;

    QGraphicsScene *scene;

    static int turns_;

    static int alive_count_;
    static int numRecs_;
    static int percent_alive_;
    static int speed_;
    QTimer* timer_;

    std::vector<QGraphicsRectItem*> bars;
    void updateGraph();
    Life* matrix[10][25];

};

#endif // LIFEWINDOW_H
