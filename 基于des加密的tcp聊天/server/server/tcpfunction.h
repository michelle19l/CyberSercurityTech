#pragma once
# define _CRT_SECURE_NO_WARNINGS
# define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<stdlib.h>
#include<stdio.h>
#include <WinSock2.h>//windows socket ���ͷ�ļ�
#include<string.h>
#include<cstring>
#include<string>
#include "deshead.h"
#define BUF_SIZE 2048
#define PORT 6666
#define CLIENTNUM 2

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

//void getkey(u_char* key);//server������Կ
//
////��������
char* msg_to_string(msg_form a);//���ַ����洢����
msg_form string_to_msg(char a[]);//���ַ����ָ�����


char* msg_en(char* ,u_char*);
char* msg_de(char*,u_char*);



char* msg_en(char* a, u_char* key) {
	u_char key_final[16][6] = {};
	//������Կ
	getkeys(key, key_final);
	u_char cipher[8] = {};
	round((u_char*)a, key_final, cipher);
	return (char*)cipher;
}
char* msg_de(char* a, u_char* key) {
	u_char key_final_[16][6] = {};
	u_char key_final[16][6] = {};
	getkeys(key, key_final_);
	for (int i = 0; i < 16; i++)
		for (int j = 0; j < 6; j++)
			key_final[15 - i][j] = key_final_[i][j];
	u_char m[8] = {};
	round((u_char*)a, key_final, m);
	return (char*)m;
}

//��������
char* msg_to_string(msg_form a)//�����ĸ�ʽתΪ�ַ���
{
	//char from_name[4];//���ͷ����
	//char to_name[4];//���շ�
	//char msg[BUF_SIZE];//��Ϣ����
	char* c = new char[BUF_SIZE + 6];

	c[0] = a.from_name;
	c[1] = a.to_name;
	c[2] = 0;
	strcat(c, a.msg);

	return c;
}
msg_form string_to_msg(char a[])//���ַ����ָ�����
{
	msg_form msg_;
	msg_.from_name = a[0];
	msg_.to_name = a[1];
	int i;
	for (i = 0; a[i + 2]; i++)
	{
		msg_.msg[i] = a[i + 2];
	}


	return msg_;
}
void getkey(u_char* key)
{
	for (int i = 0; i < 8; i++)
	{
		key[i] = (u_char)(rand() % 128);
	}
}



