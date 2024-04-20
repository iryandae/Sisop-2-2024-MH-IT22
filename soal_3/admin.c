#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

pid_t pid;
FILE *log_file;

void write_log(FILE *file, char *process_name, char *status) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(file, "[%02d:%02d:%04d]-[%02d:%02d:%02d]-%d-%s_%s\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, pid, process_name, status);
}

void monitor(char *user) {
    printf("monitor proses berjalan\n");
    char filename[50];
    sprintf(filename, "%s.log", user);
    log_file = fopen(filename, "a");
    if (log_file == NULL) {
        printf("Gagal membuka file log\n");
        exit(1);
    }
    pid = fork();
    if (pid < 0) {
        printf("Gagal membuat proses anak\n");
        exit(1);
    }
    if (pid == 0) {
        while (1) {
            pid_t child_pid = fork();
            if (child_pid == 0) {
                char *args[] = {"/bin/sh", "-c", NULL, NULL};
                char command[50];
                sprintf(command, "ps -u %s > /dev/null", user);
                args[2] = command;
                execvp(args[0], args);
            } else {
                wait(NULL);
                write_log(log_file, "monitor proses", "berjalan");
                sleep(1);
            }
        }
    }
}

void stop() {
    printf("proses monitor dihentikan\n");
    kill(pid, SIGKILL);
    write_log(log_file, "monitor proses", "dihentikan");
    fclose(log_file);
}

void cancel() {
    printf("proses digagalkan\n");
    while(1) {
        kill(pid, SIGSTOP);
        write_log(log_file, "monitor proses", "digagalkan");
        sleep(1);
    }
}

void resume() {
    printf("proses dijalankan kembali\n");
    kill(pid, SIGCONT);
    write_log(log_file, "monitor proses", "dijalankan kembali");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Argumen tidak valid\n");
        return 1;
    }
    char *user = argv[2];
    if (strcmp(argv[1], "-m") == 0) {
        monitor(user);
    } else if (strcmp(argv[1], "-s") == 0) {
        stop();
    } else if (strcmp(argv[1], "-c") == 0) {
        cancel();
    } else if (strcmp(argv[1], "-a") == 0) {
        resume();
    } else {
        printf("Argumen tidak valid\n");
    }
    return 0;
}

