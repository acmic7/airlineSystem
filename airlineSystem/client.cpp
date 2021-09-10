#include "client.h"
#include "ui_client.h"
#include "mainwindow.h"
#include "globle.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QtDebug>
#include <QString>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QItemSelectionModel>
#include <QModelIndexList>
#include <QMessageBox>
#include <QString>

client::client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint);
    connect(ui->pushButtonreturn,&QPushButton::clicked,this, &client::sendSignal);  //返回主页
    setWindowTitle(QStringLiteral("民航票务管理与售票系统"));

    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else{
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("../info.db");
        qDebug()<<"===success===";
    }
    if(!db.open())
    {
        //QMessageBox::warning(this, "error", db.lastError().text());
    }
    model = new QSqlTableModel(this);
    model->setTable("flight");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit); //设置model的修改模式
    ui->labelReallyName->setText(username_qj);
    ui->tableView->setModel(model);

    QString str = QString("name = '%1'").arg(username_qj);
    ui->tableView->setSortingEnabled(true);

    model->select();    //显示表
    model->setHeaderData(0, Qt::Horizontal, QStringLiteral("航班号"));
    model->setHeaderData(1, Qt::Horizontal, QStringLiteral("起点"));
    model->setHeaderData(2, Qt::Horizontal, QStringLiteral("终点"));
    model->setHeaderData(3, Qt::Horizontal, QStringLiteral("日期"));
    model->setHeaderData(4, Qt::Horizontal, QStringLiteral("起飞时间"));
    model->setHeaderData(5, Qt::Horizontal, QStringLiteral("到达时间"));
    model->setHeaderData(6, Qt::Horizontal, QStringLiteral("预设座位数"));
    model->setHeaderData(7, Qt::Horizontal, QStringLiteral("剩余座位数"));
    model->setHeaderData(8, Qt::Horizontal, QStringLiteral("票价"));
    model->setHeaderData(9, Qt::Horizontal, QStringLiteral("航班所属公司"));
}

client::~client()
{
    delete ui;
}

void client::sendSignal(){
    emit mySignal();
}

//返回功能
void client::on_pushButtonreturn_clicked()
{
    MainWindow *dh;
    this->hide();
    dh=new MainWindow;
    dh->show();
}

//退出功能
void client::on_pushButtonquit_clicked()
{
    this->close();
}

//购票功能
void client::on_pushButtonbuy_clicked()
{
    QString str_fid = ui->lineEditfid->text();  //获取需要订票的航班号
    QString str_num = ui->lineEditnum->text();  //票数
    QString str_name = ui->lineEditname->text();  //姓名
    QString str_uid = ui->lineEdituid->text();   //身份证
    QString str_safe = ui->lineEditsafe->text();  //保险
    QString str_time = "2021-09-10";
    if((str_fid == NULL || str_num == NULL || str_name == NULL || str_uid == NULL || str_safe == NULL))
    {
        QMessageBox::information(this, QStringLiteral("提醒"),QStringLiteral("购票前需要填写完整信息！"));
    }
    else
    {
        QMessageBox::warning(this,QStringLiteral("购票"),QStringLiteral("确定购票?"),QMessageBox::Yes,QMessageBox::No);
        int ok = QMessageBox::warning(this,QStringLiteral("支付"),QStringLiteral("交易中..."),QMessageBox::Yes,QMessageBox::No);
        if(ok == QMessageBox::Yes)
        {
    /*
            QSqlQuery sql_d;
            QString clear_sql = "delete from ticket";
            sql_d.prepare(clear_sql);
            if(!sql_d.exec())
            {
                qDebug() << sql_d.lastError();
            }
            else
            {
                qDebug() << "table cleared";
            }
    */
            QSqlQuery sql_q; //建表
            QString create_sql = "create table test1 (uid varchar(20) primary key, name varchar(20), fid int, num int, safe varchar(20),time varchar(20))";
            sql_q.prepare(create_sql);
            if(!sql_q.exec())
            {
                qDebug() << "Error: Fail to create table." << sql_q.lastError();
            }
            else
            {
                qDebug() << "Table created!";
            }

            QString insert_sql = "insert into test1 values (?, ?, ?, ?, ?, ?)";  //增加记录
            sql_q.prepare(insert_sql);
            sql_q.addBindValue(str_uid);
            sql_q.addBindValue(str_name);
            sql_q.addBindValue(str_fid);
            sql_q.addBindValue(str_num);
            sql_q.addBindValue(str_safe);
            sql_q.addBindValue(str_time);
            if(!sql_q.exec())
            {
                qDebug() << sql_q.lastError();
            }
            else
            {
                qDebug() << "inserted!";
            }

            //更新航班记录
            QSqlQuery sql_query;
            QString update_sql = "update flight set remain = remain - :num where fid = :fid";
            sql_query.prepare(update_sql);
            sql_query.bindValue(":fid", str_fid);
            sql_query.bindValue(":num", str_num);

            if(!sql_query.exec())
            {
                qDebug() << sql_query.lastError();
            }
            else
            {
                qDebug() << "updated!";
            }

            QMessageBox::warning(this,QStringLiteral("支付"),QStringLiteral("购票成功！"),QMessageBox::Yes);

            //打印订单信息
            QSqlQuery sql_query4;
            QString select_all_sql = "select * from test1";
            sql_query4.prepare(select_all_sql);
            if(!sql_query4.exec())
            {
                qDebug()<<"select error!"<<sql_query4.lastError();
            }
            else
            {
                while(sql_query4.next())
                {
                    QString str_uid = sql_query4.value(0).toString();
                    QString str_name = sql_query4.value(1).toString();
                    int str_fid = sql_query4.value(2).toInt();
                    int str_num = sql_query4.value(3).toInt();
                    QString str_safe = sql_query4.value(4).toString();
                    QString str_time = sql_query4.value(5).toString();
                    qDebug()<<QString("身份证号:%1 姓名:%2 航班号:%3 票数:%4 是否需要保险:%5 购票时间:%6").arg(str_uid).arg(str_name).arg(str_fid).arg(str_num).arg(str_safe).arg(str_time);
                }
            }
            model->submitAll(); //提交，在数据库中删除该行
        } else {
            model->revertAll(); //如果不删除，则撤销
        }
    }

}

//查询功能
void client::on_pushButtonans_clicked()
{
    QString start = ui->lineEditsta->text();
    QString end = ui->lineEditend->text();
    QString date = ui->lineEditdat->text();
    if(start == NULL || end == NULL || date ==NULL)
    {
        QMessageBox::information(this, QStringLiteral("提醒"),QStringLiteral("查询条件不完整！"));
    }
    else
    {
        QString str = QString("start = '%1' and end = '%2' and date= '%3'").arg(start).arg(end).arg(date);
        model->setFilter(str);
        model->select();
    }
}

//刷新功能
void client::on_pushButtonfresh_clicked()
{
    model = new QSqlTableModel(this);
    model->setTable("flight");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit); //设置model的修改模式
    ui->labelReallyName->setText(username_qj);
    ui->tableView->setModel(model);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, QStringLiteral("航班号"));
    model->setHeaderData(1, Qt::Horizontal, QStringLiteral("起点"));
    model->setHeaderData(2, Qt::Horizontal, QStringLiteral("终点"));
    model->setHeaderData(3, Qt::Horizontal, QStringLiteral("日期"));
    model->setHeaderData(4, Qt::Horizontal, QStringLiteral("起飞时间"));
    model->setHeaderData(5, Qt::Horizontal, QStringLiteral("到达时间"));
    model->setHeaderData(6, Qt::Horizontal, QStringLiteral("预设座位数"));
    model->setHeaderData(7, Qt::Horizontal, QStringLiteral("剩余座位数"));
    model->setHeaderData(8, Qt::Horizontal, QStringLiteral("票价"));
    model->setHeaderData(9, Qt::Horizontal, QStringLiteral("航班所属公司"));
}
