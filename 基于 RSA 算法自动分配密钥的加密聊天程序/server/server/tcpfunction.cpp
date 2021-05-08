#include "tcpfunction.h"
#include<time.h>



char* msg_en(char* a, u_char key_final[16][6], char* cipher) {
	//u_char key_final[16][6] = {};
	////生成密钥
	//getkeys(key, key_final);
	//cipher=new char[9];
	int len = strlen(a);
	u_char temp[8], tempcipher[8];
	int index = 0;
	memset(cipher, 0, BUF_SIZE);
	for (int i = 0; i < len; i += 8)
	{
		memset(temp, 0, sizeof(temp));
		memset(tempcipher, 0, sizeof(tempcipher));
		for (int j = 0; j < 8; j++)
		{
			temp[j] = (u_char)a[i + j];
		}
		round(temp, key_final, tempcipher);
		for (int j = 0; j < 8; j++)
		{
			cipher[index++] = tempcipher[j];
		}
	}
	cout << "encoded: ";
	for (int i = 0; i < sizeof(cipher); i++)
	{
		cout << hex << (int)cipher[i] << " ";
	}
	cout << endl;
	return cipher;
}
char* msg_de(char* a, u_char key_final[16][6], char* m) {


	int len = strlen(a);
	u_char temp[8], tempm[8];
	int index = 0;
	memset(m, 0, BUF_SIZE);
	for (int i = 0; i < BUF_SIZE; i += 8)
	{
		memset(temp, 0, sizeof(temp));
		memset(tempm, 0, sizeof(tempm));
		for (int j = 0; j < 8; j++)
		{
			temp[j] = a[i + j];
		}
		round(temp, key_final, tempm);
		for (int j = 0; j < 8; j++)
		{
			m[index++] = tempm[j];
		}
	}
	cout << "decoded: ";
	for (int i = 0; i < sizeof(m); i++)
	{
		cout << hex << (int)m[i] << " ";
	}
	cout << endl;

	return m;
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
	string key = "";
	srand((int)time(0));
	for (int i = 0; i < 8; i++)
	{
		key += rand() % 128;
	}
	return key;
}