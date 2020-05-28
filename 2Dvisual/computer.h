#ifndef COMPUTER_H
#define COMPUTER_H

#include <QGraphicsScene>
#include <QFile>
const int MARGIN = 50;
const int DMARGIN = MARGIN*2;
const int SIZE_INCREMENT = 10;

struct Point2D
{
    qreal x;
    qreal y;
};

struct Graph{
    Point2D a;
    Point2D b;
};

struct Point2DValue{
    Point2D point;
    qreal value;
    qreal rotation = 0.0;
};

struct Filter{
    QFile * file;
    const char * mask;
    int maskColumn;
    int yAxelColumn;
    int xAxelColumn;
};

void calculateGraphs(std::vector<Graph> &graphs, std::vector<Point2DValue> &axelsValues, int w, int h, Filter f);



#endif // COMPUTER_H
