#include "view.h"
#include "ui_view.h"
#include <QFileDialog>
#include <QDebug>
#include "computer.h"
#include <QVector2D>
#include <QVector3D>


int columns = 0;
int rows = 0;

float angleX = 67.0f;
float angleY = 25.0f;
float angleZ = 89.0f;

float rotateX = 67.0f;
float rotateY = 25.0f;
float rotateZ = 89.0f;

int bottomPeackNormal = 0;
int upperPeackNormal = 1;
double min = 0.0;
double max = 0.0;
float SPACE = 10.0f;
View::View(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::View)
{
    ui->setupUi(this);
    ui->gview->setFixedSize(ui->gview->size());
    ui->gview->setScene(new QGraphicsScene(this));
    ui->gview->setSceneRect(ui->gview->rect());

    rotateX = ui->rotateX->value();
    rotateY = ui->rotateY->value();
    rotateZ = ui->rotateZ->value();

    angleX = ui->angleX->value();
    angleY = ui->angleY->value();
    angleZ = ui->angleZ->value();

    upperPeackNormal = ui->upperRange->value();
    bottomPeackNormal = ui->bottomRange->value();

    SPACE = ui->space->value();

}

View::~View()
{
    delete ui;
}

void View::on_open_clicked()
{
    QString path = /*QFileDialog::getOpenFileName(this, tr("Выбрать датасет"), "/home/vladimir", tr("Датасеты(*.csv)"));*/ "students_exams.csv";
    ui->filePath->setText(path);
    QFile file(path);
    if(file.open(QFile::ReadOnly))
    {
        QVector<QVector3D> nodes;

        columns  = 0;
        rows = 0;
        while(!file.atEnd())
        {
            QStringList fieldsOfRow = QString(file.readLine()).split(",");
            if(rows == 0)
                min = max = fieldsOfRow.at(0).toDouble();

            columns = 0;
            for(; columns < fieldsOfRow.size(); columns++)
            {
                double val = fieldsOfRow.at(columns).toDouble();
                if(min > val)
                    min = val;
                else if(max < val)
                    max = val;
                nodes.append(QVector3D(columns,rows,val));
            }
            rows++;
        }

        file.close();
        Compute(ui->gview->scene(),ui->gview->width(),ui->gview->height(), nodes);
    }
}










void View::on_bottomRange_valueChanged(int arg1)
{
    bottomPeackNormal = arg1;
    on_open_clicked();
}

void View::on_upperRange_valueChanged(int arg1)
{
    upperPeackNormal = arg1;
    on_open_clicked();
}

void View::on_angleX_valueChanged(double arg1)
{
    angleX = (float)arg1;
    on_open_clicked();
}

void View::on_angleY_valueChanged(double arg1)
{
    angleY = (float)arg1;
    on_open_clicked();
}

void View::on_angleZ_valueChanged(double arg1)
{
    angleZ = (float)arg1;
    on_open_clicked();
}

void View::on_space_valueChanged(int arg1)
{
    SPACE = (float)arg1;
    on_open_clicked();
}


void View::on_rotateX_valueChanged(double value)
{
    rotateX = (float)value;
    on_open_clicked();
}

void View::on_rotateY_valueChanged(double value)
{
    rotateY = (float)value;
    on_open_clicked();
}

void View::on_rotateZ_valueChanged(double value)
{
    rotateZ = (float)value;
    on_open_clicked();
}
