#include "computer.h"
#include <QPainter>
#include <QGraphicsTextItem>
const int MARGIN = 50;
const int DMARGIN = MARGIN*2;
const int SIZE_INCREMENT = 10;

struct Node
{
    quint32 year = 0;
    qreal value = 0.0;
};

void ComputeAndDraw(QGraphicsScene * s, int w, int h, QFile * file, const char * region, int column, const char * nameColumn)
{
    if(!s || !file->open(QFile::ReadOnly))
        return;

    s->clear();
        s->addText(nameColumn)->moveBy(w-DMARGIN,0);
    s->addText(region)->moveBy(DMARGIN, 0);
    int width = w;
    int height = h;

    // разбираем файл
    Node * pdata = (Node*) malloc(SIZE_INCREMENT * sizeof (Node));
    size_t size = SIZE_INCREMENT;
    size_t iter = 0;
    qreal max = 0.0, min = 0.0;
    while (!file->atEnd())
    {
        QString note(file->readLine());
        if(note.contains(region))
        {
            QStringList fieldsOfNote = note.split(",");
            Node n;
            n.year = fieldsOfNote.at(0).toInt();
            n.value = fieldsOfNote.at(column).toDouble();
            if(n.value == 0.0)
                continue;

            if(iter == 0)
                max = min = n.value;

            if(n.value > max)
                max = n.value;
            else if (n.value < min)
                min = n.value;

            if(iter == size)
            {
                size += SIZE_INCREMENT;
                pdata = (Node*) realloc(pdata, sizeof(Node) * size);
            }

            pdata[iter++] = n;
        }
    }

    size = iter;

    // ось Y
    s->addLine(MARGIN,MARGIN,MARGIN,height-MARGIN);

    // ось X, в зависимости от знака минимального элемента рисуем либо посередине сцены либо снизу с отступом
    s->addLine(MARGIN,height - (min < 0 ? height/2 : MARGIN),width-MARGIN,height - (min < 0 ? height/2 : MARGIN));

    // сдвижка каждого следующего элемента по оси X
    qreal widthOffset = (width-DMARGIN)/size;

    // отношение сцены к разбежке максимального и минимального значений
    qreal heightRate = abs((height-DMARGIN)/(max-min));

    // среднее по в разбежке максимального и менимального значений, можно установить больше
    qreal horizontalMidLines [] = {min, min+(max-min)*0.33, min+(max-min)*0.66, max};
    for (int i = 0; i < 4; i++) {
        // рисуем засечку и пишем рядом с ней ее значение
        qreal y = height-MARGIN-abs(heightRate*(horizontalMidLines[i]-min));
        s->addLine(MARGIN-5, y, MARGIN+5, y);
        QGraphicsTextItem * txtV = s->addText(QString::number(horizontalMidLines[i]));
        txtV->moveBy(0, y);
    }

    // рисуем последовательно сам график, перебирая накопленные элементы
    for(size_t i = 0; i < size; i++)
    {
        if(i < size-1)
        {
            // сама кривая
            qreal x1 = widthOffset*i;
            qreal y1 = height-abs(heightRate*(pdata[i].value-min));
            qreal x2 = widthOffset*(i+1);
            qreal y2 = height-abs(heightRate*(pdata[i+1].value-min));
            s->addLine(MARGIN+x1,y1-MARGIN,MARGIN+x2, y2-MARGIN);
        }

        // рисуем засечку на оси X
        s->addLine(MARGIN + widthOffset*i ,height - (min < 0 ? height/2 : MARGIN) - 5, MARGIN + widthOffset*i ,height - (min < 0 ? height/2 : MARGIN) + 5);

        // добавляем текстовое значение года под засечкой на оси X, поворачиваем его под углом 75
        QGraphicsTextItem * txtH = s->addText(QString::number(pdata[i].year));
        txtH->setRotation(75);
        txtH->moveBy(MARGIN + widthOffset*i + widthOffset/2 , height - (min < 0 ? height/2 : MARGIN) + 5);
    }

    file->close();
//    free(pdata);
}
