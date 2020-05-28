#include "view.h"
#include "ui_view.h"
#include <QFileDialog>
#include <QDebug>
#include "computer.h"
#include <QVector2D>
#include <QVector3D>
#include "scene.h"


// параметры для калькуляции, подробнее в определении структуры
CalculateParams cparams;

View::View(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::View)
{
    ui->setupUi(this);

    // изначально
    cparams.angleParams.rotation.x = ui->rotateX->value();
    cparams.angleParams.rotation.y = ui->rotateY->value();
    cparams.angleParams.rotation.z = ui->rotateZ->value();

    cparams.angleParams.angle.x = ui->angleX->value();
    cparams.angleParams.angle.y = ui->angleY->value();
    cparams.angleParams.angle.z = ui->angleZ->value();

    cparams.normalRange.top = ui->upperRange->value();
    cparams.normalRange.bottom = ui->bottomRange->value();

    cparams.scale = ui->scale->value();

    this->setWindowTitle("ЛР 4. 3D визуализация.");
}

View::~View()
{
    delete ui;
}

void View::on_open_clicked()
{
    QString path ;
    if(ui->filePath->text().isEmpty())
    {
        path = QFileDialog::getOpenFileName(this, tr("Выбрать датасет"), tr("Датасеты(*.csv)"));
        ui->filePath->setText(path);
    }
    else
        path = ui->filePath->text();

    QFile file(path);
    if(file.open(QFile::ReadOnly))
    {
        // обнуляем старые параметры
        cparams.nodes.clear();
        cparams.baseRange.bottom = 0.0;
        cparams.baseRange.top = 0.0;
        cparams.columns  = 0;
        cparams.rows = 0;

        // читаем файл и заполняем первичный массив векторов данными
        // в массиве nodes находятся 3D векторы(т.е пространственные коорднаты), x - колонка, y - строка, z - считанное значение на пересечении x,y
        while(!file.atEnd())
        {
            QStringList fieldsOfRow = QString(file.readLine()).split(",");

            // в самом начале фиксируем минимальный и максимальный предел на первом элементе
            if(cparams.rows == 0)
                cparams.baseRange.bottom = cparams.baseRange.top = fieldsOfRow.at(0).toDouble();

            // обнуляем колонку, т.к. будем идти вдоль оси X по таблице
            cparams.columns = 0;
            for(; cparams.columns < fieldsOfRow.size(); cparams.columns++)
            {
                // берем значение на пересечении и сравниваем его с актуальными пределами, если что - обновляем
                double val = fieldsOfRow.at(cparams.columns).toDouble();
                if(cparams.baseRange.bottom > val)
                    cparams.baseRange.bottom = val;
                else if(cparams.baseRange.top < val)
                    cparams.baseRange.top = val;

                // создаем вектор и кладем в массив считанных векторов
                cparams.nodes.append(QVector3D(cparams.columns,cparams.rows,val));
            }
            cparams.rows++;
        }

        file.close();

        // обновляем размеры сцены на случай если они изменились, это нужно для обсчета
        cparams.w = ui->scene->width();
        cparams.h = ui->scene->height();

        // очищаем данные которые там были и кладем новые
        ui->scene->clearData();
        Compute(ui->scene->dots, cparams);

        // перересовываем
        ui->scene->repaint();
    }
}


void View::on_bottomRange_valueChanged(int arg1)
{
    cparams.normalRange.bottom = arg1;
    on_open_clicked();
}

void View::on_upperRange_valueChanged(int arg1)
{
    cparams.normalRange.top = arg1;
    on_open_clicked();
}

void View::on_angleX_valueChanged(double arg1)
{
    cparams.angleParams.angle.x = (float)arg1;
    on_open_clicked();
}

void View::on_angleY_valueChanged(double arg1)
{
    cparams.angleParams.angle.y = (float)arg1;
    on_open_clicked();
}

void View::on_angleZ_valueChanged(double arg1)
{
    cparams.angleParams.angle.z = (float)arg1;
    on_open_clicked();
}

void View::on_rotateX_valueChanged(double value)
{
    cparams.angleParams.rotation.x = (float)value;
    on_open_clicked();
}

void View::on_rotateY_valueChanged(double value)
{
    cparams.angleParams.rotation.y = (float)value;
    on_open_clicked();
}

void View::on_rotateZ_valueChanged(double value)
{
    cparams.angleParams.rotation.z = (float)value;
    on_open_clicked();
}

void View::resizeEvent(QResizeEvent *e)
{
    on_open_clicked();
}

void View::on_scale_valueChanged(double arg1)
{
    cparams.scale = arg1;
    on_open_clicked();
}
