#pragma once
# define _CRT_SECURE_NO_WARNINGS
# define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include<stdlib.h>
#include<stdio.h>
#include <WinSock2.h>//windows socket ���ͷ�ļ�
#include "deshead.h"
using namespace std;

#define BUF_SIZE 2048
#define PORT 6666
#define CLIENTNUM 1




//���ĸ�ʽ
class msg_form
{
public:

	char from_name;//���ͷ����
	char to_name;//���շ�
	char msg[BUF_SIZE];//��Ϣ����
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



//��������
char* msg_to_string(msg_form a);//���ַ����洢����
msg_form string_to_msg(char a[]);//���ַ����ָ�����

//void funcEn(des_test_case m)//���ܹ���
char* msg_en(char*, u_char*);
string initial_key();
char* msg_de(char*, u_char*);








