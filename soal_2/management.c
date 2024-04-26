#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <ctype.h>
#include <time.h>

void init_dae(){
	pid_t sid,pid=fork();
	
	if(pid<0){
		exit(EXIT_FAILURE);
	}
	if(pid>0){
		exit(EXIT_SUCCESS);
	}
	umask(0);
	sid=setsid();
	if(sid<0){
		exit(EXIT_FAILURE);
	}
	if((chdir("/"))<0){
		exit(EXIT_FAILURE);
	}
	
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}
void dump(){
	FILE *output=fopen("/home/user/modul_2/soal_2:/debug.log","a");
		
	if(output==NULL){
		perror("unable to open");
		exit(EXIT_FAILURE);
	}
	dup2(fileno(output),1);
	dup2(fileno(output),2);
	fclose(output);
}
void unduh(){
	pid_t pid=fork();
	dump();
	if(pid==0){
		char *download[]={"wget","https://drive.google.com/uc?id=1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup&export=download","-O","/home/user/modul_2/soal_2:/library.zip",NULL};
		execv("/bin/wget",download);
		exit(EXIT_FAILURE);
	}
}
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
void decryptFilename(char *str){
	while(*str){
		*str=decryptFormula(*str);
		str++;
	}
}
void removeFile(char *str){
	char filepath[200];
	strcpy(filepath,"/home/user/modul_2/soal_2:/library/");
	strcat(filepath,str);
	if(remove(filepath)==0) printf("%s deleted\n",str);
	else perror("Unable to delete file");
}
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
char historyLog(char *nama, char *waktu, char *file, char *kondisi){
	FILE *log_file=fopen("/home/user/modul_2/soal_2:/history.log","a");
	if(log_file==NULL){
		perror("unable to open");
		return 1;
	}
	fprintf(log_file, "[%s][%s] - %s - Succesfully %s\n", nama, waktu,file,kondisi);
	fclose(log_file);
}

int main(int argc,char *argv[]){
	init_dae();
	int status;
	while(1){
	char userName[200];
	char *user=getenv("USER");
	strcpy(userName,user);
	
	time_t t=time(NULL);
	struct tm *tm_info=localtime(&t);
	char time_str[9];
	strftime(time_str,sizeof(time_str),"%H:%M:%S:",tm_info);
	
	char *path="/home/user/modul_2/soal_2:/library/";
	struct dirent *de;
		
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
	else{
		unduh();
		wait(&status);
		if(WIFEXITED(status)){
			unzip();
		}
		wait(&status);
		if(WIFEXITED(status)){
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
				decryptFilename(de->d_name);
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
	}
	remove("/home/user/modul_2/soal_2:/debug.log");
	sleep(30);
	}
}
