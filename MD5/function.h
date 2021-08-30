#pragma once

#include<iostream>
#include<fstream>
#include<cstring>
#include<cmath>

using namespace std;

unsigned int A = 0x01234567, B = 0x89abcdef,
C = 0xfedcba98, D = 0x76543210;

int s[4][16] =
{
	{7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22},
	{5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20},
	{4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23},
	{6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21}
};

struct newint
{
	int a[4];
	newint();
	void copy(newint b);
	void print();
	unsigned int value();//以小端方式读入和设置值
	void set(unsigned int);
	newint(char*);
};
newint add_(newint a, newint b);
newint add_big(newint a, newint b);
newint and_(newint a, newint b);
newint or_(newint a, newint b);
newint xor_(newint a, newint b);
newint not_(newint a);
newint leftmove(newint a, int s);//循环左移
unsigned int leftmove(unsigned int a, int s);
int compare(newint a[4], newint b[4]);//比较有多少bit不同
struct text
{
	int round;
	newint m[1500][16];//明文
	newint fabcd[17][4];
	newint gabcd[17][4];
	newint habcd[17][4];
	newint iabcd[17][4];

	newint cipher[4];
	void cipherprint();
	void tostring(char *);
	//void copy(text);
};
void in(char* a, text& m);//输入明文
void in(char* a, text& m,int len);//输入明文
void sestlength(text& m);//扩充明文
void set(char m, int& a, int i);//用于char和int的转换，
void small(unsigned int a, newint& b);//小段存储
void big(unsigned int a, newint& b);//大端存储
//void smallbit(char* temp,int a);
void smallbit(int len, newint& a);
void getbit(unsigned int a, int b);


void enF(int index, text& txt);//加密F
void enG(int index, text& txt);
void enH(int index, text& txt);
void enI(int index, text& txt);
void encrypt(text& txt);
int readfile(char* file, char* filename);



static const struct {
	const char* msg;
	unsigned char hash[16];
}tests[] = {
	{ "",
	  { 0xd4, 0x1d, 0x8c, 0xd9, 0x8f, 0x00, 0xb2, 0x04,
		0xe9, 0x80, 0x09, 0x98, 0xec, 0xf8, 0x42, 0x7e } },
	{ "a",
	  {0x0c, 0xc1, 0x75, 0xb9, 0xc0, 0xf1, 0xb6, 0xa8,
	   0x31, 0xc3, 0x99, 0xe2, 0x69, 0x77, 0x26, 0x61 } },
	{ "abc",
	  { 0x90, 0x01, 0x50, 0x98, 0x3c, 0xd2, 0x4f, 0xb0,
		0xd6, 0x96, 0x3f, 0x7d, 0x28, 0xe1, 0x7f, 0x72 } },
	{ "message digest",
	  { 0xf9, 0x6b, 0x69, 0x7d, 0x7c, 0xb7, 0x93, 0x8d,
		0x52, 0x5a, 0x2f, 0x31, 0xaa, 0xf1, 0x61, 0xd0 } },
	{ "abcdefghijklmnopqrstuvwxyz",
	  { 0xc3, 0xfc, 0xd3, 0xd7, 0x61, 0x92, 0xe4, 0x00,
		0x7d, 0xfb, 0x49, 0x6c, 0xca, 0x67, 0xe1, 0x3b } },
	{ "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
	  { 0xd1, 0x74, 0xab, 0x98, 0xd2, 0x77, 0xd9, 0xf5,
		0xa5, 0x61, 0x1c, 0x2c, 0x9f, 0x41, 0x9d, 0x9f } },
	{ "12345678901234567890123456789012345678901234567890123456789012345678901234567890",
	  { 0x57, 0xed, 0xf4, 0xa2, 0x2b, 0xe3, 0xc9, 0x55,
		0xac, 0x49, 0xda, 0x2e, 0x21, 0x07, 0xb6, 0x7a } },

}
;

string msg[] = {
	"1 2345678901234567890123456789012345678901234567890123456789012345678901234567890",
	"1234567890123456789012345 6789012345678901234567890123456789012345678901234567890",
	"1234567890123456789012345678901234567890123 4567890123456789012345678901234567890",
	"12345678901234567890123456789012345678901234567890,123456789012345678901234567890",
	"123456789012345 67890123456789012345678901234567890123456789012345678901234567890",
	"123456789012345678901234567890123456789012345678901234567890.12345678901234567890",
	"12345678.901234567890123456789012345678901234567890123456789012345678901234567890",
	"1234567890123456789012345?6789012345678901234567890123456789012345678901234567890",

};