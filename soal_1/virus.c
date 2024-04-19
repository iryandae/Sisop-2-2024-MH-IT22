#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>

#define BUFFER_SIZE 1024
#define LOG_FILE "virus.log"
#define SUSPICIOUS_STRINGS_COUNT 3
#define SUSPICIOUS_STRINGS {"m4LwAr3", "5pYw4R3", "R4nS0mWaR3"}
#define REPLACEMENTS {"[MALWARE]", "[SPYWARE]", "[RANSOMWARE]"}

char log_path[1024]; // Path for log file

void log_change(const char *filename) {
    FILE *log = fopen(log_path, "a");
    if (log != NULL) {
        time_t now;
        time(&now);
        struct tm *local = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%d-%m-%Y %H:%M:%S", local);
        fprintf(log, "[%s] Suspicious string at %s successfully replaced!\n", timestamp, filename);
        fclose(log);
    }
}

void replace_string(const char *filename) {
    const char *suspicious_strings[SUSPICIOUS_STRINGS_COUNT] = SUSPICIOUS_STRINGS;
    const char *replacements[SUSPICIOUS_STRINGS_COUNT] = REPLACEMENTS;
    char buffer[BUFFER_SIZE];
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    int i;
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        for (i = 0; i < SUSPICIOUS_STRINGS_COUNT; i++) {
            char *pos = strstr(buffer, suspicious_strings[i]);
            while (pos != NULL) {
                fseek(file, pos - buffer, SEEK_SET);
                fputs(replacements[i], file);
                fseek(file, 0, SEEK_CUR); // Move file pointer back to original position
                pos = strstr(pos + 1, suspicious_strings[i]);
                log_change(filename);
            }
        }
    }
    fclose(file);
}

void scan_directory(const char *path) {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }
    chdir(path); // Change working directory to the specified path
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Regular file
            replace_string(entry->d_name);
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <directory> <log_directory>\n", argv[0]);
        return 1;
    }
    // Set log directory
    snprintf(log_path, sizeof(log_path), "%s/%s", argv[2], LOG_FILE);
    
    // Daemonize the process
    pid_t pid, sid;
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    umask(0);
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Main loop
    while (1) {
        scan_directory(argv[1]);
        sleep(15);
    }

    return 0;
}
