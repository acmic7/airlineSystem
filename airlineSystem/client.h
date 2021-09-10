#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QSqlTableModel>

namespace Ui {
class client;
}

class client : public QWidget
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = nullptr);
    ~client();
    void sendSignal();
    QSqlDatabase db;

signals:
    void mySignal();

private slots:
    void on_pushButtonreturn_clicked();

    void on_pushButtonquit_clicked();

    void on_pushButtonbuy_clicked();

    void on_pushButtonans_clicked();

    void on_pushButtonfresh_clicked();

private:
    Ui::client *ui;
    QSqlTableModel *model;
};

#endif // CLIENT_H

