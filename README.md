# Sisop-2-2024-MH-IT22
## Anggota Kelompok
- 5027231003  Chelsea Vania Hariyono
- 5027231024  Furqon Aryadana
- 5027231057  Elgracito Iryanda Endia


## Soal 1
Membuat folder penyimpanan file.
```shell
mkdir sink && cd sink
```
Membuat/konfigurasi file virus.c
```shell
nano virus.c
```
Konfigurasi
```c
//belum diganti//
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
```
Buat folder untuk teks yang akan diubah agar lebih efisien dan tidak acak atau salah saat ingin menjalankan perintah nanti, karena perintah yang dijalankan akan merubah file pada suatu folder.
```shell
cd.. && mkdir dink && cd dink
```
Selanjutnya buat file text (.txt) pada text editor, dan pindahkan pada folder "dink".
```shell
pU=-JWQ$5$)D-[??%AVh]$cB6bm4LwAr3jEQC2p3R{HV]=-AUaxj:Qe+h
!aNX,i:!z3W=2;.tHc3&S+}6F)CFf%tfZLP1*w5m1PAzZJUux(
Pd&f8$F5=E?@#[6jd{TJKj]5pYw4R3{KK1?hz384$ge@iba5GAj$gqB41
#C&&a}M9C#f64Eb.?%c)dGbCvJXtU[?SE4h]BY4e1PR4nS0mWaR3{]S/{w?*
```
Tampilkan apakah teks sudah tersimpan
```shell
cat jamettie
```
Pindah direktori untuk menjalankan program virus.c
```shell
cd.. && cd sink
```
Perintah untuk menjalankan program/konfigurasi C
```shell
gcc -o virus virus.c
```
Jalankan program virus.c
```shell
./virus /home/combrero[user]/dink
```
Tampilkan apakah ada perubahan yang tercatat
```shell
cat virus.log
```
Akan muncul kalimat "Suspicious string at /home./combrero[user]/dink[folder temat text file berada]/jamettie[nama text file] successfully replaced!" 
Lalu text pada text file akan berubah menjadi
```shell
pU=-JWQ$5$)D-[??%AVh]$cB6b[MALWARE]jEQC2p3R{HV]=-AUaxj:Qe+h
!aNX,i:!z3W=2;.tHc3&S+}6F)CFf%tfZLP1*w5m1PAzZJUux(
Pd&f8$F5=E?@#[6jd{TJKj][SPYWARE]{KK1?hz384$ge@iba5GAj$gqB41
#C&&a}M9C#f64Eb.?%c)dGbCvJXtU[?SE4h]BY4e1P[RANSOMWARE]{]S/{w?*
```
## Soal 2
fungsi untuk mengunduh file dari link yang disediakan
```c
void unduh(){
	pid_t pid=fork();
	dump();
	if(pid==0){
		char *download[]={"wget","https://drive.google.com/uc?id=1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup&export=download","-O","/home/user/modul_2/soal_2:/library.zip",NULL};
		execv("/bin/wget",download);
		exit(EXIT_FAILURE);
	}
}
```
fungsi untuk unzip file
```c
void unzip(){
	pid_t pid=fork();
	dump();
	if(pid==0){
		char *fileunzip[]={"unzip","/home/user/modul_2/soal_2:/library.zip","-d","/home/user/modul_2/soal_2:/",NULL};
		execv("/bin/unzip",fileunzip);
		exit(EXIT_FAILURE);
	}
	else if(pid<0){
		exit(EXIT_FAILURE);
	}
}
```
kedua fungsi tersebut dijalankan di dalam main dengan wait status untuk memastikan fungsi download sudah selesai sebelum fungsi unzip dijalankan
```c
unduh();
		wait(&status);
		if(WIFEXITED(status)){
			unzip();
		}
```
mendekripsi nama file diperlukan rumus dekripsi dengan kunci yang sudah ditentukan (kunci=19), dapat dilakukan dengan rumus
```c
char decryptFormula(char ch){
	int key=19;
	if(isalpha(ch)){
		char offset=isupper(ch) ? 'A' : 'a';
		return ((ch-offset-key+26)%26)+offset;
	}
	else{
		return ch;
	}
}
```
kemudian rumus dalam fungsi tersebut digunakan dalam fungsi untuk mendekripsi tiap karakter yang ada pada nama file
```c
void decryptFilename(char *str){
	while(*str){
		*str=decryptFormula(*str);
		str++;
	}
}
```
Kemudian, di dalam main digunakan dirent untuk membaca file di dalam direktori
```c
DIR *dir=opendir(path);
		if(dir==NULL) return 0;
		while((de=readdir(dir))!=NULL){
			//fungsi dekripsi
		closedir(dir);
		}
```
Pada program saya, fungsi dekripsi dilakukan setelah file yang tidak terenkripsi sudah diproses berdasarkan kode filenya
```c
DIR *dir=opendir(path);
		if(dir==NULL) return 0;
		while((de=readdir(dir))!=NULL){
			
			char old_name[200];
			char new_name[200];
			strcpy(old_name,path);
			strcat(old_name,de->d_name);
			
			if(strstr(de->d_name,"d3Let3")!=NULL){
				removeFile(de->d_name);
				historyLog(userName,time_str,de->d_name,"deleted");
			}
			else if(strstr(de->d_name,"r3N4mE")!=NULL){
				renameFile(old_name);
				historyLog(userName,time_str,de->d_name,"renamed");
			}
			else if(strstr(de->d_name,"m0V3")==NULL){
				strcpy(new_name,path);
				decryptFilename(de->d_name);        //dekripsi nama file mulai di sini
				strcat(new_name,de->d_name);
				rename(old_name,new_name);
					
				if(strstr(de->d_name,"d3Let3")!=NULL){
					removeFile(de->d_name);
					historyLog(userName,time_str,old_name,"deleted");
				}
				else if(strstr(de->d_name,"r3N4mE")!=NULL){
					renameFile(new_name);
					historyLog(userName,time_str,old_name,"renamed");
				}
			}
		}
		closedir(dir);
		}
```
Dalam kumpulan perintah tersebut, untuk setiap file yang memiliki kode ```d3Let3``` akan dihapus dengan fungsi berikut:
```c
void removeFile(char *str){
	char filepath[200];
	strcpy(filepath,"/home/user/modul_2/soal_2:/library/");
	strcat(filepath,str);
	if(remove(filepath)==0) printf("%s deleted\n",str);
	else perror("Unable to delete file");
}
```
dan untuk setiap file yang memiliki kode ```r3N4mE``` akan direname sesuai dengan ketentuan yang sudah diberikan dengan fungsi berikut:
```c
void renameFile(char *str){
	if(strstr(str,".ts")!=NULL){
					rename(str,"/home/user/modul_2/soal_2:/library/helper.ts");
				}
				else if(strstr(str,".py")!=NULL){
					rename(str,"/home/user/modul_2/soal_2:/library/calculator.py");
				}
				else if(strstr(str,".go")!=NULL){
					rename(str,"/home/user/modul_2/soal_2:/library/server.go");
				}
				else{
					rename(str,"/home/user/modul_2/soal_2:/library/renamed.file");
				}
}
```
File yang tidak diproses dan tidak memiliki kode ```m0V3``` akan dianggap sebagai file yang terdekripsi dan baru akan didekripsi untuk kemudian diproses sesuai dengan kode yang diberikan pada setiap file. Semua proses hingga titik ini merupakan mode default dari program.

mode default (isi direktori library)

![Screenshot 2024-04-26 171052](https://github.com/iryandae/Sisop-2-2024-MH-IT22/assets/121481079/89290b26-543f-4998-bb94-4d3860500268)

Selain itu, dalam program ini diharuskan memiliki tiga mode. Mode kedua yang dimiliki program ini adalah mode backup yang dapat dijalankan dengan perintah ```./management -m backup```

untuk memungkinkan program berjalan berdasarkan perintah yang diinginkan, dapat digunakan perintah berikut:
```c
int mode;
	if(argc>1){
		if(strcmp(argv[1],"-m")==0){
			if(strcmp(argv[2],"backup")==0){
				mode=1;
			}
			else if(strcmp(argv[2],"restore")==0){
				mode=2;
			}
		}
	}
```
mode backup digunakan untuk memindahkan file dengan kode ```m0V3``` ke dalam direktori backup. Jika tidak ditemukan direktori yang dibutuhkan, program akan secara otomatis membuat direktori baru sebagai backup
```c
void direktoriBackup(){
	pid_t pid=fork();
	dump();
	if(pid==0){
		char *download[]={"mkdir","/home/user/modul_2/soal_2:/library/backup",NULL};
		execv("/bin/mkdir",download);
		exit(EXIT_FAILURE);
	}
	else if(pid<0){
		exit(EXIT_FAILURE);
	}
}
```
Jika direktori backup sudah ada/sudah dibuat. Program akan langsung memindahkan file ke dalam direktori backup dengan fungsi berikut:
```c
void moveFile(char *source, char *destination){
	pid_t pid=fork();
	if(pid==0){
		char *moving[]={"mv",source,destination,NULL};
		execv("/bin/mv",moving);
		exit(EXIT_FAILURE);
	}
	else if(pid<0){
		exit(EXIT_FAILURE);
	}
	else if(pid>0){
		wait(NULL);
	}
}
```
fungsi tersebut dijalankan menggunakan dirent di dalam main
```c
if(mode==1){
		DIR *dir=opendir(path);
		if(dir==NULL) return 0;
		direktoriBackup();
		while((de=readdir(dir))!=NULL){
			char old_path[200],new_path[200];
			if(strstr(de->d_name,"m0V3")!=NULL){
				strcpy(old_path,"/home/user/modul_2/soal_2:/library/");
				strcat(old_path,de->d_name);
					
				strcpy(new_path,"/home/user/modul_2/soal_2:/library/backup/");
				strcat(new_path,de->d_name);
				moveFile(old_path,new_path);
				historyLog(userName,time_str,de->d_name,"moved to backup");
			}
		}
		closedir(dir);
	}
```

mode backup (direktori backup & file dipindahkan)

![image](https://github.com/iryandae/Sisop-2-2024-MH-IT22/assets/121481079/7d1bf831-2739-4b66-8caf-26b42f30e93a)

kemudian untuk mode restore—untuk mengembalikan file pada direktori backup kepada direktori sebelumnya—dapat digunakan struktur dan fungsi yang sama, tetapi dengan file path yang berbeda
```c
else if(mode==2){
		char *new_path="/home/user/modul_2/soal_2:/library/backup/";
		DIR *dir=opendir(new_path);
		if(dir==NULL) return 0;
		while((de=readdir(dir))!=NULL){
			char old_path[200],new_path[200];
			if(strstr(de->d_name,"m0V3")!=NULL){
				strcpy(old_path,"/home/user/modul_2/soal_2:/library/");
				strcat(old_path,de->d_name);
					
				strcpy(new_path,"/home/user/modul_2/soal_2:/library/backup/");
				strcat(new_path,de->d_name);
				moveFile(new_path,old_path);
				historyLog(userName,time_str,de->d_name,"restored from backup");
			}
		}
		closedir(dir);
	}
```

mode restore (file backup kembali ke lokasi semula)

![Screenshot 2024-04-26 215406](https://github.com/iryandae/Sisop-2-2024-MH-IT22/assets/121481079/0b76dab1-7daa-4c4b-9662-118645df905a)

Dalam setiap perintah utama dari program tersebut diikuti dengan fungsi yang berperan untuk mencatat aktifitas yang terjadi pada program yang kemudian akan disimpan pada ```history.log```
```c
char historyLog(char *nama, char *waktu, char *file, char *kondisi){
	FILE *log_file=fopen("/home/user/modul_2/soal_2:/history.log","a");
	if(log_file==NULL){
		perror("unable to open");
		return 1;
	}
	fprintf(log_file, "[%s][%s] - %s - Succesfully %s\n", nama, waktu,file,kondisi);
	fclose(log_file);
}
```
Berikut merupakan isi dari file ```history.log```

![Screenshot 2024-04-26 170753](https://github.com/iryandae/Sisop-2-2024-MH-IT22/assets/121481079/37f195e4-5a06-429a-b739-52cb9f9cfe11)

Kendala yang saya alami adalah ketika saya mencoba mengintegrasikan kemampuan untuk mengganti mode ini dengan mengirim sinyal ke daemon, dengan ketentuan yang sudah ditentukan. Program saya tidak berjalan dan juga tidak menggeluarkan output apapun. Di dalam ```debug.log``` yang saya buat. Output error dari program juga tidak menampilkan apapun.

Berdasarkan masalah ini saya berusaha mencari penyebab masalah dan solusinya hingga saya menemukan kemungkinan bahwa yang menyebabkan error tersebut merupakan kegagalan sistem untuk menerima sinyal yang seharusnya sudah diatur untuk masing-masing modenya. Saya juga sudah mencoba solusi yang diberikan di forum internet, tetapi hal tersebut tidak membuahkan hasil pada kode saya.
## Soal 3


## Soal 4
