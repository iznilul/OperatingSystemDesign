#include "window_exe_data.h"
#include "ui_window_exe_data.h"

window_exe_data::window_exe_data(QWidget *parent,TCB* tcb) :
    QWidget(parent),
    ui(new Ui::window_exe_data)
{
    ui->setupUi(this);
    this->tcb = tcb;
    fileSelected = nullptr;
    this->setWindowTitle("数据执行");
    // 关闭即销毁
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    // 定义
    lv_fileList = ui->lv_fileList;
    ll_owner = ui->text_owner;
    ll_fileName = ui->text_fileName;
    ll_fileSize = ui->text_fileSize;
    ll_createTime = ui->text_createTime;
    ll_loadFile = ui->text_loadFiles;
    nb_pageNumber = ui->nb_pageNumber;

    model = new QStandardItemModel();
    lv_fileList->setModel(model);
    connect(lv_fileList,SIGNAL(clicked(QModelIndex)),this,SLOT(showClick(QModelIndex)));

    updateFiles();
}

window_exe_data::~window_exe_data()
{
    delete ui;
}

// 读入按钮被按下
void window_exe_data::on_pushButton_clicked()
{
    // 加锁
    CGlobal::mSem->acquire();
    //ui->pushButton->setDisabled(true);

    // 获取页号数
    int page = nb_pageNumber->value();
    // 在内存中读取数据
    string value = CGlobal::mManager->read(tcb,page);


    // 解锁
    CGlobal::mSem->release();
}

// 更新文件列表
void window_exe_data::updateFiles()
{
    qDebug() << "执行线程：更新文件列表";
    model->clear();
    queue<FCB*> q = CGlobal::fManager->getFiles();
    // 备份一份
    while(!q.empty()){
        FCB* fcb = q.front();
        QStandardItem * item = new QStandardItem();
        item->setEditable(false);
        item->setText(QString::fromStdString(fcb->fileName));
        model->appendRow(item);
        q.pop();
        tmpFCB.push_back(fcb);
    }

}

// 锁定一个数据执行线程只能访问一个文件，要访问第二个文件需要打开多个窗口
void window_exe_data::on_btn_load_clicked()
{
    // fileSelected 指向选中文件的指针
    if(fileSelected == nullptr){
        auto box = new QMessageBox();
        box->setText("未选择文件");
        box->exec();
    } else {
        // 在创建数据执行线程时，已经在FCB中分配了4个内存，所以直接禁用调入内存等
        ((QPushButton*)sender())->setEnabled(false);
        lv_fileList->setDisabled(true);
        ll_loadFile->setText(QString::fromStdString(fileSelected->fileName));

        // 设置页号范围
        int blockSize = fileSelected->fileSize;
        if (blockSize <= 0){
            nb_pageNumber->setRange(0,0);
            // 不允许读入
        } else {
            nb_pageNumber->setRange(0,blockSize-1);
            btnRead->setEnabled(true);
        }
    }
}

void window_exe_data::showClick(QModelIndex index)
{
    FCB* fcb = tmpFCB.at(index.row());
    ll_owner->setText(QString::fromStdString(fcb->owner));
    ll_fileName->setText(QString::fromStdString(fcb->fileName));
    ll_fileSize->setText(QString::number(fcb->fileSize));
    ll_createTime->setText(parseTM(fcb->createTime));
    fileSelected = fcb;
}

void window_exe_data::closeEvent(QCloseEvent *event)
{
    // TODO 清除内存，先获取信号量
    CGlobal::mSem->acquire();
    int status = CGlobal::mManager->freeBlock(this->tcb);
    if(status == STATUS_OK){
        qDebug() << "内存管理器返回正常，内存已清除" ;
    } else {
        qDebug() << "警告！内存泄露，清除失败" ;
    }
    CGlobal::mSem->release();
    // 发射信号
    emit notifyUpdate();
}
