#include "computer.h"
#include <QVector2D>
void Compute(QGraphicsScene *s, const int width, const int height, const Matrix & nodes)
{
    if(!s) return;

    s->clear();

    Matrix mtx = rotate(normalize(nodes));

//    s->addLine()

    for(QVector3D n : mtx)
    {
        s->addEllipse(n.x(),n.y(),n.z(),n.z());
    }




}










Matrix normalize(const Matrix &nodes)
{
    Matrix normalNodes;

    int normalRange = tR - bR;

    int Range = max - min;

    double c = Range/normalRange;

    for(QVector3D node : nodes)
        normalNodes.append(QVector3D(node.x()*SPACE, node.y()*SPACE, c/node.z()));

    return normalNodes;
}

Matrix rotate(const Matrix &nodes)
{
    Matrix rotatedNodes;
    for(QVector3D node : nodes)
    {
        // разворот по X
        node.setY(node.y()*cos(angleX) - node.z()*sin(angleX));
        node.setZ(node.y()*sin(angleX) + node.z()*cos(angleX));

        // разворот по Y
        node.setX(node.x()*cos(angleY) + node.z()*sin(angleY));
        node.setZ((node.x()*-1.0f)*sin(angleY) + node.z()*cos(angleY));

        // разворот по Z
        node.setX(node.x()*cos(angleZ) - node.y()*sin(angleZ));
        node.setY(node.x()*sin(angleZ) + node.y()*cos(angleZ));

        rotatedNodes.append(node);
    }
    return rotatedNodes;

}
