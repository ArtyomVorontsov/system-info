#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "config.h"

char *Config::default_path = "/";
int Config::color_scheme = 0;

void Config::loadEnv(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
        return;

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        char *eq = strchr(line, '=');
        if (!eq)
            continue;

        *eq = '\0';
        char *key = line;
        char *value = eq + 1;

        // Remove newline
        value[strcspn(value, "\r\n")] = 0;

        setenv(key, value, 1);
    }

    fclose(file);

    Config::default_path = getenv("DEFAULT_PATH");
    Config::color_scheme = atoi(getenv("COLOR_SCHEME"));
}
