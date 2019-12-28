#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QAbstractButton>
#include <QTabWidget>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //1.设置UI界面
    // 设置菜单
    QMenuBar * bar = new QMenuBar();
    bar->setParent(this);
    setMenuBar(bar);
    // 第一个菜单，
    QMenu * menu1 = new QMenu();
    menu1->addAction("关于",[=](){
       QMessageBox  * message = new QMessageBox();
       message->setText("任务管理器V1.0 By Kingtous");
       message->addButton("确定",QMessageBox::AcceptRole);
       message->addButton("取消",QMessageBox::DestructiveRole);
       message->show();
    });
    menu1->addAction("退出",[=](){
        // 退出功能
        this->close();
    });
    menu1->setTitle("功能");
    bar->addMenu(menu1);

    // Tab选项
    QTabWidget * tabWidget = new QTabWidget();
    tabWidget->setParent(this);
    tabWidget->setFixedSize(this->width(),this->height());



}


MainWindow::~MainWindow()
{
    delete ui;
}