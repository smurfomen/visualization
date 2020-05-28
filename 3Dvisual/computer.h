#ifndef COMPUTER_H
#define COMPUTER_H
#include <cmath>

extern int bottomPeackNormal;
extern int upperPeackNormal;
extern double min;
extern double max;

extern float SPACE;
#include <QVector>
#include <QVector3D>
#include <QGraphicsScene>

#include <vector>
#include <QMatrix4x4>

// совокупность углов по 3м осям
struct AngleXYZ
{
    float x;
    float y;
    float z;
};

// параметры наклонов
struct AngleParams{
    AngleXYZ angle;                     // наклон
    AngleXYZ rotation;                  // вращение
};

// диапазон
struct Range {
    double top;                         // верхний предел
    double bottom;                      // нижний предел
};

// параметры для калькуляции, инкапсулируют несколько подструктур
struct CalculateParams {
    int w;                              // ширина плоскости (ширина сцены)
    int h;                              // высота плоскости (высота сцены)
    int rows;                           // количество строк в датасете
    int columns;                        // количество колонок в датасете        rows * columns = количество вершин
    QVector<QVector3D> nodes;           // изначальный массив с векторами. По сути - это данные из датасета, у которых X и Y это колонка и строка соответственно, а Z - значение на их пересечении
    AngleParams angleParams;            // параметры наклона
    Range baseRange;                    // базовый диапазон (от min до max)
    Range normalRange;                  // нормирующий диапазон (задается на форме), нужен что бы нивелировать большой разброс между Z координатами
    double scale;
};

// обсчитать переданные параметры и положить обсчитанные вершины в массив dots (он передан по ссылке!!!)
void Compute(std::vector<std::vector<QPointF>> & dots, CalculateParams params);

// нормализация
QVector<QVector3D> normalize(const QVector<QVector3D> &nodes, const CalculateParams &p);

// вращение относительно углов
QVector<QVector3D> rotate(const QVector<QVector3D> &nodes, const AngleParams & aParams);

// умножение вектора на матрицу
QVector3D Multiply(QVector3D p, std::vector<std::vector<float> > m);

// матрица поворота по оси X для угла а
std::vector<std::vector<float> > Mx(float a);

// матрица поворота по оси Y для угла а
std::vector<std::vector<float> > My(float a);

// матрица поворота по оси Z для угла а
std::vector<std::vector<float> > Mz(float a);

// матрица равномерного распределения точек
std::vector<std::vector<float> > Mscale();

std::vector<std::vector<float>> Mplane();

#endif // COMPUTER_H
