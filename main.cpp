/*
Kyle Schultz and Liz Parker
Homework 4
main.cpp actually runs our code in QTCreator
*/

#include "lifewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LifeWindow w;
    w.show();

    return a.exec();
}
