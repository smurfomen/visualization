#include "scene.h"

Scene::Scene(QWidget *parent) : QWidget(parent){}

void Scene::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter p(this);

    // настраиваем режим антиалайсинга для более сглаженной картинки
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(Qt::black));

    // рисуем оси и график
    drawGraps(&p,graphs);

    // рисуем значения на осях
    drawAxelsValues(&p, axelsValues);
}

void Scene::Scene::drawGraps(QPainter *p, const std::vector<Graph> &graphs)
{
    for(int i = 0; i < graphs.size(); i++)
    {
        // берем 2 точки и рисуем между ними прямую
        Point2D a = graphs.at(i).a;
        Point2D b = graphs.at(i).b;
        p->drawLine(a.x, a.y, b.x, b.y);
    }
}


// рисуем значения на осях
void Scene::Scene::drawAxelsValues(QPainter *p, const std::vector<Point2DValue> &axelsValues)
{
    for(int i = 0; i < axelsValues.size(); i++)
    {
        Point2DValue av = axelsValues.at(i);
        // перемещаемся к этой точке
        p->translate(av.point.x,av.point.y);
        // вращаем пэинтер на угол разворота значения (полезно при большой кучности значений)
        p->rotate(av.rotation);

        // пишем значение с маленьким отступом 5
        p->drawText(5,5, QString::number(axelsValues.at(i).value));

        // возвращаемся в исходное положение
        p->rotate(-av.rotation);
        p->translate(-av.point.x, -av.point.y);
    }
}

// очищаем данные БЕЗ ПЕРЕОТРИСОВКИ
// для переотрисовки нужно вызвать метод repaint
void Scene::clearData()
{
    graphs.clear();
    axelsValues.clear();
}
