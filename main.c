/*
* FiLCh
* GNU GPL
*/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>

#define PORT "8080"
#define BACKLOG 5
char* str;

void filesInDir();
int initSock();
void sendToClient(int sock);
int main(void) {
	sendToClient(initSock());// Вызываем функцию sendToClient(), в которую передаем int sock,
	// возвращаемый функцией initSock(), и  char *str, возвращаемый функцией filesInDir
	return 0;
}
void filesInDir()
{
	str = (char*) malloc (256);// Максимальная длина имени одного файла, сумма символовв именах всех файлов,
	// не должны превышать 256 символов для крректной работы програмы
	memset(str, 0, sizeof (str));// Заполняем строку пустыми символами, иначе в выводе присутствуют непонятные знаки
	DIR *dir;
	struct dirent *entry;
	char *nameDir = "./";// Директория, файлы которой нужно вывести
	dir = opendir(nameDir);
	while ((entry = readdir(dir)) != NULL)
	{
		sprintf(str, "%s %s\n", str, (char*)(entry->d_name));// Заполняем строку именами файлов
	}
	closedir(dir);
}
int initSock()
{
	int sock;
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof (hints));
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo(NULL , PORT, &hints, &res);
	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)// Создаём файловый дескриптор
	{
		perror("socket() error!");
		exit(1);// При ошибке socket(), процесс завершается с кодом 1
	}
	if ((bind(sock, res->ai_addr, res->ai_addrlen)) < 0)// Присваиваем файловому дескриптору адрес
	{
		perror("bind() error!");
		exit(2);// При ошибке bind(), процесс завершается с кодом 2
	}
	return sock;
}
void sendToClient(int sock)
{
	struct sockaddr_storage their_addr;
	socklen_t addr_size = sizeof (their_addr);
	if ((listen(sock, BACKLOG)) < 0)//Ждём подключение пользователя
	{
		perror("listen error!");
		exit(3);// При ошибке listen(), процесс завершается с кодом 3
	}
	if ((sock = accept(sock, (struct sockaddr *) &their_addr, &addr_size)) < 0)
	{
		perror("accept() error!");
		exit(4);// При ошибке accept(), процесс завершается с кодом 4
	}
	filesInDir();
	send(sock, str, strlen(str), 0);// Посылаем пользователю список имён файлов из текущей директории.
	free(str);
}