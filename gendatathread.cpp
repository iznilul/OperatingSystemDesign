#include "gendatathread.h"
#include <QLabel>



// 构造函数
GenDataThread::GenDataThread()
{

}

ExeDataThread::ExeDataThread()
{

}

DelDataThread::DelDataThread()
{

}

// 多线程
void GenDataThread::run()
{
    qDebug()<<"开始运行数据生成线程";
    //等待对话框
    QDialog * dialog = new QDialog();
    QVBoxLayout*  layout = new QVBoxLayout();
    layout->addWidget(new QLabel("等待*数据生成线程*结束..."));
    dialog->setLayout(layout);
    emit showWaitDialog(dialog);
    CGlobal::genThreadSem->acquire();
    qDebug()<<"开始运行";
    // 显示等待加载框
    emit closeWaitDialog(dialog);
    // 打开MainWindow
    emit openUI();
    qDebug()<<"运行结束";
    this->exec();
}


void DelDataThread::run()
{
    qDebug()<<"开始运行数据删除线程";
    //等待对话框
    QDialog * dialog = new QDialog();
    QVBoxLayout*  layout = new QVBoxLayout();
    layout->addWidget(new QLabel("等待*数据删除线程*结束..."));
    dialog->setLayout(layout);
    emit showWaitDialog(dialog);
    CGlobal::delThreadSem->acquire();
    qDebug()<<"开始运行";
    // 显示等待加载框
    emit closeWaitDialog(dialog);
    emit openUI();
    qDebug()<<"运行结束";
    this->exec();
}

// 执行进程，互斥访问内存
void ExeDataThread::run()
{
    TCB* tcb = new TCB();
    int status = CGlobal::mManager->allocMemory(tcb);
        // 先申请4块内容
    if (status == STATUS_OK){
        emit openUI(tcb);
    } else {
        emit showMessage("内存不足，无法打开更多的执行线程，请关闭一些执行线程再重试");
    }
    emit notify();
    this->exec();
}


