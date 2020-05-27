#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QResizeEvent>

namespace Ui {
class View;
}

class View : public QMainWindow
{
    Q_OBJECT

public:
    explicit View(QWidget *parent = nullptr);
    ~View();

private slots:
    void on_openFile_clicked();

    void on_CalculateAndDraw_clicked();

    void on_loadData_clicked();

    void headerClicked(const QModelIndex& index);

private:
    Ui::View *ui;

};

#endif // VIEW_H
