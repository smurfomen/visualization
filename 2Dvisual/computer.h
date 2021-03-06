#ifndef COMPUTER_H
#define COMPUTER_H

#include <QGraphicsScene>
#include <QFile>
const int MARGIN = 50;
const int DMARGIN = MARGIN*2;
const int SIZE_INCREMENT = 10;

// точка с координатами на плоскости
struct Point2D
{
    qreal x;
    qreal y;
};

// граф с двумя вершинами
struct Graph
{
    Point2D a;
    Point2D b;
};

// точка с значением и углом разворота - нужна что бы рисовать значения шкалы на графике
struct Label
{
    Point2D point;
    qreal value;
    qreal rotation = 0.0;
};

struct Chart {
    std::vector<Graph> graphs;
    std::vector<Label> labels;
};

// фильтрационные данные
struct Settings
{
    int w;
    int h;
    QString filePath;           // читаемый файл
    QString mask;      // маска, по которой будут фильтроваться записи
    int maskColumn;         // колонка в которой искать маску
    quint16 yAxelColumn;    // колонка с данными по оси Y (значения)
    quint16 xAxelColumn;    // колонка с данными по оси X (года)
};

///\brief обсчет графов
///\param graphs - ссылка на вектор графов, который будет наполнен
///\param axelsValues - ссылка на вектор точек со значениями, для отображения размерности шкал
///\param w - ширина плоскости
///\param h - высота плоскости
///\param f - данные
Chart makeChartExecute(Settings f);



#endif // COMPUTER_H
