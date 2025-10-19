class SysInfo {
    public: 
        static char *getCpuInfo(char *buffer);
        static char *getMemInfo(char *buffer);
        static char *getDiskInfo(char *buffer);
        static char *getGpuInfo(char *buffer);
        static char *getKernelInfo(char *buffer);
        static char *getSystemTime(char *buffer);
};