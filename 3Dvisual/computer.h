#ifndef COMPUTER_H
#define COMPUTER_H
#include <cmath>
extern int columns;
extern int rows;

extern float angleX;
extern float angleY;
extern float angleZ;

extern float rotateX;
extern float rotateY;
extern float rotateZ;


extern int bottomPeackNormal;
extern int upperPeackNormal;
extern double min;
extern double max;

extern float SPACE;
#include <QVector>
#include <QVector3D>
#include <QGraphicsScene>

typedef QVector<QVector3D> Matrix;
#include <vector>
#include <QMatrix4x4>

void Compute(QGraphicsScene * s, const int width, const int height, const Matrix & nodes);
Matrix normalize(const Matrix &nodes);

QVector<QVector3D> rotate(const QVector<QVector3D> &nodes);

QVector3D Multiply(QVector3D p, std::vector<std::vector<float> > m);

std::vector<std::vector<float> > Mx(float a);

std::vector<std::vector<float> > My(float a);

std::vector<std::vector<float> > Mz(float a);

std::vector<std::vector<float> > Mscale();

QMatrix4x4 Mplane();

#endif // COMPUTER_H
