#pragma once
#ifndef RSA
#define RSA


#include<ctime>
#include<cmath>
#include<iostream>
#include<cstring>

using namespace std;

#define INT_MAX 4294967296

//����ͬ���㷨
#define LINEAR_M  INT_MAX - 1
#define LINEAR_A  16807


int change(char a);




class big//ʮ������
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
	int getbit(int i);//��2^iλ
};
big add(big a, big b);
big sub(big a, big b);
big sub(big a, big b, int d);//ƫ��
int compare(big a, big b, int d);
int compare(big a, big b);
big mul(big a, big b);
big mod(big a, big b);
big pow(big a, big b, big n);//a^b mod n
big getinv(big a, big b);
big div(big a, big b, big& c);//�����̣�r������


void getsmallprime();
class prime {
public:
	big number;
	prime();
	prime(string a);
	prime(string a, int x);
	void getarbitrary();//����512bit�����������
	void check2000();//����Ƿ��ܱ�2000���ڵ���������,���������2��ֱ������
	void millerrabin();//����Ƿ�������
};


class RSA_//����
{
public:
	big n, phi;
	big q;
	big p;
	big d, e;
	//big c, m;//���ĺ�����

	RSA_(big p, big q, big e);

};






class RSAen_//����
{
public:
	big n, e;
	big c, m;//���ĺ�����

	RSAen_(RSA_ a, big m);


};

class RSAde_//����
{
public:
	big n, d;
	big c, m;//���ĺ�����
	RSAde_(RSA_ a, big c);
};



#endif // !1