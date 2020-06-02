#include "computer.h"
#include <QVector2D>
#include <QFile>
#include <iostream>
#include <fstream>
#include <iosfwd>
// P.S: Артефакты по прежнему есть при разворотах, я думаю что это связанно с неверным приведением или умножением на 0 на каком-то из этапов преобразования
#include <string>

std::vector<QString> readline(std::ifstream * f){
    std::vector<QString> lines;
    std::string tmp;
    getline(*f, tmp);
    if(!tmp.length())
        throw -1;
    QString qtmp = QString::fromStdString(tmp);
    QStringList list = qtmp.split(',');
    for(auto str : list)
    {
        lines.push_back(str);
    }
    return lines;
}

// обсчитать графы
std::vector<std::vector<QPointF>> makeChartExecute(Settings params)
{
    std::vector<std::vector<QPointF>> dots;
    Range baseRange;
    int columns  = 0;
    int rows = 0;
    QVector<QVector3D> nodes;           // изначальный массив с векторами. По сути - это данные из датасета, у которых X и Y это колонка и строка соответственно, а Z - значение на их пересечении
    std::ifstream file(params.filePath.toStdString().c_str());
    if(!file.is_open()) return dots;

    // обнуляем старые параметры
    baseRange.bottom = 0.0;
    baseRange.top = 0.0;

    // читаем файл и заполняем первичный массив векторов данными
    // в массиве nodes находятся 3D векторы(т.е пространственные коорднаты), x - колонка, y - строка, z - считанное значение на пересечении x,y
    while(file.good())
    {
        try {
            std::vector<QString> fieldsOfRow = readline(&file);

            // в самом начале фиксируем минимальный и максимальный предел на первом элементе
            if(rows == 0)
                baseRange.bottom = baseRange.top = fieldsOfRow.at(0).toDouble();

            // обнуляем колонку, т.к. будем идти вдоль оси X по таблице
            columns = 0;
            for(; columns < fieldsOfRow.size(); columns++)
            {
                // берем значение на пересечении и сравниваем его с актуальными пределами, если что - обновляем
                double val = fieldsOfRow.at(columns).toDouble();
                if(baseRange.bottom > val)
                    baseRange.bottom = val;
                else if(baseRange.top < val)
                    baseRange.top = val;

                // создаем вектор и кладем в массив считанных векторов
                nodes.append(QVector3D(columns,rows,val));
            }
            rows++;
        } catch (...) {
            if(file.good())
                return dots;
        }

    }

    file.close();



    // нормализуем точки и разворачиваем их согласно углам
    QVector<QVector3D> mtx = rotate(normalize(nodes, baseRange, params.normalRange), params.angleParams);


    // преобразуем точки из 3D пространства в проекции на полоскость сцены
    for(int i = 0; i < mtx.size()-1; i++)
    {
        qreal xDot = mtx.at(i).z() == 0.0f ? ((qreal)params.w) * mtx.at(i).x() : ((qreal)params.w) * mtx.at(i).x()/    mtx.at(i).z();
        qreal yDot = mtx.at(i).z() == 0.0f ? ((qreal)params.h) * mtx.at(i).y(): ((qreal)params.h) * mtx.at(i).y()/   mtx.at(i).z();

        // скалируем получившиеся точки прежде чем добавить в массив точек
        xDot *= params.scale;
        yDot *= params.scale;

        // если нужно добавить строчку - добавляем
        if(i/rows == dots.size())
            dots.push_back(std::vector<QPointF>());

        // продолжаем добавлять в актуальную строку вершины
        dots.at(i/rows).push_back(QPointF(xDot,yDot));
    }

    return dots;
}




// нормализовать значения по Z
QVector<QVector3D> normalize(const QVector<QVector3D> &nodes, Range bRange, Range nRange)
{
    QVector<QVector3D> normalNodes;

    // диапазон нормализации
    float normalRange = nRange.top - nRange.bottom;

    // диапазон разброса реальных значений по Z
    float Range = bRange.top - bRange.bottom;


    for(int i = 0; i < nodes.size(); i++)
    {
        float x = 5+nodes.at(i).x();
        float y = 5+nodes.at(i).y();

        // согласно формуле нормализуем координату Z
        float z = nRange.bottom + (nodes.at(i).z() - bRange.bottom) * normalRange/Range;

        QVector3D node(x,y,z);
        // умножаем вектор на матрицу масштабирования и равномерного распределения
        node = Multiply(node, Mscale());
        node = Multiply(node, Mplane());
        normalNodes.append(node);
    }

    return normalNodes;
}


// развернуть согласно выставленным углам разворота
QVector<QVector3D> rotate(const QVector<QVector3D> &nodes, const AngleParams & aParams)
{
    QVector<QVector3D> reangleNodes;
    for(int i = 0; i < nodes.size(); i++)
    {
        QVector3D node = nodes.at(i);

        // берем углы наклона из параметров
        AngleXYZ angle = aParams.angle;

        // разворот по X
        if(angle.x != 0.0)
        {
            node.setY(node.y()*cosf(angle.x) - node.z()*sinf(angle.x));
            node.setZ(node.y()*sinf(angle.x) + node.z()*cosf(angle.x));
        }

        // разворот по Y
        if(angle.y!= 0.0f)
        {
            node.setX(node.x()*cosf(angle.y) + node.z()*sinf(angle.y));
            node.setZ((node.x()*-1.0f)*sinf(angle.y) + node.z()*cosf(angle.y));
        }

        // разворот по Z
        if(angle.z != 0.0f)
        {
            node.setX(node.x()*cosf(angle.z) - node.y()*sinf(angle.z));
            node.setY(node.x()*sinf(angle.z) + node.y()*cosf(angle.z));
        }

        reangleNodes.append(node);
    }

    // берем углы разворота из параметров
    AngleXYZ rotate = aParams.rotation;

    // разворачиваем обсчитанные точки согласно выставленным углам
    // для этого умнажаем вектор на соответствующую матрицу посчитанную для нужного угла, в какую сторону вращаем
    // например: вращение по X - берем матрицу разворота по X для угла А и умножаем вектор на матрицу
    QVector<QVector3D> rotatedNodes;
    for(int i = 0; i < reangleNodes.size(); i++)
    {
        QVector3D m(reangleNodes.at(i));
        m = Multiply(m,Mx(rotate.x));
        m = Multiply(m,My(rotate.y));
        m = Multiply(m, Mz(rotate.z));

        rotatedNodes.append(m);
    }

    return rotatedNodes;

}

// умножение вектора на матрицу
QVector3D Multiply(QVector3D p, std::vector<std::vector<float>> m)
{
    QVector3D point;
    point.setX(p.x() * m[0][0] + p.x() * m[0][1] + p.z() * m[0][2]);
    point.setY(p.x() * m[1][0] + p.y() * m[1][1] + p.z() * m[1][2]);
    point.setZ(p.x() * m[2][0] + p.y() * m[2][1] + p.z() * m[2][2]);
    return point;
}

// матрицы из примеров
std::vector<std::vector<float>> Mx(float a)
{
    std::vector<std::vector<float>> m = {
        {1, 0, 0, 0},
        {0, cosf(a), sinf(a)*-1.0f, 0},
        {0, sinf(a), cosf(a), 0}

    };
    return m;
}

std::vector<std::vector<float>> My(float a)
{
    std::vector<std::vector<float>> m = {
        {cosf(a), 0, sinf(a), 0},
        {0, 1, 0, 0},
        {sinf(a) * -1.0f, 0, cosf(a), 0}
    };
    return m;
}

std::vector<std::vector<float>> Mz(float a)
{
    std::vector<std::vector<float>> m = {
        {cosf(a), sinf(a) * -1.0f, 0, 0},
        {sinf(a), cosf(a), 0, 0},
        {0, 0, 1, 0}
    };
    return m;
}

std::vector<std::vector<float>> Mscale()
{
    std::vector<std::vector<float>> m = {
        {30, 0, 0, 0},
        {0, 30, 0, 0},
        {0, 0, 30, 0},
    };
    return m;
}

std::vector<std::vector<float>> Mplane()
{
    std::vector<std::vector<float>> m ={
        {1.0f, 0, 0, 0},
        {0, 1.0f, 0, 0},
        {0, 0, 1.0f, 0},
        {0,0,0,1}

    };
    return m;
}

