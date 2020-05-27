#ifndef COMPUTER_H
#define COMPUTER_H
#include <cmath>
extern int columns;
extern int rows;
extern double angleX;
extern double angleY;
extern double angleZ;

extern int bR;
extern int tR;
extern double min;
extern double max;

const int SPACE = 10;
#include <QVector>
#include <QVector3D>
#include <QGraphicsScene>

typedef QVector<QVector3D> Matrix;

void Compute(QGraphicsScene * s, const int width, const int height, const Matrix & nodes);

Matrix normalize(const Matrix &nodes);

Matrix rotate(const Matrix& nodes);
#endif // COMPUTER_H
