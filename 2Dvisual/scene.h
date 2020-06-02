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

    void setChart(Chart c){
        chart = c;
        repaint();
    }

    Chart chart;

    void drawGraps(QPainter * p);

    void drawAxelsValues(QPainter * p);

    void clearData();
};



#endif // SCENE_H
