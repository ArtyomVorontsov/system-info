#include "./ui.h"
#include <ncurses.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "sys-info.h"

void UI::drawMenu()
{
    char buffer[1000];
    clear();

    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "Path: %s", current_path);
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(3));
    mvprintw(1, 0, "Navigate: UP/DOWN  Enter=Open  Backspace=Up  q=Quit");
    attroff(COLOR_PAIR(3));

    SysInfo::getCpuInfo(buffer);
    mvprintw(3, 0, buffer);

    SysInfo::getMemInfo(buffer);
    mvprintw(4, 0, buffer);

    SysInfo::getDiskInfo(buffer);
    mvprintw(5, 0, buffer);

    SysInfo::getGpuInfo(buffer);
    mvprintw(6, 0, buffer);

    SysInfo::getKernelInfo(buffer);
    mvprintw(7, 0, buffer);

    for (int i = 0; i < file_count; i++)
    {
        if (i == current_selection)
            attron(A_REVERSE);
        mvprintw(i + 9, 2, "%s", files[i]);
        if (i == current_selection)
            attroff(A_REVERSE);
    }
    refresh();
}

void UI::listDir(const char *path)
{
    DIR *dir;
    struct dirent *entry;

    // free old file list
    for (int i = 0; i < file_count; i++)
    {
        free(files[i]);
    }
    file_count = 0;

    dir = opendir(path);
    if (!dir)
        return;

    while ((entry = readdir(dir)) != NULL)
    {
        files[file_count] = strdup(entry->d_name);
        file_count++;
        if (file_count >= MAX_FILES)
            break;
    }
    closedir(dir);
}

int UI::renderUi()
{

    getcwd(current_path, sizeof(current_path));
    listDir(current_path);

    // Init colors
    initscr();
    start_color(); 
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    int ch;
    while (1)
    {
        drawMenu();
        ch = getch();

        switch (ch)
        {
        case KEY_UP:
            if (current_selection > 0)
                current_selection--;
            break;
        case KEY_DOWN:
            if (current_selection < file_count - 1)
                current_selection++;
            break;
        case 10:
        { // Enter key
            char new_path[PATH_MAX];
            snprintf(new_path, sizeof(new_path), "%s/%s", current_path, files[current_selection]);

            struct stat st;
            if (stat(new_path, &st) == 0 && S_ISDIR(st.st_mode))
            {
                strcpy(current_path, new_path);
                current_selection = 0;
                listDir(current_path);
            }
            break;
        }
        case 127: // Backspace
        case KEY_BACKSPACE:
        {
            char *slash = strrchr(current_path, '/');
            if (slash && slash != current_path)
            {
                *slash = '\0';
            }
            else
            {
                strcpy(current_path, "/");
            }
            current_selection = 0;
            listDir(current_path);
            break;
        }
        case 'q':
            endwin();
            return 0;
        }
    }
}
