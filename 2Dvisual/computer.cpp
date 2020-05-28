#include "computer.h"
#include <QGraphicsTextItem>

struct Node
{
    quint32 xAxelValue = 0;
    qreal yAxelValue = 0.0;
};

// для упрощения создания графов
Graph makeGraph(qreal x1, qreal y1, qreal x2, qreal y2)
{
    return Graph{Point2D{x1,y1}, Point2D{x2,y2}};
}

// для упрощения создания пометок
Point2DValue makePoint2DValue(qreal x, qreal y, qreal value, qreal rotation = 0.0){
    Point2DValue p;
    p.point = Point2D {x,y}; p.value = value; p.rotation = rotation;
    return p;
}

void calculateGraphs(std::vector<Graph> & graphs, std::vector<Point2DValue> & axelsValues, int w, int h, Filter f)
{
    QFile * file = f.file;
    if(file == nullptr || !file->open(QFile::ReadOnly) || !strlen(f.mask) )
        return;

    int width = w;
    int height = h;



    // разбираем файл
    Node * pdata = (Node*) malloc(SIZE_INCREMENT * sizeof (Node));
    size_t size = SIZE_INCREMENT;
    size_t iter = 0;
    qreal max = 0.0, min = 0.0;
    while (!f.file->atEnd())
    {
        QString note(f.file->readLine());
        if(note.contains(QString(f.mask)))
        {
            QStringList fieldsOfNote = note.split(",");
            Node n;
            n.xAxelValue = fieldsOfNote.at(f.xAxelColumn).toInt();
            n.yAxelValue = fieldsOfNote.at(f.yAxelColumn).toDouble();

            if(n.yAxelValue == 0.0)
                continue;

            if(iter == 0)
                max = min = n.yAxelValue;

            if(n.yAxelValue > max)
                max = n.yAxelValue;
            else if (n.yAxelValue < min)
                min = n.yAxelValue;

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
    graphs.push_back(makeGraph(MARGIN,MARGIN,MARGIN,height-MARGIN));


    // ось X, в зависимости от знака минимального элемента рисуем либо посередине сцены либо снизу с отступом
    graphs.push_back(makeGraph(MARGIN,height - (min < 0 ? height/2 : MARGIN),width-MARGIN,height - (min < 0 ? height/2 : MARGIN)));

    // сдвижка каждого следующего элемента по оси X
    qreal widthOffset = (width-DMARGIN)/size;

    // отношение сцены к разбежке максимального и минимального значений
    qreal heightRate = abs((height-DMARGIN)/(max-min));

    // среднее по в разбежке максимального и менимального значений, можно установить больше
    qreal horizontalMidLines [] = {min, min+(max-min)*0.33, min+(max-min)*0.66, max};
    for (int i = 0; i < 4; i++) {
        // рисуем засечку и пишем рядом с ней ее значение
        qreal y = height-MARGIN-abs(heightRate*(horizontalMidLines[i]-min));

        graphs.push_back(makeGraph(MARGIN-5, y, MARGIN+5, y));
        axelsValues.push_back(makePoint2DValue(0,y, horizontalMidLines[i]));
    }

    // рисуем последовательно сам график, перебирая накопленные элементы
    for(size_t i = 0; i < size; i++)
    {
        if(i < size-1)
        {
            // сама кривая
            qreal x1 = widthOffset*i;
            qreal y1 = height-abs(heightRate*(pdata[i].yAxelValue-min));
            qreal x2 = widthOffset*(i+1);
            qreal y2 = height-abs(heightRate*(pdata[i+1].yAxelValue-min));
            graphs.push_back(makeGraph(MARGIN+x1,y1-MARGIN,MARGIN+x2, y2-MARGIN));
        }

        // рисуем засечку на оси X
        graphs.push_back(makeGraph(MARGIN + widthOffset*i ,height - (min < 0 ? height/2 : MARGIN) - 5, MARGIN + widthOffset*i ,height - (min < 0 ? height/2 : MARGIN) + 5));

        // добавляем текстовое значение года под засечкой на оси X, поворачиваем его под углом 75
        axelsValues.push_back(makePoint2DValue(MARGIN + widthOffset*i + widthOffset/2 , height - (min < 0 ? height/2 : MARGIN) + 5, pdata[i].xAxelValue, 75.00));
    }

    file->close();
    free(pdata);
    return;
}
