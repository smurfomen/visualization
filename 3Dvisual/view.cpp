#include "view.h"
#include "ui_view.h"
#include <QFileDialog>
#include <QDebug>
#include "computer.h"
#include <QVector2D>
#include <QVector3D>
#include "scene.h"


// параметры для калькуляции, подробнее в определении структуры
Settings cparams;

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
    if(ui->filePath->text().isEmpty())
    {
        QString path = QFileDialog::getOpenFileName(this, tr("Выбрать датасет"), tr("Датасеты(*.csv)"));
        ui->filePath->setText(path);
    }
    else
    {
        // обновляем размеры сцены на случай если они изменились, это нужно для обсчета
        cparams.w = ui->scene->width();
        cparams.h = ui->scene->height();
        cparams.filePath = ui->filePath->text();

        ui->scene->setChart(makeChartExecute(cparams));
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

void View::on_scale_valueChanged(double arg1)
{
    cparams.scale = arg1;
    on_open_clicked();
}

void View::resizeEvent(QResizeEvent *e)
{
    on_open_clicked();
}


