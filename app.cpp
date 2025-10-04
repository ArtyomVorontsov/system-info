#include <ncurses.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

#define MAX_FILES 1024

char *files[MAX_FILES];
int file_count = 0;
int current_selection = 0;
char current_path[PATH_MAX];

void listDir(const char *path) {
    DIR *dir;
    struct dirent *entry;

    // free old file list
    for (int i = 0; i < file_count; i++) {
        free(files[i]);
    }
    file_count = 0;

    dir = opendir(path);
    if (!dir) return;

    while ((entry = readdir(dir)) != NULL) {
        files[file_count] = strdup(entry->d_name);
        file_count++;
        if (file_count >= MAX_FILES) break;
    }
    closedir(dir);
}

void draw_menu() {
    clear();
    mvprintw(0, 0, "Path: %s", current_path);
    mvprintw(1, 0, "Navigate: ↑ ↓  Enter=Open  Backspace=Up  q=Quit");

    for (int i = 0; i < file_count; i++) {
        if (i == current_selection)
            attron(A_REVERSE);
        mvprintw(i + 3, 2, "%s", files[i]);
        if (i == current_selection)
            attroff(A_REVERSE);
    }
    refresh();
}

int main() {
    getcwd(current_path, sizeof(current_path));
    listDir(current_path);

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    int ch;
    while (1) {
        draw_menu();
        ch = getch();

        switch (ch) {
            case KEY_UP:
                if (current_selection > 0) current_selection--;
                break;
            case KEY_DOWN:
                if (current_selection < file_count - 1) current_selection++;
                break;
            case 10: { // Enter key
                char new_path[PATH_MAX];
                snprintf(new_path, sizeof(new_path), "%s/%s", current_path, files[current_selection]);
                
                struct stat st;
                if (stat(new_path, &st) == 0 && S_ISDIR(st.st_mode)) {
                    strcpy(current_path, new_path);
                    current_selection = 0;
                    listDir(current_path);
                }
                break;
            }
            case 127: // Backspace
            case KEY_BACKSPACE: {
                char *slash = strrchr(current_path, '/');
                if (slash && slash != current_path) {
                    *slash = '\0';
                } else {
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
