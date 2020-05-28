#include "computer.h"
#include <QVector2D>

void Compute(std::vector<std::vector<QPointF> > &dots, CalculateParams params)
{

    // нормализуем точки и разворачиваем их согласно углам
    QVector<QVector3D> mtx = rotate(normalize(params.nodes, params), params.angleParams);

    // преобразуем точки из 3D пространства в проекции на полоскость сцены
    for(int i = 0; i < mtx.size()-1; i++)
    {
        qreal xDot = mtx.at(i).z() == 0.0f ? ((qreal)params.w) * mtx.at(i).x() : ((qreal)params.w) * mtx.at(i).x()/    mtx.at(i).z();
        qreal yDot = mtx.at(i).z() == 0.0f ? ((qreal)params.h) * mtx.at(i).y(): ((qreal)params.h) * mtx.at(i).y()/   mtx.at(i).z();

        if(i/30 == dots.size())
            dots.push_back(std::vector<QPointF>());
        dots.at(i/30).push_back(QPointF(xDot,yDot));
    }
}




// нормализовать значения по Z
QVector<QVector3D> normalize(const QVector<QVector3D> &nodes, const CalculateParams &p)
{
    QVector<QVector3D> normalNodes;

    float normalRange = p.normalRange.top - p.normalRange.bottom;

    float Range = p.baseRange.top - p.baseRange.bottom;

    for(int i = 0; i < nodes.size(); i++)
    {
        float x = 10 + nodes.at(i).x();
        float y = 10 + nodes.at(i).y();
        float z = p.normalRange.bottom + (nodes.at(i).z() - p.baseRange.bottom) * normalRange/Range;
        QVector3D n(x,y,z);
        n = Multiply(n,Mscale());

        normalNodes.append(n);
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

    AngleXYZ rotate = aParams.rotation;
    // разворачиваем обсчитанные точки согласно выставленным углам
    QVector<QVector3D> rotatedNodes;
    for(int i = 0; i < reangleNodes.size(); i++)
    {
        QVector3D m = Multiply(reangleNodes.at(i), Mx(rotate.x));
        m = Multiply(m,My(rotate.y));
        m = Multiply(m, Mz(rotate.z));
        rotatedNodes.append(m);
    }

    return rotatedNodes;

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

