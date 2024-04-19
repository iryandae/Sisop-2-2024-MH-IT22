#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define MAX_PATH_LENGTH 1000
#define SUSPICIOUS_STRING_COUNT 3

// Array string yang akan diganti
const char *strings_to_replace[] = {"m4LwAr3", "5pYw4R3", "R4nS0mWaR3"};
// Array string pengganti
const char *replacement_strings[] = {"[MALWARE]", "[SPYWARE]", "[RANSOMWARE]"};

// Fungsi untuk mereplace string dalam sebuah string
char *replace_string(char *str, const char *old_str, const char *new_str) {
    char *result;
    int i, cnt = 0;
    int new_str_len = strlen(new_str);
    int old_str_len = strlen(old_str);

    // Hitung berapa kali old_str muncul dalam str
    for (i = 0; str[i] != '\0'; i++) {
        if (strstr(&str[i], old_str) == &str[i]) {
            cnt++;
            i += old_str_len - 1;
        }
    }

    // Buat string baru dengan ukuran yang sesuai
    result = (char *)malloc(i + cnt * (new_str_len - old_str_len) + 1);

    i = 0;
    while (*str) {
        // Jika old_str ditemukan, replace dengan new_str
        if (strstr(str, old_str) == str) {
            strcpy(&result[i], new_str);
            i += new_str_len;
            str += old_str_len;
        } else {
            result[i++] = *str++;
        }
    }
    result[i] = '\0';
    return result;
}

// Fungsi untuk mereplace string dalam sebuah file
void replace_strings_in_file(const char *file_path) {
    FILE *file = fopen(file_path, "r+");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char buffer[MAX_PATH_LENGTH];
    FILE *temp_file = tmpfile();
    if (temp_file == NULL) {
        perror("Error creating temporary file");
        fclose(file);
        return;
    }

    // Baca file per baris
    while (fgets(buffer, sizeof(buffer), file)) {
        for (int i = 0; i < SUSPICIOUS_STRING_COUNT; i++) {
            char *pos = strstr(buffer, strings_to_replace[i]);
            while (pos != NULL) {
                fputs(buffer, temp_file);
                fputs(replacement_strings[i], temp_file);
                pos = strstr(pos + strlen(strings_to_replace[i]), strings_to_replace[i]);
            }
        }
    }

    fseek(file, 0, SEEK_SET);
    fseek(temp_file, 0, SEEK_SET);

    // Salin isi file temporary ke file asli
    char ch;
    while ((ch = fgetc(temp_file)) != EOF) {
        fputc(ch, file);
    }

    fclose(file);
    fclose(temp_file);

    // Catat penggantian string dalam virus.log
    FILE *log_file = fopen("virus.log", "a");
    if (log_file != NULL) {
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%d-%m-%Y %H:%M:%S", tm_info);
        fprintf(log_file, "[%s] Suspicious strings in %s successfully replaced!\n", timestamp, file_path);
        fclose(log_file);
    } else {
        perror("Error writing to virus.log");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <folder_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Cek apakah folder_path valid
    const char *folder_path = argv[1];
    struct stat path_stat;
    if (stat(folder_path, &path_stat) != 0 || !S_ISDIR(path_stat.st_mode)) {
        fprintf(stderr, "Error: Folder path is not valid.\n");
        exit(EXIT_FAILURE);
    }

    // Buka file virus.log untuk menulis
    FILE *log_file = fopen("virus.log", "a");
    if (log_file == NULL) {
        perror("Error opening virus.log");
        exit(EXIT_FAILURE);
    }

    // Tulis pesan pembukaan ke virus.log
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%d-%m-%Y %H:%M:%S", tm_info);
    fprintf(log_file, "[%s] Program started.\n", timestamp);

    // Jalankan program untuk setiap file dalam folder
    DIR *dir;
    struct dirent *entry;
    dir = opendir(folder_path);
    if (dir == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    // Proses setiap file dalam folder
    while ((entry = readdir(dir)) != NULL) {
        char file_path[MAX_PATH_LENGTH];
        snprintf(file_path, sizeof(file_path), "%s/%s", folder_path, entry->d_name);

        // Hanya proses file yang memiliki ekstensi .txt
        if (strstr(entry->d_name, ".txt") != NULL) {
            replace_strings_in_file(file_path);
        }
    }

    // Tulis pesan penutup ke virus.log
    now = time(NULL);
    tm_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%d-%m-%Y %H:%M:%S", tm_info);
    fprintf(log_file, "[%s] Program finished.\n", timestamp);

    // Tutup file virus.log
    fclose(log_file);

    closedir(dir);

    return 0;
}
