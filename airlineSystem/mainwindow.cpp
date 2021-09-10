#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globle.h"
#include "client.h"
#include "manage.h"
#include "order.h"
#include <QString>
#include <QRadioButton>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint);
    setWindowTitle(QStringLiteral("民航登录系统"));
    ui->lineEditPass->setEchoMode(QLineEdit::Password);
    ui->radioButtonstu->setChecked(true);   //默认选择用户

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");  //添加Sqlite数据库
    db.setDatabaseName("../info.db");//设置数据库
    //打开数据库
    if(!db.open()) //数据库打开失败
    {
        QMessageBox::warning(this, "错误", db.lastError().text());
        return;
    }
    //对数据库进行初始化(仅第一次需要)

    //创建admin表
    QSqlQuery query;
    query.exec("DROP TABLE IF EXISTS `admin`;");
    query.exec("CREATE TABLE `admin` (`id` int(12) NOT NULL DEFAULT '0',`username` text,`password` text,PRIMARY KEY (`id`));");
    query.exec("INSERT INTO `admin` VALUES ('1', 'admin', 'admin');");
    query.exec("INSERT INTO `admin` VALUES ('2', 'admin2', 'admin2');");
    query.exec("INSERT INTO `admin` VALUES ('3', 'admin3', 'admin3');");

    //创建usr表
    query.exec("DROP TABLE IF EXISTS `usr`;");
    query.exec("CREATE TABLE `usr` ("
                 "`id` text NOT NULL,"          //id
                 "`name` text,"                     //姓名
                 "`password` text DEFAULT '123456'," //密码
                 "PRIMARY KEY (`id`)"
               ");");
    query.exec(QStringLiteral("INSERT INTO `usr` VALUES ('u1001', '1001', '123456');"));
    query.exec(QStringLiteral("INSERT INTO `usr` VALUES ('u1002', '1002', '123456');"));
    query.exec(QStringLiteral("INSERT INTO `usr` VALUES ('u1003', '1003', '123456');"));

    //创建flight表
    query.exec("DROP TABLE IF EXISTS `flight`;");
    query.exec("CREATE TABLE `flight` ("
                 "`fid` text NOT NULL,"              //航班号
                 "`start` text,"                    //起点
                 "`end` text,"                      //终点
                 "`date` text,"                     //日期
                 "`start_time` text,"                //起飞时刻
                 "`end_time` text,"                 //到达时刻
                 "`seat` int(12),"                  //预设座位数
                 "`remain` int(12),"              //剩余座位数
                 "`price` int(12),"                   //票价
                 "`company` text DEFAULT '无'," //所属公司
                 "PRIMARY KEY (`fid`)"
               ");");

    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1001','北京','上海','2021-11-03','12:10:00','13:20:00',100,100,500,'东方航空');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1004','南京','重庆','2021-10-23','10:00:00','12:20:00',80,80,620,'南方航空');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1002','北京','海口','2021-09-20','22:40:00','00:30:00',98,98,500,'国航');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1005','天津','上海','2021-09-12','14:10:00','16:20:00',78,78,680,'东方航空');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1003','海南','昆明','2021-09-10','08:00:00','08:55:00',60,60,800,'东方航空');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1014','海南','黑龙江','2021-09-10','07:00:00','09:50:00',60,60,800,'南方航空');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1009','上海','大连','2021-09-10','07:00:00','09:50:00',60,60,750,'海南航空');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1019','成都','扬州','2021-09-10','07:10:00','09:50:00',60,60,700,'春秋航空');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1021','海南','成都','2021-09-10','07:00:00','09:50:00',60,60,900,'春秋航空');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1008','海南','大连','2021-09-10','07:00:00','09:50:00',60,60,800,'海南航空');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1006','南京','南宁','2021-09-08','11:20:00','13:00:00',60,60,670,'东方航空');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1007','深圳','上海','2021-10-02','20:30:00','21:40:00',87,87,580,'深圳航空');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1010','宁波','广州','2021-09-14','09:00:00','10:20:00',90,90,700,'南方航空');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1011','桂林','南京','2021-09-10','23:20:00','01:30:00',86,86,810,'东方航空');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1012','扬州','广州','2021-09-15','08:00:00','10:20:00',99,99,600,'春秋航空');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1013','扬州','广州','2021-09-16','08:00:00','10:20:00',99,99,600,'春秋航空');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1015','北京','西宁','2021-09-20','22:40:00','00:30:00',98,98,670,'国航');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1018','北京','海口','2021-09-21','22:40:00','00:30:00',98,98,800,'国航');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1020','北京','青海','2021-09-22','22:40:00','00:30:00',98,98,520,'东方航空');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1015','南京','桂林','2021-09-11','22:20:00','00:30:00',86,86,790,'东方航空');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1016','深圳','大连','2021-09-10','07:00:00','09:50:00',60,60,800,'海南航空');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1017','南京','北京','2021-09-00','11:20:00','13:00:00',60,60,670,'东方航空');"));
    query.exec(QStringLiteral("INSERT INTO `flight` VALUES ('1022','西安','南京','2021-10-02','19:30:00','21:40:00',87,87,780,'国航');"));


    /*
    //创建order表
    query.exec("DROP TABLE IF EXISTS `order`;");
    query.exec("CREATE TABLE `order`("
                 "`fid` text,"             //航班号
                 "`num` int(12),"                     //票数
                 "`buydate` text DEFAULT '2021-9-10'," //时间戳
                 "`name` text,"       //姓名
                 "`uid` text UNIQUE,"          //身份证
                 "`safe` text DEFAULT '否'"       //保险
               ");");
    query.exec(QStringLiteral("INSERT INTO `order` VALUES ('1001', '1', '2021-9-10','明明','211102198010121011','是');"));
*/
    db.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeClient(){
    this->show();
    sw->hide();
}

void MainWindow::changeManage(){
    this->show();
    tw->hide();
}

//登录功能槽函数
void MainWindow::on_pushButton_clicked()
{
    QString username = ui->lineEditNum->text();
    QString password = ui->lineEditPass->text();
    if(username==""||password==""){  //输入为空
        QMessageBox::information(this, QStringLiteral("提醒"),QStringLiteral("请按照格式正确填写用户名和密码！"));
        QString username = ui->lineEditNum->text();
        QString password = ui->lineEditPass->text();
    }
    //设置数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../info.db");

    //打开数据库
    if(!db.open()) //数据库打开失败
    {
        QMessageBox::warning(this, "错误", db.lastError().text());
        return;
    }
    bool in=false;
    if(ui->radioButtonTea->isChecked()){    //进入管理员模块
        QSqlQuery select;
        QString sql = QString("select * from admin where username = '%1' and password='%2';").arg(username,password);
        in = select.exec(sql);
        if(select.next()){
            this->hide();
            username_qj = username;
            password_qj = password;
            tw = new manage;
            this->tw->show();
        }
        else{
            QMessageBox::information(this, QStringLiteral("提醒"),QStringLiteral("用户名或密码有误！！！"));
        }
    }
    if(ui->radioButtonstu->isChecked()){    //进入用户模块
        QSqlQuery select;
        QString sql = QString("select * from usr where name = '%1' and password='%2';").arg(username,password);
        in =select.exec(sql);
        if(select.next()){
            db.removeDatabase(db.connectionName());
            this->hide();
            username_qj = username;
            password_qj = password;
            sw = new client;
            this->sw->show();
        }
        else{
            QMessageBox::information(this, QStringLiteral("提醒"),QStringLiteral("用户名或密码有误！"));
        }
    }
}

//退出功能
void MainWindow::on_pushButton_2_clicked()
{
    this->close();
}
