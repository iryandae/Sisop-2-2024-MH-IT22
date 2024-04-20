#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define buffersize 1024

void replacestrings(const char *filepath, const char *log_path) {
    const char *suspects[] = {"m4LwAr3", "5pYw4R3", "R4nS0mWaR3"};
    const char *replacements[] = {"[MALWARE]", "[SPYWARE]", "[RANSOMWARE]"};
    char temp[] = "temp.txt";
    int input, output, log;
    char buffer[buffersize];
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);

    input = open(filepath, O_RDONLY);
    output = open(temp, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    log = open(log_path, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);

    while (read(input, buffer, buffersize) > 0) {
        char *tempbuffer = strdup(buffer);
        int replaced = 0;
        for (int i = 0; i < sizeof(suspects) / sizeof(suspects[0]); ++i) {
            char *found = strstr(tempbuffer, suspects[i]);
            if (found) {
                strncpy(found, replacements[i], strlen(replacements[i]) + 1);
                replaced = 1;
                dprintf(log, "[%02d-%02d-%04d][%02d:%02d:%02d] Suspicious string at %s successfully replaced!\n",
                        lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900,
                        lt->tm_hour, lt->tm_min, lt->tm_sec, filepath);
            }
        }
        write(output, tempbuffer, strlen(tempbuffer));
        free(tempbuffer);
    }

    close(input);
    close(output);
    close(log);

    remove(filepath);
    rename(temp, filepath);
}

void searchh(const char *dirpath, const char *log_path) {
    DIR *dir;
    struct dirent *ent;
    char path[1000];
    struct stat filestat;

    dir = opendir(dirpath);
    if (dir) {
        while ((ent = readdir(dir)) != NULL) {
            snprintf(path, sizeof(path), "%s/%s", dirpath, ent->d_name);
            stat(path, &filestat);
            if (S_ISREG(filestat.st_mode)) {
                replacestrings(path, log_path);
            }
        }
        closedir(dir);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    umask(0);

    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1) {
        searchh(argv[1], "virus.log");
        sleep(15);
    }
    exit(EXIT_SUCCESS);
}
