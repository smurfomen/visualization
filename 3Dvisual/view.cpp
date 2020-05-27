#include "view.h"
#include "ui_view.h"
#include <QFileDialog>
#include <QDebug>
#include "computer.h"
#include <QVector2D>
#include <QVector3D>


int columns = 0;
int rows = 0;
double angleX = 45.0;
double angleY = -45.0;
double angleZ = 45.0;
int bR = 0;
int tR = 1;
double min = 0.0;
double max = 0.0;


View::View(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::View)
{
    ui->setupUi(this);
    ui->gview->setFixedSize(ui->gview->size());
    ui->gview->setScene(new QGraphicsScene(this));
    ui->gview->setSceneRect(ui->gview->rect());
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

        QStringList fieldsOfRow = QString(file.readLine()).split(",");
        min = max = fieldsOfRow.at(0).toDouble();
        columns = fieldsOfRow.size();
        rows = 1;
        while(!file.atEnd())
        {
            for(int x = 0; x < fieldsOfRow.size(); x++)
            {
                double val = fieldsOfRow.at(x).toDouble();
                if(min > val)
                    min = val;
                else if(max < val)
                    max = val;
                nodes.append(QVector3D(x,rows,val));
                fieldsOfRow = QString(file.readLine()).split(",");
            }
            rows++;
        }
        file.close();
        Compute(ui->gview->scene(),ui->gview->scene()->width(),ui->gview->scene()->height(), nodes);


    }
}










void View::on_bottomRange_valueChanged(int arg1)
{
    bR = arg1;
}

void View::on_upperRange_valueChanged(int arg1)
{
    tR = arg1;
}
