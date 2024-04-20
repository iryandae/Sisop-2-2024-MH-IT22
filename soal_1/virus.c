#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define size 1024

void replace(char *file) {
    const char *sus[] = {"m4LwAr3", "5pYw4R3", "R4nS0mWaR3"};
    const char *rep[] = {"[MALWARE]", "[SPYWARE]", "[RANSOMWARE]"};
    char temp[] = "temp.txt";
    FILE *f, *t, *l;
    char buffer[size];
    time_t ttime = time(NULL);
    struct tm *lt = localtime(&ttime);

    f = fopen(file, "r");
    t = fopen(temp, "w");
    l = fopen("virus.log", "a");

    while (fgets(buffer, size, f) != NULL) {
        char *b = strdup(buffer); // Create a duplicate buffer
        int r = 0;
        for (int i = 0; i < sizeof(sus) / sizeof(sus[0]); ++i) {
            char *found = strstr(b, sus[i]);
            if (found) {
                strncpy(found, rep[i], strlen(rep[i]));
                r = 1;
            }
        }
        if (r) {
            fprintf(l, "[%02d-%02d-%04d][%02d:%02d:%02d] Suspicious string at %s successfully replaced!\n",
                    lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900, lt->tm_hour, lt->tm_min, lt->tm_sec, file);
        }
        fprintf(t, "%s", b);
        free(b); // Free the duplicate buffer
    }

    fclose(f);
    fclose(t);
    fclose(l);

    remove(file);
    rename(temp, file);
}

void search(char *d) {
    DIR *dir;
    struct dirent *ent;
    char p[1000];
    struct stat s;

    dir = opendir(d);
    if (dir) {
        while ((ent = readdir(dir)) != NULL) {
            snprintf(p, sizeof(p), "%s/%s", d, ent->d_name);
            stat(p, &s);
            if (S_ISREG(s.st_mode)) {
                replace(p);
            }
        }
        closedir(dir);
    }
}

int main(int argc, char *argv[]) {
    pid_t p;

    p = fork();

    if (p < 0) {
        exit(EXIT_FAILURE);
    }

    if (p > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    close(fileno(stdin));
    close(fileno(stdout));
    close(fileno(stderr));

    while (1) {
        search(argv[1]);
        sleep(15);
    }

    exit(EXIT_SUCCESS);
}
