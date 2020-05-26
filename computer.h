#ifndef COMPUTER_H
#define COMPUTER_H

#include <QGraphicsScene>
#include <QFile>

void ComputeAndDraw(QGraphicsScene * scene, int w, int h, QFile * file, const char *region, int column, const char *nameColumn);



#endif // COMPUTER_H
