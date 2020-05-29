#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
namespace Ui {
class View;
}

class View : public QMainWindow
{
    Q_OBJECT

public:
    explicit View(QWidget *parent = 0);
    ~View();

private slots:
    // открыть, посчитать и отобразить график
    void on_open_clicked();

    // слоты изменения пределов диапазона нормирования с формы
    void on_bottomRange_valueChanged(int arg1);
    void on_upperRange_valueChanged(int arg1);

    // слоты изменения углов поворота и наклона с формы
    void on_angleX_valueChanged(double arg1);
    void on_angleY_valueChanged(double arg1);
    void on_angleZ_valueChanged(double arg1);
    void on_rotateX_valueChanged(double value);
    void on_rotateY_valueChanged(double value);
    void on_rotateZ_valueChanged(double value);

    // слот изменения масштаба
    void on_scale_valueChanged(double arg1);

private:
    Ui::View *ui;

    // переопределение события изменения размера у формы для реотресовки графика с новым масштабом
    virtual void resizeEvent(QResizeEvent * e);
};

#endif // VIEW_H
