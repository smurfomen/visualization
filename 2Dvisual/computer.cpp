#include "computer.h"
#include <QGraphicsTextItem>
#include <iostream>
#include <fstream>
// узел, фактически представляет собой значения конкретной точки
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
Label makePoint2DValue(qreal x, qreal y, qreal value, qreal rotation = 0.0){
    Label p;
    p.point = Point2D {x,y}; p.value = value; p.rotation = rotation;
    return p;
}


std::vector<QString> readline(std::ifstream * f, QString mask){
    std::vector<QString> lines;
    while(f->good())
    {
        std::string tmp;
        getline(*f, tmp);
        if(!tmp.length())
            throw -1;

        QString qtmp = QString::fromStdString(tmp);
        if(qtmp.contains(mask))
        {
            QStringList list = qtmp.split(',');
            for(auto str : list)
            {
                lines.push_back(str);
            }
            break;
        }
    }

    return lines;
}


Chart makeChartExecute(Settings f)
{
    std::vector<Graph> graphs;
    std::vector<Label> labels;
    std::ifstream file(f.filePath.toStdString().c_str());
    if(!file.is_open() || f.mask.isEmpty() )
        return Chart{};

    int width = f.w;
    int height = f.h;

    Node * pdata = (Node*) malloc(SIZE_INCREMENT * sizeof (Node));
    size_t size = SIZE_INCREMENT;
    size_t iter = 0;
    qreal max = 0.0, min = 0.0;

    // разбираем файл
    while (file.good())
    {
        try {
            std::vector<QString> fieldsOfNote = readline(&file,f.mask);
            if(!file.good())
                break;
            if(f.xAxelColumn >= fieldsOfNote.size() || f.yAxelColumn >= fieldsOfNote.size())
                return Chart{};

            // создаем ноду и кладем в нее значения из соответствующих столбцов
            Node n;
            n.xAxelValue = fieldsOfNote.at(f.xAxelColumn).toInt();
            n.yAxelValue = fieldsOfNote.at(f.yAxelColumn).toDouble();

            // пропускаем если значение 0. В противном случае, при отсутствии данных в столбце значения (yAxelColumn), график будет с провисаниями
            // а так он рисуется, пропуская нулевые значения. Как лучше - вопрос открытый
            if(n.yAxelValue == 0.0)
                continue;

            // в самом начале минимальный и максимальный элементы будут равны первому элементу
            if(iter == 0)
                max = min = n.yAxelValue;

            // обновляем минимум и максимум
            if(n.yAxelValue > max)
                max = n.yAxelValue;
            else if (n.yAxelValue < min)
                min = n.yAxelValue;

            // если текущая позиция на которую будет происходить добавление равна размеру - увеличиваем размер и реаллоцируем всю память в другое место
            if(iter == size)
            {
                size += SIZE_INCREMENT;
                pdata = (Node*) realloc(pdata, sizeof(Node) * size);
            }

            // так как iter увеличивается постинкрементом - по завершению цикла его значение будет равно количеству элементов, а не последнему индексу
            pdata[iter++] = n;
        } catch (...) {
            // поймали исключение а файл еще не кончился - ошибка, возвращаем пустой график
            if(file.good())
                return Chart{};
        }

    }

    // размер равен количеству итераций.
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
    // этот массив нужен, чтобы отрисовать в равных пропорциях шкалу Y
    qreal verticalMidValues [] = {min, min+(max-min)*0.33, min+(max-min)*0.66, max};
    for (int i = 0; i < 4; i++) {
        qreal y = height-MARGIN-abs(heightRate*(verticalMidValues[i]-min));

        graphs.push_back(makeGraph(MARGIN-5, y, MARGIN+5, y));
        labels.push_back(makePoint2DValue(0,y, verticalMidValues[i]));
    }

    // наполняем графами массив, перебирая накопленные элементы
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
        labels.push_back(makePoint2DValue(MARGIN + widthOffset*i + widthOffset/2 , height - (min < 0 ? height/2 : MARGIN) + 5, pdata[i].xAxelValue, 75.00));
    }

    // закрываем файл, освобождаем память
    file.close();
    free(pdata);

    Chart chart;
    chart.graphs = graphs;
    chart.labels = labels;
    return chart;
}
