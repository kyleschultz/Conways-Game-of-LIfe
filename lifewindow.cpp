/*
Kyle Schultz and Liz Parker
Homework 4
lifewindow.cpp is the model for Conway's Game of Life. It controls all objects and UI.
lifewindow.cpp also sets up methods for the LifeWindow class.
*/


#include "lifewindow.h"
#include "ui_lifewindow.h"
#include <QDebug>


//initialize static fields
int LifeWindow::turns_ = 0;
int LifeWindow::alive_count_ = 0;
int LifeWindow::percent_alive_ = 0;
int LifeWindow::numRecs_ = 0;
int LifeWindow::speed_ = 100;

LifeWindow::LifeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LifeWindow)
{
    /*
        LifeWindow constructor sets up UI, timer, and boxes of life
        @param *parent is the QWidget used in our UI
    */
    timer_ = new QTimer(parent = nullptr);
    scene = new QGraphicsScene;
    ui->setupUi(this);
    QGraphicsView * view = ui->lifeGraphicsView;
    view->setScene(scene);
    view->setSceneRect(0,0,view->frameSize().width(),view->frameSize().height());
    srand(time(0));
    connect(timer_, SIGNAL(timeout()), this, SLOT(Play()));
    QColor color(255, 255, 255);
    //qDebug() << view->frameSize().width();
    //qDebug() << view->frameSize().height();
    int row = 0;
    int col = 0;
    for(int j = 10; j < 210; j+=20)
    {
       for(int i = 50; i < 550; i+=20)
       {
           //create new box of "life"
           Life * s = new Life(i,j,color);
           scene->addItem(s);
           //connect slots for click and right click
           connect(s, &Life::SquareClicked, this, &LifeWindow::SquareClickedSlot);
           connect(s, &Life::SquareRightClicked, this, &LifeWindow::SquareRightClickedSlot);
           matrix[row][col] = s;
           col++;
       }
       row++;
       col = 0;
    }
    scene->addRect(50, 215, 500 ,100);

    StartGame(view);
}

void LifeWindow::StartGame(QGraphicsView * view)
{
    /*
        Sets up life boxes first random orientation, corresponding labels, and graph
        @param view is the QGraphicsView used in our UI
    */
    QTransform t;
    alive_count_ = 0;
    turns_ = 0;
    for(int j = 10; j < 210; j+=20)
    {
       for(int i = 50; i < 550; i+=20)
       {
           QPointF p(i,j);
           QGraphicsItem * curr = scene->itemAt(p,t);
           Life * current = dynamic_cast<Life *>(curr);
           int alive = rand() % 2;
           current->set_alive(alive);
           if(current->is_alive())
           {
               int red = rand() % 256;
               int blue = rand() % 256;
               int green = rand() % 256;
               QColor c(red,green,blue);
               current->setColor(c);
               alive_count_++;
           }
           else
           {
               QColor c(255,255,255);
               current->setColor(c);
           }

       }
    }
    //qDebug() << alive_count_;
    percent_alive_ = int((100*alive_count_)/250);
    //qDebug() << "Percent alive:" << percent_alive_;
    int h = 215+percent_alive_;
    QGraphicsRectItem* temp = new QGraphicsRectItem(50, h,20,100-percent_alive_);
    bars.push_back(temp);
    numRecs_++;
    scene->addItem(temp);
    ui->popLabel->setText(QString("Population: ") + QString(std::to_string(alive_count_).c_str()) + QString("  (") + QString(std::to_string(percent_alive_).c_str()) + QString("%)"));
    ui->turnLabel->setText(QString("Turn: ") + QString(std::to_string(turns_).c_str()));
    update();
}

LifeWindow::~LifeWindow()
{
    //deconstructor
    delete ui;
}

void LifeWindow::on_resetbutton_clicked()
{
    /*
        Cleans up UI, resets some variables, and starts a new game
    */
    timer_->stop();
    for (int i = 0; i < numRecs_;i++){
        scene->removeItem(bars[i]);
        bars.pop_back();
    }
    numRecs_ = 0;
    QGraphicsView * view = ui->lifeGraphicsView;
    StartGame(view);
    view->scene()->update();
    repaint();
}

void LifeWindow::SquareClickedSlot(int x, int y)
{
    /*
        Changes dead square to alive and updates UI
        @param x and y are coordinate values of click
    */
    QTransform t;
    QPointF p(x,y);
    QGraphicsView * view = ui->lifeGraphicsView;
    QGraphicsItem * curr = view->scene()->itemAt(p,t);
    Life * current = dynamic_cast<Life *>(curr);

    if(!current->is_alive())
    {
        current->set_alive(true);
        int red = rand() % 256;
        int blue = rand() % 256;
        int green = rand() % 256;
        QColor c(red,green,blue);
        current->setColor(c);
        alive_count_++;
        percent_alive_ = int((100*alive_count_)/250);
        ui->popLabel->setText(QString("Population: ") + QString(std::to_string(alive_count_).c_str()) + QString("  (") + QString(std::to_string(percent_alive_).c_str()) + QString("%)"));
        view->scene()->update();
        repaint();
    }

}

void LifeWindow::SquareRightClickedSlot(int x, int y)
{
    /*
        Changes alive square to dead and updates UI
        @param are x and y are coordinate values of click
    */
    QTransform t;
    QPointF p(x,y);
    QGraphicsView * view = ui->lifeGraphicsView;
    QGraphicsItem * curr = view->scene()->itemAt(p,t);
    Life * current = dynamic_cast<Life *>(curr);

    if(current->is_alive())
    {
        current->set_alive(false);
        QColor c(255,255,255);
        current->setColor(c);
        alive_count_--;
        percent_alive_ = int((100*alive_count_)/250);
        ui->popLabel->setText(QString("Population: ") + QString(std::to_string(alive_count_).c_str()) + QString("  (") + QString(std::to_string(percent_alive_).c_str()) + QString("%)"));
        view->scene()->update();
        repaint();
    }
}

int LifeWindow::CheckNeighbors(int i, int j)
{
    /*
        helper function for TakeTurn()
        updates alive count by looking at corners, edges, and surrounded life boxes
        @param i and j are row and column value of Life box being checked's position in matrix
        @return updated alive_count_
    */
    int alive_count = 0;
    if(i == 0 && j == 0) //top left corner case
    {
        alive_count += matrix[1][0]->is_alive();
        alive_count += matrix[0][1]->is_alive();
        alive_count += matrix[1][1]->is_alive();
        alive_count += matrix[9][0]->is_alive();
        alive_count += matrix[9][1]->is_alive();
        alive_count += matrix[9][24]->is_alive();
        alive_count += matrix[0][24]->is_alive();
        alive_count += matrix[1][24]->is_alive();
    }
    else if(i == 9 && j ==0) //bottom left corner case
    {
        alive_count += matrix[0][0]->is_alive();
        alive_count += matrix[0][1]->is_alive();
        alive_count += matrix[8][1]->is_alive();
        alive_count += matrix[8][0]->is_alive();
        alive_count += matrix[9][1]->is_alive();
        alive_count += matrix[0][24]->is_alive();
        alive_count += matrix[9][24]->is_alive();
        alive_count += matrix[8][24]->is_alive();
    }
    else if(i == 0 && j == 24) //top right corner case
    {
        alive_count += matrix[1][24]->is_alive();
        alive_count += matrix[0][23]->is_alive();
        alive_count += matrix[1][23]->is_alive();
        alive_count += matrix[9][23]->is_alive();
        alive_count += matrix[9][24]->is_alive();
        alive_count += matrix[9][0]->is_alive();
        alive_count += matrix[0][0]->is_alive();
        alive_count += matrix[1][0]->is_alive();
    }
    else if(i == 9 && j == 24) //bottom right corner case
    {
        alive_count += matrix[0][0]->is_alive();
        alive_count += matrix[8][1]->is_alive();
        alive_count += matrix[9][0]->is_alive();
        alive_count += matrix[0][23]->is_alive();
        alive_count += matrix[0][24]->is_alive();
        alive_count += matrix[9][23]->is_alive();
        alive_count += matrix[8][23]->is_alive();
        alive_count += matrix[8][24]->is_alive();
    }
    else if(i == 0) //left edge of grid, excluding corner
    {
        alive_count += matrix[i][j+1]->is_alive();
        alive_count += matrix[i][j-1]->is_alive();
        alive_count += matrix[i+1][j]->is_alive();
        alive_count += matrix[i+1][j-1]->is_alive();
        alive_count += matrix[i+1][j+1]->is_alive();
        alive_count += matrix[9][j]->is_alive();
        alive_count += matrix[9][j-1]->is_alive();
        alive_count += matrix[9][j+1]->is_alive();
    }
    else if(i == 9) //right edge of grid, excluding corner
    {
        alive_count += matrix[i][j+1]->is_alive();
        alive_count += matrix[i][j-1]->is_alive();
        alive_count += matrix[i-1][j]->is_alive();
        alive_count += matrix[i-1][j-1]->is_alive();
        alive_count += matrix[i-1][j+1]->is_alive();
        alive_count += matrix[0][j]->is_alive();
        alive_count += matrix[0][j-1]->is_alive();
        alive_count += matrix[0][j+1]->is_alive();
    }
    else if(j == 0) //top edge of grid, excluding corner
    {
        alive_count += matrix[i-1][j]->is_alive();
        alive_count += matrix[i+1][j]->is_alive();
        alive_count += matrix[i-1][j+1]->is_alive();
        alive_count += matrix[i][j+1]->is_alive();
        alive_count += matrix[i+1][j+1]->is_alive();
        alive_count += matrix[i-1][24]->is_alive();
        alive_count += matrix[i][24]->is_alive();
        alive_count += matrix[i+1][24]->is_alive();
    }
    else if(j == 24) //right edge of grid excluding corner
    {
        alive_count += matrix[i-1][j]->is_alive();
        alive_count += matrix[i+1][j]->is_alive();
        alive_count += matrix[i-1][j]->is_alive();
        alive_count += matrix[i-1][j-1]->is_alive();
        alive_count += matrix[i-1][j+1]->is_alive();
        alive_count += matrix[0][j]->is_alive();
        alive_count += matrix[0][j-1]->is_alive();
        alive_count += matrix[0][j+1]->is_alive();
    }
    else //every cell not on edges
    {
        //generic case for surrounded life boxes
        alive_count += matrix[i+1][j]->is_alive();
        alive_count += matrix[i-1][j]->is_alive();
        alive_count += matrix[i+1][j+1]->is_alive();
        alive_count += matrix[i-1][j-1]->is_alive();
        alive_count += matrix[i][j+1]->is_alive();
        alive_count += matrix[i][j-1]->is_alive();
        alive_count += matrix[i+1][j-1]->is_alive();
        alive_count += matrix[i-1][j+1]->is_alive();
    }
    return alive_count;
}

void LifeWindow::TakeTurn()
{
    /*
        Resets each life box according to its neighbors
    */
    alive_count_ = 0;
    //set up new box color
    QColor dead(255,255,255);
    int red = rand() % 256;
    int blue = rand() % 256;
    int green = rand() % 256;
    QColor alive(red,green,blue);
    //store life/death values in temporary 2d matrix temp
    bool temp[10][25];
    for(int i = 0; i< 10; i++)
    {
        for(int j = 0; j < 25; j++)
        {
            temp[i][j] = matrix[i][j]->is_alive();
        }
    }
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 25; j++)
        {
            int neighbors = CheckNeighbors(i,j);
            //qDebug() << "Neigh" << neighbors << i << "," << j;
            if(neighbors < 2 && matrix[i][j]->is_alive())
            {
                temp[i][j] = false;
            }
            else if((neighbors == 2 || neighbors == 3) && matrix[i][j]->is_alive())
            {
                temp[i][j] = true;
            }
            else if(neighbors > 3 && matrix[i][j]->is_alive())
            {
                temp[i][j] = false;
            }
            else if(neighbors == 3 && !(matrix[i][j]->is_alive()))
            {
                temp[i][j] = true;
            }
        }
    }
    //change life/death values in life value matrix
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 25; j++)
        {
            if(temp[i][j])
            {
                alive_count_++;
                matrix[i][j]->set_alive(true);
                matrix[i][j] ->setColor(alive);
            }
            else
            {
                matrix[i][j]->set_alive(false);
                matrix[i][j] ->setColor(dead);
            }
        }
    }
    //update UI
    turns_++;
    QGraphicsView * view = ui->lifeGraphicsView;
    percent_alive_ = int((100*alive_count_)/250);
    ui->popLabel->setText(QString("Population: ") + QString(std::to_string(alive_count_).c_str()) + QString("  (") + QString(std::to_string(percent_alive_).c_str()) + QString("%)"));
    ui->turnLabel->setText(QString("Turn: ") + QString(std::to_string(turns_).c_str()));
    updateGraph();
    view->scene()->update();
    repaint();
}

void LifeWindow::Play()
{
    /*
        Calls take turn; dependent upon timer
    */
    TakeTurn();
}

void LifeWindow::on_stepButton_clicked()
{
    /*
        Slot to take one turn
    */
    TakeTurn();
}

void LifeWindow::on_playButton_clicked()
{
    /*
        Slot to start timer
    */
    timer_->start();
}

void LifeWindow::on_pauseButton_clicked()
{
    /*
        Slot to stop timer
    */
    timer_->stop();
}

void LifeWindow::updateGraph(){
    /*
        Updates graph to reflect population percentage
    */
    int h = 315-percent_alive_;
    if (numRecs_ < 25)
    {
        //initial graph only adding rectangles
        QGraphicsRectItem* temp = new QGraphicsRectItem(50 + (20*numRecs_), h,20,percent_alive_);
        numRecs_++;
        bars.push_back(temp);
        scene->addItem(temp);
    }
    else
    {
        //remove old rectanlge and add new one (shift left)
        scene->removeItem(bars[0]);
        bars.erase(bars.begin());
        numRecs_--;
        for (int i = 0; i < int(bars.size()); i++)
        {
            QPointF t(20,0);
            bars[i]->setPos((bars[i]->pos())-t);
        }
        QGraphicsRectItem* temp = new QGraphicsRectItem(50 + (20*numRecs_), h,20,percent_alive_);
        bars.push_back(temp);
        scene->addItem(temp);
        numRecs_++;
    }
}

void LifeWindow::on_horizontalSlider_valueChanged(int value)
{
    /*
        Slot to set play speed, timer, and labels
        @param value is the value the slider was changed to
    */
    speed_ = 0;
    speed_ = int(value+1);
    timer_->setInterval(speed_);
    ui->speedLabel->setText(QString("Speed: ") + QString(std::to_string(101-speed_).c_str()));
    //qDebug() << value;
}
