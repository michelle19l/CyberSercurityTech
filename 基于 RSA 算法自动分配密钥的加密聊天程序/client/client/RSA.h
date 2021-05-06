#pragma once
#ifndef RSA
#define RSA


#include<ctime>
#include<cmath>
#include<iostream>
#include<cstring>

using namespace std;

#define INT_MAX 4294967296

//线性同余算法
#define LINEAR_M  INT_MAX - 1
#define LINEAR_A  16807


int change(char a);




class big//十六进制
{
public:
	int num[512];
	big();
	void copy(big);
	void print();
	void set(unsigned int a);
	void set(int a);
	void set(string a);
	void set(string a, int x);
	void set(unsigned long long a);
	int getbit(int i);//第2^i位
};
big add(big a, big b);
big sub(big a, big b);
big sub(big a, big b, int d);//偏移
int compare(big a, big b, int d);
int compare(big a, big b);
big mul(big a, big b);
big mod(big a, big b);
big pow(big a, big b, big n);//a^b mod n
big getinv(big a, big b);
big div(big a, big b, big& c);//返回商，r是余数


void getsmallprime();
class prime {
public:
	big number;
	prime();
	prime(string a);
	prime(string a, int x);
	void getarbitrary();//生成512bit随机数，奇数
	void check2000();//检查是否能被2000以内的素数整除,若不能则加2，直至可以
	void millerrabin();//检查是否是素数
};


class RSA_//加密
{
public:
	big n, phi;
	big q;
	big p;
	big d, e;
	//big c, m;//密文和明文

	RSA_(big p, big q, big e);

};






class RSAen_//加密
{
public:
	big n, e;
	big c, m;//密文和明文

	RSAen_(RSA_ a, big m);


};

class RSAde_//加密
{
public:
	big n, d;
	big c, m;//密文和明文
	RSAde_(RSA_ a, big c);
};



#endif // !1