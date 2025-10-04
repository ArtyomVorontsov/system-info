

#ifndef _LIMITS_H
#define _LIMITS_H 1
#include <limits.h>
#endif

#define MAX_FILES 1024

class UI
{
private:
    char *files[MAX_FILES];
    int file_count = 0;
    int current_selection = 0;
    char current_path[PATH_MAX];
    void drawMenu();
    void listDir(const char *path);

public:
    int renderUi();
    
};