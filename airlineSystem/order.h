#ifndef ORDER_H
#define ORDER_H

#include <QWidget>
#include <QSqlTableModel>
#include <order.h>

namespace Ui {
class order;
}

class order : public QWidget
{
    Q_OBJECT

public:
    explicit order(QWidget *parent = nullptr);
    ~order();
    QSqlDatabase db;

private slots:
    void on_pushButtonreturn_clicked();

    void on_pushButtonfresh_clicked();

    void on_pushButtonsure_clicked();

    void on_pushButtonadd_clicked();

    void on_pushButtondelete_clicked();

private:
    Ui::order *ui;
    QSqlTableModel *model;
    order *od;
};

#endif // ORDER_H


