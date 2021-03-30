#pragma once
# define _CRT_SECURE_NO_WARNINGS
# define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include<stdlib.h>
#include<stdio.h>
#include <WinSock2.h>//windows socket 编程头文件
#include "deshead.h"
using namespace std;

#define BUF_SIZE 2048
#define PORT 6666
#define CLIENTNUM 1




//报文格式
class msg_form
{
public:

	char from_name;//发送方标号
	char to_name;//接收方
	char msg[BUF_SIZE];//消息内容
	msg_form() { memset(msg, 0, BUF_SIZE); }
	msg_form(const char a, const char b, const char c[BUF_SIZE])
	{
		from_name = a;
		to_name = b;
		strcpy(msg, c);

	}
	msg_form(char a, char b, char c[BUF_SIZE])
	{

		from_name = a;
		to_name = b;
		strcpy(msg, c);

	}
};



//函数声明
char* msg_to_string(msg_form a);//用字符串存储报文
msg_form string_to_msg(char a[]);//将字符串恢复成类

//void funcEn(des_test_case m)//加密过程
char* msg_en(char*, u_char*);
string initial_key();
char* msg_de(char*, u_char*);








