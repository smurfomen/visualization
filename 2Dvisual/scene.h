#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include "computer.h"
#include <QPainter>
#include <QTextItem>
class Scene : public QWidget
{
public:
    Scene(QWidget *parent = nullptr);

    virtual void paintEvent(QPaintEvent * e);

    std::vector<Graph> graphs;
    std::vector<Point2DValue> axelsValues;


    void drawGraps(QPainter * p, const std::vector<Graph> &graphs);

    void drawAxelsValues(QPainter * p, const std::vector<Point2DValue> &axelsValues);
};

#endif // SCENE_H
