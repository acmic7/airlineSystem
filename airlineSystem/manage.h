#ifndef MANAGE_H
#define MANAGE_H

#include <QWidget>
#include<QSqlTableModel>
#include<order.h>

namespace Ui {
class manage;
}

class manage : public QWidget
{
    Q_OBJECT

public:
    explicit manage(QWidget *parent = nullptr);
    ~manage();
    QSqlDatabase db;
    void refresh();
    void sendSignal();
    void changeOrder();

signals:
    void mySignal();

private slots:
    void on_pushButtonreturn_clicked();

    void on_pushButtonexit_clicked();

    void on_pushButtonNum_clicked();

    void on_pushButtonStart_clicked();

    void on_pushButtoncancel_clicked();

    void on_pushButtonsure_clicked();

    void on_pushButtondelete_clicked();

    void on_pushButtonadd_clicked();

    void on_pushButtonfresh_clicked();

    void on_pushButtonorder_clicked();

private:
    Ui::manage *ui;
    QSqlTableModel *model;
    order *od;
};

#endif // MANAGE_H
