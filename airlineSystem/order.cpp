#include "order.h"
#include "ui_order.h"
#include "mainwindow.h"
#include "globle.h"
#include <order.h>
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QItemSelectionModel>
#include <QModelIndexList>
#include <QMessageBox>
#include <QString>

order::order(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::order)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("售票系统"));
    this->setWindowFlags(Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint);

    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else{
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("../info.db");
        qDebug()<<"===success===";
    }
    model = new QSqlTableModel(this);
    model->setTable("test1");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit); //设置model的修改模式
    ui->tableVieworder->setModel(model);
    QString str = QString("name = '%1'").arg(username_qj);
    ui->tableVieworder->setSortingEnabled(true);
    model->select();    //显示表
    model->setHeaderData(0, Qt::Horizontal, QStringLiteral("身份证号"));
    model->setHeaderData(1, Qt::Horizontal, QStringLiteral("姓名"));
    model->setHeaderData(2, Qt::Horizontal, QStringLiteral("航班号"));
    model->setHeaderData(3, Qt::Horizontal, QStringLiteral("票数"));
    model->setHeaderData(4, Qt::Horizontal, QStringLiteral("是否保险"));
    model->setHeaderData(5, Qt::Horizontal, QStringLiteral("购票时间"));
}

order::~order()
{
    delete ui;
}

//返回功能
void order::on_pushButtonreturn_clicked()
{
    this->close();
}

//刷新功能
void order::on_pushButtonfresh_clicked()
{
    model = new QSqlTableModel(this);
    model->setTable("test1");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit); //设置model的修改模式
    ui->tableVieworder->setModel(model);
    QString str = QString("name = '%1'").arg(username_qj);
    ui->tableVieworder->setSortingEnabled(true);
    model->select();    //显示表
    model->setHeaderData(0, Qt::Horizontal, QStringLiteral("身份证号"));
    model->setHeaderData(1, Qt::Horizontal, QStringLiteral("姓名"));
    model->setHeaderData(2, Qt::Horizontal, QStringLiteral("航班号"));
    model->setHeaderData(3, Qt::Horizontal, QStringLiteral("票数"));
    model->setHeaderData(4, Qt::Horizontal, QStringLiteral("是否保险"));
    model->setHeaderData(5, Qt::Horizontal, QStringLiteral("购票时间"));
}

//修改功能
void order::on_pushButtonsure_clicked()
{
    model->database().transaction();    //开启事务
    int ok = QMessageBox::warning(this,QStringLiteral("信息更改通知"),QStringLiteral("确定修改信息？"),QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::Yes)
    {
        if(model->submitAll())
        {
            model->database().commit();
        }
    }
    else
    {
        model->database().rollback();   //提交过程中一旦出错进行回滚
        QMessageBox::warning(this, tr("提醒"),QStringLiteral("修改未完成！%1").arg(model->lastError().text()));
    }
}
//添加功能
void order::on_pushButtonadd_clicked()
{
    QSqlRecord record = model->record();    //获取空记录
    int row = model->rowCount();            //获取行数
    model->insertRecord(row,record);        //进行添加
}

//退票功能
void order::on_pushButtondelete_clicked()
{
    QString str_uid = ui->lineEdituid->text();  //身份证
    QString str_fid = ui->lineEditfid->text();  //航班号
    QString str_num = ui->lineEditnum->text();  //票数

    if(str_uid == NULL && str_fid == NULL && str_num == NULL)
        QMessageBox::warning(this,QStringLiteral("提醒"),QStringLiteral("信息不能为空！"),QMessageBox::Yes);
    else
    {
        QSqlQuery sql_query;
        QString select_sql = "select uid from test1"; //查找对应个人信息
        if(!sql_query.exec(select_sql))
            qDebug()<<sql_query.lastError();
        else
        {
            while(sql_query.next())
            {
                QString id = sql_query.value(0).toString();
                if(id == str_uid)
                {
                    bool identy = true;
                    QString delete_sql = "delete from test1 where uid = ?"; //删除信息
                    sql_query.prepare(delete_sql);
                    sql_query.addBindValue(id);
                    if(!sql_query.exec())
                    {
                        qDebug()<<sql_query.lastError();
                    }
                    else
                    {
                        qDebug()<<"deleted!";
                        QString update_sql = "update flight set remain = remain + :num where fid = :fid"; //票务信息更改
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
                        QMessageBox::warning(this,QStringLiteral("退票"),QStringLiteral("退票成功！"),QMessageBox::Yes);
                    }
                }

            }
        }
    }
}
