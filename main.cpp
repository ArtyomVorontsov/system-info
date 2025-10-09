#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./class/sys-info.h"
#include "./class/ui.h"
#include "./class/config.h"

int main()
{
    SysInfo *sysInfo = new SysInfo();

    Config::loadEnv(".system-info-config");

    UI *ui = new UI(Config::default_path, Config::color_scheme);
    ui->renderUi();
}