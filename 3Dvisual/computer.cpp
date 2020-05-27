#include "computer.h"
#include <QVector2D>

void Compute(QGraphicsScene *s, const int width, const int height, const Matrix & nodes)
{
    if(!s) return;

    s->clear();

    // нормализуем точки и разворачиваем их согласно углам
    QVector<QVector3D> mtx = rotate(normalize(nodes));

    // преобразуем точки из 3D пространства в проекции на полоскость сцены
    std::vector<std::vector<QPointF>> dots;
    for(int i = 0; i < mtx.size()-1; i++)
    {
        qreal xDot = mtx.at(i).z() == 0.0f ? ((qreal)width) * mtx.at(i).x() : ((qreal)width) * mtx.at(i).x()/    mtx.at(i).z();
        qreal yDot = mtx.at(i).z() == 0.0f ? ((qreal)height) * mtx.at(i).y(): ((qreal)height) * mtx.at(i).y()/   mtx.at(i).z();

        if(i/30 == dots.size())
        {
            dots.push_back(std::vector<QPointF>());
        }
        dots.at(i/30).push_back(QPointF(xDot,yDot));
    }

    // отображаем точки и связываем их линиями
    for(int r = 0; r < dots.size(); r++)
    {
        for(int c = 0; c < dots.at(r).size(); c++)
        {
            QPointF dot = dots.at(r).at(c);
            s->addRect(QRectF(dot,QPointF(dot.rx()+1, dot.ry()+1)));
            // линия в соседа справа
            if(c+1 < dots.at(r).size())
                s->addLine(QLineF(dot, dots.at(r).at(c+1)));

            // линия в соседа вниз
            if(r+1 < dots.size() && c != 29)
                s->addLine(QLineF(dot, dots.at(r+1).at(c)));
        }
    }
}










Matrix normalize(const Matrix &nodes)
{
    Matrix normalNodes;

    int normalRange = upperPeackNormal - bottomPeackNormal;

    double Range = max - min;

    for(int i = 0; i < nodes.size(); i++)
    {
        float x = nodes.at(i).x();
        float y = nodes.at(i).y();
        float z = bottomPeackNormal + (nodes.at(i).z() - min) * normalRange/Range;
        QVector3D n(x,y,z);
        n = Multiply(n,Mscale());

        normalNodes.append(n);
    }

    return normalNodes;
}

QVector<QVector3D> rotate(const QVector<QVector3D> &nodes)
{
    QVector<QVector3D> reangleNodes;
    for(int i = 0; i < nodes.size(); i++)
    {
        QVector3D node = nodes.at(i);
        // разворот по X
        if(angleX != 0.0)
        {
            node.setY(node.y()*cosf(angleX) - node.z()*sinf(angleX));
            node.setZ(node.y()*sinf(angleX) + node.z()*cosf(angleX));
        }

        // разворот по Y
        if(angleY != 0.0f)
        {
            node.setX(node.x()*cosf(angleY) + node.z()*sinf(angleY));
            node.setZ((node.x()*-1.0f)*sinf(angleY) + node.z()*cosf(angleY));
        }

        // разворот по Z
        if(angleZ != 0.0f)
        {
            node.setX(node.x()*cosf(angleZ) - node.y()*sinf(angleZ));
            node.setY(node.x()*sinf(angleZ) + node.y()*cosf(angleZ));
        }


        reangleNodes.append(node);
    }

    // разворачиваем обсчитанные точки согласно выставленным углам
    QVector<QVector3D> rotateNodes;
    for(int i = 0; i < reangleNodes.size(); i++)
    {
        QVector3D m = Multiply(reangleNodes.at(i), Mx(rotateX));
        m = Multiply(m,My(rotateY));
        m = Multiply(m, Mz(rotateZ));
        rotateNodes.append(m);
    }

    return rotateNodes;

}


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
        {SPACE, 0, 0, 0},
        {0, SPACE, 0, 0},
        {0, 0, SPACE, 0},
    };
    return m;
}

QMatrix4x4 Mplane()
{
    float values[] ={
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0,0,0,1

    };
    QMatrix4x4 m(values);
    return m;
}

QVector3D Multiply(QVector3D p, std::vector<std::vector<float>> m)
{
    QVector3D point;
    point.setX(p.x() * m[0][0] + p.x() * m[0][1] + p.z() * m[0][2]);
    point.setY(p.x() * m[1][0] + p.y() * m[1][1] + p.z() * m[1][2]);
    point.setZ(p.x() * m[2][0] + p.y() * m[2][1] + p.z() * m[2][2]);
    return point;
}
