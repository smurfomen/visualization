#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QResizeEvent>
#include "computer.h"
namespace Ui {
class View;
}

class View : public QMainWindow
{
    Q_OBJECT

public:
    virtual void resizeEvent(QResizeEvent * e);
    explicit View(QWidget *parent = nullptr);
    ~View();

private slots:
    void on_openFile_clicked();

    void on_CalculateAndDraw_clicked();

    void on_loadData_clicked();

    void headerClicked(const QModelIndex& index);

private:

    void drawGraps(const std::vector<Graph>& graphs);
    void drawAxelsValues(const std::vector<Point2DValue> &axelsValues);
    Ui::View *ui;

};

#endif // VIEW_H
