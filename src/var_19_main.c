#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <string.h>
#include "lab_4.h"

const int SIZE_OF_CHARS = 200;

int main(){
	srand(time(0));

	char path1[200] = "",path2[200] = "";

	char t[200] = "Введи в следующий двух строках - пути к файлам, куда я буду отправлять выходные данные ^_^\n";
	write(0,t,sizeof(char) * 200);

	read(0,path1,sizeof(char)*200);
	int path1Len = strlen(path1);
	path1[path1Len-1] = '\0';
	
	if(!strcmp(path1, "pip1") || !strcmp(path1, "ex1") || 
			!strcmp(path1, "pip2") || !strcmp(path1, "ex2")){
		char k[200] = "Такое название файлов недопустимо, попробуйте снова!\n";
		write(0,k,sizeof(char) * 200);
		return 1;
	}

	read(0,path2,sizeof(char)*200);
	int path2Len = strlen(path2);
	path2[path2Len-1] = '\0';

	if(!strcmp(path2, "pip2") || !strcmp(path2, "ex2") ||
			!strcmp(path2, "pip1") || !strcmp(path2, "ex1")){
		char k[200] = "Такое название файлов недопустимо, попробуйте снова!\n";
		write(0,k,sizeof(char) * 200);
		return 1;
	}

	int ft1 = open(path1,O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
	int ft2 = open(path2,O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);

	if(ft1 == -1 || ft2 == -1){
		char k[200] = "С файлами произошла неизвестная ошибка!\n";
		write(0,k,sizeof(char) * 200);
		return 1;
	}

	// int fd[2];
	// int fd2[2];
	int pip1int = open("pip1", O_CREAT | O_RDWR, S_IRWXU);
	char* mm1 = (char*)mmap(NULL, SIZE_OF_CHARS, PROT_READ | PROT_WRITE, MAP_SHARED, pip1int, 0);
	ftruncate(pip1int, SIZE_OF_CHARS);

	int ex1int = open("ex1", O_CREAT | O_RDWR, S_IRWXU);
	char* mme1 = (char*)mmap(NULL, 1, PROT_READ | PROT_WRITE, MAP_SHARED, ex1int, 0);
	ftruncate(ex1int, 1);

	int pip2int = open("pip2", O_CREAT | O_RDWR, S_IRWXU);
	char* mm2 = (char*)mmap(NULL, SIZE_OF_CHARS, PROT_READ | PROT_WRITE, MAP_SHARED, pip2int, 0);
	ftruncate(pip2int, SIZE_OF_CHARS);
	
	int ex2int = open("ex2", O_CREAT | O_RDWR, S_IRWXU);
	char* mme2 = (char*)mmap(NULL, sizeof(char*), PROT_READ | PROT_WRITE, MAP_SHARED, ex2int, 0);
	ftruncate(ex2int, 1);

	if(mm1 == MAP_FAILED || mm2 == MAP_FAILED || 
			mme1 == MAP_FAILED || mme2 == MAP_FAILED)
	{
		char k[200] = "При создании mmap произошла ошибка!\n";
		write(0,k,sizeof(char) * 200);
		return 1;
	}

	int id = fork();
	int id2 = fork();

	char in[SIZE_OF_CHARS];
	if(id != 0 && id2 != 0){
		char a[50] = "Введите свои строки\n";
		write(0,a,50*sizeof(char));
		while((scanf("%s",in)) == 1){
			if(rand() % 100 < 80){
				char a[50] = "Первый файл\n";			
				write(0,a,50*sizeof(char));
				
				mme1[0] = '+';
				strcpy(mm1, in);
			}
			else{
				char a[50] = "Второй файл\n";			
				write(0,a,50*sizeof(char));

				mme2[0] = '+';
				strcpy(mm2, in);
			}
		}

		mme2[0] = '-';
		mme1[0] = '-';
	}
	else if(id2 == 0 && id != 0){
		while(1){
			while(mme2[0] != '+' && mme2[0] != '-') {}

			if(mme2[0] == '-')
				break;
			mme2[0] = '_';
			write(ft2, Task(mm2), SIZE_OF_CHARS);
			printf("[2] Успешно записано! [2]\n");
		}
	}
	else if(id == 0 && id2 != 0){
		while(1){
			while(mme1[0] != '+' && mme1[0] != '-') {}
			if(mme1[0] == '-')
				break;
			//printf("[1] I catch PLUS\n");
			mme1[0] = '_';
			write(ft1, Task(mm1), SIZE_OF_CHARS);
			printf("[1] Успешно записано! [1]\n");
		}
	}

	close(ft1);
	close(ft2);

	remove("ex1");
	remove("ex2");
	remove("pip1");
	remove("pip2");
	return 0;
}
