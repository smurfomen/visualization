#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include "computer.h"
#include <QPainter>

class Scene : public QWidget
{
public:
    Scene(QWidget *parent = nullptr);

    virtual void paintEvent(QPaintEvent *e);

    std::vector<std::vector<QPointF>> dots;

    void clearData();
};

#endif // SCENE_H
