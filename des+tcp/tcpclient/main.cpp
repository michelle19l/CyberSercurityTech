#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include <pthread.h>
#include<iostream>
using namespace std;

#define BUF_SIZE 1024
void* threadrecv(void*ptr);
void* threadsend(void*ptr);

int main()
{
	int sockCli=socket(AF_INET, SOCK_STREAM, 0);//地址类型（ipv4），服务类型（流式套接字）
	if(sockCli<0)
	{
		perror("服务端socket初始化失败");
		exit(-1);
	}

	sockaddr_in addrCli;

	addrCli.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrCli.sin_family = AF_INET;
	addrCli.sin_port = htons(21);

	int sockCli;
	
	socklen_t len=sizeof(sockaddr);
	int sockCon=connect(sockCli,(sockaddr*)&addrCli, sizeof(sockaddr_in));
	pthread_t threadId1,threadId2;
	if(1)//链接成功
	{
		int recvthr=pthread_create(&threadId1,NULL,threadrecv,(void*)sockCon);
		int sendthr=pthread_create(&threadId2,NULL,threadsend,(void*)sockCon);
	}
}

void * threadrecv(void*ptr)
{
	int socktemp=*(int*)ptr;
	while(1)
	{char recvbuf[BUF_SIZE]={};
	recv(socktemp,recvbuf,BUF_SIZE,MSG_WAITALL);
	if(recvbuf[0])
	{
		cout<<recvbuf<<endl;
	}
	else 
	{
		break;
	}}
}
void* threadsend(void* ptr)
{
	int socktemp=*(int*)ptr;
	while(1)
	{char sendbuf[BUF_SIZE]={};
	cin.getline(sendbuf,BUF_SIZE,'\n');
	send(socktemp,sendbuf,BUF_SIZE,0);}
}