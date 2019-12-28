#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

// 多线程
#include <QThread>
#include "data_define.h"
// 文件夹
#include "folder_manager.h"
#include "memory_manager.h"
#include "disk_manager.h"

class ProcessManager
{
public:
    ProcessManager();
    // 两个Manager
    DiskManager dManager;
    FolderManager fManager;
    MemoryManager mManager;



};







#endif // PROCESS_MANAGER_H
