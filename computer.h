#ifndef COMPUTER_H
#define COMPUTER_H

#include <QGraphicsScene>
#include <QFile>
const int MARGIN = 50;
const int DMARGIN = MARGIN*2;
const int SIZE_INCREMENT = 10;
void ComputeAndDraw(QGraphicsScene * scene, int w, int h, QFile * file, const char *region, int column);



#endif // COMPUTER_H
