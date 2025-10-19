#include "./sys-info.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sys/statvfs.h>
#include <cstdio>
#include <memory>
#include <array>
#include <sys/utsname.h>
#include <sys/time.h>

char *SysInfo::getCpuInfo(char *buffer)
{
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;

    while (std::getline(cpuinfo, line))
    {
        if (line.find("model name") != std::string::npos)
        {
            sprintf(buffer, "%s\n", line.c_str());
            break;
        }
    }

    return buffer;
}

char *SysInfo::getMemInfo(char *buffer)
{
    std::ifstream meminfo("/proc/meminfo");
    std::string key, unit;
    long value;
    while (meminfo >> key >> value >> unit)
    {
        if (key == "MemTotal:")
        {
            sprintf(buffer, "%s %ld %s\n", "Total RAM: ", value / 1024, " MB");
            break;
        }
    }

    return buffer;
}

char *SysInfo::getDiskInfo(char *buffer)
{
    struct statvfs stat;
    if (statvfs("/", &stat) == 0)
    {
        unsigned long long total = stat.f_blocks * stat.f_frsize;
        unsigned long long free = stat.f_bfree * stat.f_frsize;

        sprintf(buffer, "%s %llu %s %s %d %s\n",
                "Disk total: ",
                (total / (1024 * 1024 * 1024)),
                " GB\n",
                " Disk free : ",
                (free / (1024 * 1024 * 1024)),
                " GB");
    }

    return buffer;
}

char *SysInfo::getGpuInfo(char *buffer)
{
    std::array<char, 128> textBuffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("lspci | grep VGA", "r"), pclose);
    if (!pipe)
        exit(1);
    while (fgets(textBuffer.data(), textBuffer.size(), pipe.get()) != nullptr)
    {
        result += textBuffer.data();
    }

    sprintf(buffer, "%s %s\n", "GPU: ", result.c_str());

    return buffer;
}

char *SysInfo::getKernelInfo(char *buffer)
{
    struct utsname textBuffer;
    if (uname(&textBuffer) == 0)
    {
        sprintf(buffer, "%s %s %s %s\n", "OS: ", textBuffer.sysname, " ", textBuffer.release);
    }

    return buffer;
}

char *SysInfo::getSystemTime(char *buffer)
{
    timeval tv;

    if (gettimeofday(&tv, nullptr) == 0)
    {
        sprintf(buffer, "UNIX epoch time: %d\n", tv.tv_sec);
    }

    return buffer;
}