#include "manage.h"
#include "ui_manage.h"
#include "mainwindow.h"
#include "globle.h"
#include "order.h"
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

manage::manage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::manage)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("民航票务管理与售票系统"));
    this->setWindowFlags(Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint);
    //返回功能，关联信号和槽
    connect(ui->pushButtonreturn,&QPushButton::clicked,this, &manage::sendSignal);

     if(QSqlDatabase::contains("qt_sql_default_connection"))
         db = QSqlDatabase::database("qt_sql_default_connection");
     else{
         QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
         db.setDatabaseName("../info.db");
     }
     if(!db.open())
     {
         QMessageBox::warning(this, "error", db.lastError().text());
     }
     //显示模型中的数据, 使用视图QTableView
     model = new QSqlTableModel(this);
     model->setTable("flight");
     model->setEditStrategy(QSqlTableModel::OnManualSubmit);  //设置model的修改模式
     ui->labelReallyName->setText(username_qj);
     ui->tableView->setSortingEnabled(true);
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

manage::~manage()
{
    delete ui;
}

void manage::sendSignal(){
    emit mySignal();
}

void manage::changeOrder(){
    this->show();
    od->hide();
}

//返回功能
void manage::on_pushButtonreturn_clicked()
{
    MainWindow *dh;
    this->hide();
    dh=new MainWindow;
    dh->show();
}

//退出功能
void manage::on_pushButtonexit_clicked()
{
    this->close();
}

//航班号查找
void manage::on_pushButtonNum_clicked()
{
    QString fid = ui->lineEdit->text();
    QString str = QString("fid = '%1'").arg(fid);
    model->setFilter(str);   //设置过滤条件
    model->select();
}

//起点查找
void manage::on_pushButtonStart_clicked()
{
    QString start = ui->lineEdit->text();
    QString str = QString("start = '%1'").arg(start);
    model->setFilter(str);
    model->select();
}

//取消功能
void manage::on_pushButtoncancel_clicked()
{
    model->revertAll();
    model->submitAll();
}

//提交修改功能
void manage::on_pushButtonsure_clicked()
{
    model->database().transaction();    //开启事务
    int ok = QMessageBox::warning(this,QStringLiteral("变更通知"),QStringLiteral("确定修改？"),QMessageBox::Yes,QMessageBox::No);
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

//删除功能
void manage::on_pushButtondelete_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();  //获取选中的model
    QModelIndexList list = select->selectedIndexes();   //选中的行的索引
    for(int i=0;i<list.size();i++){
        model->removeRow(list.at(i).row());
    }
    int ok = QMessageBox::warning(this,QStringLiteral("删除选中的行"),QStringLiteral("确定删除当前选取中的行?"),QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::Yes)
    {
        model->submitAll(); //提交，在数据库中删除该行
    } else {
        model->revertAll(); //如果不删除，则撤销
    }
}

//添加功能
void manage::on_pushButtonadd_clicked()
{
    QSqlRecord record = model->record();    //获取空记录
    int row = model->rowCount();            //获取行数
    model->insertRecord(row,record);        //进行添加
}

//刷新功能
void manage::refresh()
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

//刷新功能槽函数
void manage::on_pushButtonfresh_clicked()
{
   refresh();
}

//进入售票系统
void manage::on_pushButtonorder_clicked()
{
    od = new order;
    this->od->show();
}
