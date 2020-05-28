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
struct AngleXYZ{
    float x;
    float y;
    float z;
};

struct AngleParams{
    AngleXYZ angle;
    AngleXYZ rotation;
};

struct Range {
    double top;
    double bottom;
};

struct CalculateParams {
    int w;
    int h;
    int rows;
    int columns;
    QVector<QVector3D> nodes;
    AngleParams angleParams;
    Range baseRange;
    Range normalRange;
};

void Compute(std::vector<std::vector<QPointF>> & dots, CalculateParams params);

QVector<QVector3D> normalize(const QVector<QVector3D> &nodes, const CalculateParams &p);

QVector<QVector3D> rotate(const QVector<QVector3D> &nodes, const AngleParams & aParams);

QVector3D Multiply(QVector3D p, std::vector<std::vector<float> > m);

std::vector<std::vector<float> > Mx(float a);

std::vector<std::vector<float> > My(float a);

std::vector<std::vector<float> > Mz(float a);

std::vector<std::vector<float> > Mscale();

QMatrix4x4 Mplane();

#endif // COMPUTER_H
