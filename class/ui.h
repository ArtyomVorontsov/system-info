

#ifndef _LIMITS_H
#define _LIMITS_H 1
#include <limits.h>
#endif

#define MAX_FILES 1024

class UI
{
public:
    UI(char *current_path, int color_scheme);
    int renderUi();

private:
    char *files[MAX_FILES];
    int file_count = 0;
    int current_selection = 0;
    int color_scheme;
    char current_path[PATH_MAX];
    void drawMenu();
    void listDir(const char *path);
    void printPath();
    void printNav();
};