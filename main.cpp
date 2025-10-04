#include "./class/sys-info.h"
#include "./class/ui.h"

int main()
{
    SysInfo *sysInfo = new SysInfo();
    UI *ui = new UI();
    ui->renderUi();
    // sysInfo->getCpuInfo();
    // sysInfo->getMemInfo();
    // sysInfo->getDiskInfo();
    // sysInfo->getGpuInfo();
    // sysInfo->getKernelInfo();
}