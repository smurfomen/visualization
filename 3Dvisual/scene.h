#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include "computer.h"
#include <QPainter>

// Класс-виджет, на нем можно рисовать, он будет сценой для QPainter'a.
// Для использования достаточно на форме разместить виджет и преобразовать его в Scene (см. Преобразование виджетов)
// хранит в себе матрицу точек, которую нужно отрисовать
class Scene : public QWidget
{
public:
    Scene(QWidget *parent = nullptr);

    virtual void paintEvent(QPaintEvent *e);

    // матрица вершин. Нужно очищать прежде чем обсчитывать снова, чтобы не получить 2 датасета вместо 1
    std::vector<std::vector<QPointF>> dots;

    // очистить матрицу
    void clearData();
};

#endif // SCENE_H
