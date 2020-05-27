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
    void on_open_clicked();

    void on_bottomRange_valueChanged(int arg1);

    void on_upperRange_valueChanged(int arg1);

    void on_angleX_valueChanged(double arg1);

    void on_angleY_valueChanged(double arg1);

    void on_angleZ_valueChanged(double arg1);

    void on_space_valueChanged(int arg1);

    void on_rotateX_valueChanged(double value);

    void on_rotateY_valueChanged(double value);

    void on_rotateZ_valueChanged(double value);

private:
    Ui::View *ui;
};

#endif // VIEW_H
