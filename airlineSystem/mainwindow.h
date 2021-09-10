#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"manage.h"
#include"client.h"
#include"order.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void changeClient();
    void changeManage();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    client *sw;
    manage *tw;
    order *od;
};

#endif // MAINWINDOW_H
