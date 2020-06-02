#include "scene.h"

Scene::Scene(QWidget *parent) : QWidget(parent)
{

}

void Scene::paintEvent(QPaintEvent *  e)
{
    Q_UNUSED(e)
    QPainter p (this);
    p.setRenderHint(QPainter::Antialiasing);
    // отображаем точки и связываем их линиями
    for(int r = 0; r < dots.size(); r++)
    {
        for(int c = 0; c < dots.at(r).size(); c++)
        {
            QPointF dot = dots.at(r).at(c);
            p.drawRect(QRectF(dot,QPointF(dot.rx()+1, dot.ry()+1)));
            // линия в соседа справа
            if(c+1 < dots.at(r).size())
                p.drawLine(QLineF(dot, dots.at(r).at(c+1)));

            // линия в соседа вниз
            if(r+1 < dots.size() && c < dots.size()-1)
                p.drawLine(QLineF(dot, dots.at(r+1).at(c)));
        }
    }
}
