#include "scene.h"

Scene::Scene(QWidget *parent) : QWidget(parent){}

void Scene::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(Qt::black));
    drawGraps(&p,graphs);
    drawAxelsValues(&p, axelsValues);
    graphs.clear();
    axelsValues.clear();
}

void Scene::Scene::drawGraps(QPainter *p, const std::vector<Graph> &graphs)
{
    for(int i = 0; i < graphs.size(); i++)
    {
        Point2D a = graphs.at(i).a;
        Point2D b = graphs.at(i).b;
        p->drawLine(a.x, a.y, b.x, b.y);
    }
}

void Scene::Scene::drawAxelsValues(QPainter *p, const std::vector<Point2DValue> &axelsValues)
{
    for(int i = 0; i < axelsValues.size(); i++)
    {
        Point2DValue av = axelsValues.at(i);
        p->translate(av.point.x,av.point.y);
        p->rotate(av.rotation);
        p->drawText(5,5, QString::number(axelsValues.at(i).value));
        p->rotate(-av.rotation);
        p->translate(-av.point.x, -av.point.y);
    }
}
