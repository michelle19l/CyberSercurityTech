#include "tcpfunction.h"




char* msg_en(char* a, u_char* key) {
	u_char key_final[16][6] = {};
	//生成密钥
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

//函数定义
char* msg_to_string(msg_form a)//将报文格式转为字符串
{
	//char from_name[4];//发送方标号
	//char to_name[4];//接收方
	//char msg[BUF_SIZE];//消息内容
	char* c = new char[BUF_SIZE + 6];

	c[0] = a.from_name;
	c[1] = a.to_name;
	c[2] = 0;
	strcat(c, a.msg);

	return c;
}
msg_form string_to_msg(char a[])//将字符串恢复成类
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

string initial_key()//server生成64位初始密钥
{
	string key="";
	for (int i = 0; i < 8; i++)
	{
		key += rand() % 128;
	}
	return key;
}