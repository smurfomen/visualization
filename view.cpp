#include "view.h"
#include "ui_view.h"
#include <QFileDialog>
#include <QDebug>
#include "computer.h"
#include <QGraphicsTextItem>
View::View(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::View)
{
    ui->setupUi(this);
    ui->gview->setRenderHint(QPainter::Antialiasing);
    ui->gview->setScene(new QGraphicsScene(this));
    ui->gview->scene()->setSceneRect(ui->gview->rect());

//    ui->filePath->setText("russian_demography.csv");
//    ui->region->setText("Stavropol Krai");
    this->setWindowTitle("ЛР 3. 2D визуализация.");
}


View::~View()
{
    delete ui;
}

void View::on_openFile_clicked()
{
    ui->filePath->setText(QFileDialog::getOpenFileName(this, tr("Выбрать датасет"), "/home/vladimir", tr("Датасеты(*.csv)")));
}

void View::on_CalculateAndDraw_clicked()
{
    QFile file(ui->filePath->text());
    int column = ui->column->value()-1;
    QString region = ui->region->text();
    int w = ui->gview->width(), h = ui->gview->height();
    QString nameColumn = ui->table->rowCount() > 0 ? ui->table->horizontalHeaderItem(column)->text() : "";

    ui->gview->scene()->clear();
    ui->gview->scene()->addText(nameColumn)->moveBy(w-DMARGIN,0);
    ui->gview->scene()->addText(region)->moveBy(DMARGIN, 0);
    ComputeAndDraw(ui->gview->scene(), w, h, &file, region.toStdString().c_str(), column);
}

void View::on_loadData_clicked()
{
    QString path = ui->filePath->text();
    if (!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QFile::ReadOnly))
        {
            QString head(file.readLine());
            QStringList columns = head.split(",");
            ui->table->setHorizontalHeaderLabels(columns);
            ui->table->horizontalHeader()->setVisible(true);
            ui->table->horizontalHeader()->adjustSize();

            connect(ui->table, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(headerClicked(const QModelIndex &)));


            QTableWidget * t = ui->table;
            while(!file.atEnd())
            {
                QStringList fields(QString(file.readLine()).split(","));
                t->insertRow(0);
                for(int c = 0; c < fields.size(); c++)
                    t->setItem(0,c, new QTableWidgetItem(fields.at(c)));
            }

            file.close();
        }
    }
}

void View::headerClicked(const QModelIndex &index)
{
    if(index.column() >= 7)
        return;
    if(index.column() == 1)
        ui->region->setText(index.data().toString());
    else if (index.column() > 1)
        ui->column->setValue(index.column()+1);
    on_CalculateAndDraw_clicked();

}
