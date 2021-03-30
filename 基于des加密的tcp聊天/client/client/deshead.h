#pragma once
#ifndef DES
#define DES

#include <iostream>
#include<fstream>
#include<iomanip>
#include<bitset>
using namespace std;

typedef unsigned char u_char;

class tuple_
{
public:
	int u_char_;//第几个字节，0开始
	int bit_;//第几位，从左至右，0开始
	tuple_() {};
	tuple_(int a, int b) {
		u_char_ = a;
		bit_ = b;
	}
};
//将字节索引该为字索引
int u_char2bit(tuple_ a);
//将字索引改为字节索引
tuple_ bit2u_char(int a);

//取第几位
int getbit(u_char a, int x);//从左至右第0-7位
//在第几位写入
void write(u_char& a, int x, int num);
//做对应转换
void convert(u_char a[], u_char b[], int i, int conv[]);

//初始置换
void convIP(u_char a[], u_char b[]);//将a进行IP置换，结果存入b中
//初始置换逆置换IP^(-1)
void convIP_1(u_char a[], u_char b[]);//逆初始置换，将a进行IP置换，结果存入b中
//密钥生成
void getkeys(u_char key[8], u_char key_final[16][6]);
//扩展置换
void convertE(u_char a[4], u_char b[6]);
//48bit异或，结果存到c中
void xor_(u_char a[6], u_char b[6], u_char c[6]);

//S盒
void convertS(u_char a[6], u_char b[4]);
void convertP(u_char a[4], u_char b[4]);

//轮加密
void round(u_char m[8], u_char key_final[16][6], u_char afterIP_1[8]);

//void funcEn(des_test_case m);//加密过程
//void funcDe(des_test_case c);
#endif