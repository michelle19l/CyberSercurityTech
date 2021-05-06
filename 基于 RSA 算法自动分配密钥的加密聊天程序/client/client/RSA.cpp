#include "RSA.h"

unsigned int smallprime[5000];
//
//void rsafunc()
//{
//
//
//	big e;
//	e.set(0x10001);
//	prime p;
//	cout << "p= "; p.number.print();
//	prime q;
//	cout << "q= "; q.number.print();
//
//	cout << "3." << endl << "n长度为1024bit的RSA加密和解密：" << endl;
//	RSA_ a(p.number, q.number, e);
//	cout << "n= "; a.n.print();
//	cout << "phi= "; a.phi.print();
//	cout << "e= "; a.e.print();
//	cout << "d= "; a.d.print();
//
//}


//int main()
//{
//
//	cout << "1." << endl << "p=3,q=11,m=2,取e=3:" << endl;
//
//	big p1, q1, e1, m1;
//	p1.set(3); q1.set(5); e1.set(3); m1.set(2);
//	RSA_ a1(p1, q1, e1);
//	cout << "加密结果：";
//	RSAen_ ae1(a1, m1);
//	ae1.c.print();
//
//	cout << "解密结果: ";
//	RSAde_ ad1(a1, ae1.c);
//	ad1.m.print();
//
//	cout << "-------------------------------" << endl << "-------------------------------" << endl;
//
//	cout << "2." << endl << "素数生成：" << endl;
//
//
//	big e;
//	e.set(0x10001);
//
//
//	prime p;
//	cout << "p= "; p.number.print();
//	prime q;
//	cout << "q= "; q.number.print();
//
//	cout << "-------------------------------" << endl << "-------------------------------" << endl;
//	cout << "3." << endl << "n长度为1024bit的RSA加密和解密：" << endl;
//	RSA_ a(p.number, q.number, e);
//	cout << "n= "; a.n.print();
//	cout << "phi= "; a.phi.print();
//	cout << "e= "; a.e.print();
//	cout << "d= "; a.d.print();
//	/*string a_ = "E83531E0C4579D193AEA24FB1AAC40AD3DFC0A8832E2981646024E5733628215073022CB3D2B53E66C9FDB6AEB4BE9BC4A64158C06B0969BA4CE6F2AE11414FB";
//	string b = "B3682FCA34677E225F2BCD52DCBDE0A37A03191F7BA160C0044F819B236E594D23EAC47BFDB22544AC63D2334D1E4C4B106CD49949FF832D0ED74D5F3E4C4B83";
//
//	prime p(a_), q(b);*/
//	//a.d.print();
//	big m;
//	m.set("Hi", 0);
//
//
//	RSAen_ temp(a, m);
//	cout << "m= "; temp.m.print();
//	cout << "加密结果：";
//	temp.c.print();
//
//	RSAde_ temp_(a, temp.c);
//	cout << "解密结果： ";
//	temp_.m.print();
//}


void big::numtostring(char* buf)
{
	for (int i = 0; i < 512; i++)
		buf[i] = this->num[i];
}


prime::prime(string a)
{
	number.set(a);
}
prime::prime(string a, int x)
{
	number.set(a, x);
}

void big::copy(big a)
{
	for (int i = 0; i < 512; i++)
		num[i] = a.num[i];
}

void big::stringtonum(char* buf)
{
	for (int i = 0; i < 512; i++)
		this->num[i] = (int)buf[i];
}


big::big() {
	memset(this, 0, sizeof(big));
}
void big::print()
{
	int i;
	for (i = 511; i > 0; i--)
		if (num[i])
			break;
	cout << "0x";
	for (; i >= 0; i--)
	{
		//cout << hex<<num[i];
		printf("%x", num[i]);
		if (i % 4 == 0)
			cout << "";
	}
	cout << endl << endl;
}

big pow(big a, big b, big n)//a^b mod n
{
	big buffer[512];
	//buffer[0].num[0]=1;
	buffer[0].copy(a);

	int x;
	for (x = 511; x > 0; x--)
		if (b.num[x])
			break;
	for (int i = 1; i <= x; i++)
	{
		big tempbuf[5];
		tempbuf[0].copy(buffer[i - 1]);
		for (int j = 1; j < 5; j++)
		{
			big temp = mul(tempbuf[j - 1], tempbuf[j - 1]);
			//temp.print();
			tempbuf[j] = mod(temp, n);
			//tempbuf[j].print();
		}
		buffer[i].copy(tempbuf[4]);
		//buffer[i].print();
	}

	big product;
	product.num[0] = 1;

	for (int i = 511; i >= 0; i--)
	{
		big temp;
		for (int j = 0; j < b.num[i]; j++)
		{
			temp = mul(product, buffer[i]);
			product = mod(temp, n);
			//product.print();
		}
	}

	return product;
}






big add(big a, big b)
{
	big c;
	for (int i = 0; i < 512; i++)
	{
		c.num[i] = a.num[i] + b.num[i];
	}
	for (int i = 0; i < 511; i++)
	{
		//int t = c.num[i] % 16;
		c.num[i + 1] += c.num[i] / 16;
		c.num[i] %= 16;
	}
	return c;
}
big sub(big a, big b)
{
	big c;
	for (int i = 0; i < 512; i++)
	{
		c.num[i] = a.num[i] - b.num[i];
	}
	for (int i = 0; i < 511; i++)
	{
		//int t = c.num[i] % 16;
		if (c.num[i] < 0)
		{
			c.num[i] += 16;
			c.num[i + 1] -= 1;
		}
	}
	return c;
}
big sub(big a, big b, int d)
{
	big c;
	c.copy(a);
	for (int i = d; i < 512; i++)
	{
		c.num[i] -= b.num[i - d];
	}
	for (int i = 0; i < 511; i++)
	{
		//int t = c.num[i] % 16;
		if (c.num[i] < 0)
		{
			c.num[i] += 16;
			c.num[i + 1] -= 1;
		}
	}
	return c;
}

big mul(big a, big b)
{
	big c;
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			c.num[i + j] += a.num[i] * b.num[j];
		}
	}
	for (int i = 0; i < 510; i++)
	{
		//int t = c.num[i] % 16;
		c.num[i + 1] += c.num[i] / 16;
		c.num[i] %= 16;
	}
	return c;
}

big div(big a, big b, big& c)
{


	big q;
	int t = compare(a, b);
	if (t == -1)return q;
	if (t == 0)
	{
		big c;
		c.set(1);
		return c;//0
	}

	int x, y;
	for (x = 511; x >= 0; x--)
		if (a.num[x])
			break;
	for (y = 511; y >= 0; y--)
		if (b.num[y])
			break;
	int d = x - y;

	c.copy(a);
	//c.print();
	while (d >= 0)
	{
		while (c.num[x] > b.num[y])
		{
			c = sub(c, b, d);
			big v1;
			v1.num[d] = 1;
			q = add(q, v1);
			//c.print();
		}
		if (c.num[x] == b.num[y] && compare(c, b, d) == 1)
		{
			c = sub(c, b, d);
			big v1;
			v1.num[d] = 1;
			q = add(q, v1);
		}
		d--;
		//if (d < 0&&c.num[x]==0)break;
		int t;
		while (compare(c, b) == 1 && c.num[x])
		{
			c = sub(c, b, d);
			big v1;
			v1.num[d] = 1;
			q = add(q, v1);
			//c.print();
		}
		if (c.num[x] == 0)x--;
		t = compare(c, b);
		if (t == -1)break;
		if (t == 0)
		{
			c = sub(c, b);
			big v1;
			v1.num[d] = 1;
			q = add(q, v1);
			return q;
		}
	}
	return q;

}


big mod(big a, big b)//a>b
{
	int t = compare(a, b);
	if (t == -1)return a;
	if (t == 0)
	{
		big c;
		return c;//0
	}

	int x, y;
	for (x = 511; x >= 0; x--)
		if (a.num[x])
			break;
	for (y = 511; y >= 0; y--)
		if (b.num[y])
			break;
	int d = x - y;

	big c;
	c.copy(a);
	//c.print();
	while (d >= 0)
	{
		while (c.num[x] > b.num[y])
		{
			c = sub(c, b, d);
			//c.print();
		}
		if (c.num[x] == b.num[y] && compare(c, b, d) == 1)
			c = sub(c, b, d);
		d--;
		//if (d < 0&&c.num[x]==0)break;
		int t;
		while (compare(c, b) == 1 && c.num[x])
		{
			c = sub(c, b, d);
			//c.print();
		}
		if (c.num[x] == 0)x--;
		t = compare(c, b);
		if (t == -1)return c;
		if (t == 0)
		{
			big s;
			return s;
		}
	}
	return c;
}

int compare(big a, big b)
{
	for (int i = 511; i >= 0; i--)
	{
		if (a.num[i] > b.num[i])return 1;
		if (a.num[i] < b.num[i])return -1;

	}
	return 0;
}
int compare(big a, big b, int d)
{
	for (int i = 511; i >= d; i--)
	{
		if (a.num[i] > b.num[i - d])return 1;
		if (a.num[i] < b.num[i - d])return -1;

	}
	return 0;
}

big getinv(big a, big b) //a mod b
{
	b = mod(b, a);
	big q[100], t[100], r[100];
	r[0].copy(a);
	r[1].copy(b);
	t[0].set(0);
	t[1].set(1);
	int i;
	big v1;
	v1.set(1);
	for (i = 2; i < 500; i++)
	{
		q[i - 1] = div(r[i - 2], r[i - 1], r[i]);
		big temp = mul(q[i - 1], t[i - 1]);
		while (compare(temp, t[i - 2]) == 1)
			t[i - 2] = add(t[i - 2], r[0]);
		t[i] = sub(t[i - 2], temp);
		if (compare(r[i], v1) == 0)
			break;
	}
	return t[i];
}



void getsmallprime()
{
	//找到2000内的素数
	int a[10001];
	memset(a, 0, sizeof(a));
	for (int i = 2; i <= 100; i++)
	{
		for (int j = 2; i * j <= 10000; j++)
		{
			a[i * j] = 1;
		}
	}
	int index = 0;
	for (int i = 2; i <= 10000; i++)
	{
		if (!a[i])
			smallprime[index++] = i;
	}
}





void prime::getarbitrary()
{
	unsigned long long num[16];
	//生成512bit随机数
	srand((unsigned)time(NULL));
	num[0] = rand() % (INT_MAX - 1) + 1;//随机生成x0

	//num[0] = 5;

	for (int i = 1; i < 16; i++)
	{
		num[i] = (num[i - 1] * LINEAR_A) % LINEAR_M;
	}
	num[15] |= (1 << 31) & 0x0000000011111111;
	num[0] |= 1;//最高位和最低为置1
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			number.num[i * 8 + j] = num[i] % 16;
			num[i] /= 16;
		}
	}
	int i;
}

void big::set(unsigned int a)
{
	memset(this, 0, sizeof(big));
	int i = 0;
	while (a)
	{
		num[i] = a % 16;
		a /= 16;
		i++;
	}
}
void big::set(unsigned long long a)
{
	memset(this, 0, sizeof(big));
	int i = 0;
	while (a)
	{
		num[i] = a % 16;
		a /= 16;
		i++;
	}
}
void big::set(int a)
{
	memset(this, 0, sizeof(big));
	if (a < 0)return;
	int i = 0;
	while (a)
	{
		num[i] = a % 16;
		a /= 16;
		i++;
	}
}

int big::getbit(int i)
{
	if ((num[i / 4] & (1 << (i % 4))) == 0)
		return 0;
	return 1;
}




prime::prime()
{
	memset(this, 0, sizeof(prime));
	getarbitrary();//生成512bit随机数，奇数
	check2000();//检查是否能被2000以内的素数整除,若不能则加2，直至可以

	millerrabin();//检查是否是素数
}



void prime::check2000()//检查是否能被2000以内的素数整除,若不能则加2，直至可以
{
	if (smallprime[0] == 0)
		getsmallprime();
	big v0;//0
	big v2;
	v2.set(2);
	while (1)
	{
		int flag = 1;
		for (int i = 0; i < 303; i++)
		{
			big temp;
			temp.set(smallprime[i]);
			big s = mod(number, temp);
			if (compare(s, v0) == 0)
			{
				flag = 0;
				break;
			}
		}
		if (flag)
		{
			break;
		}
		else
		{
			number = add(number, v2);
		}
	}
}

void prime::millerrabin()//检查是否是素数,512位！！！
{
	int a[5] = { 2,3,13,17,19 };
	int flag = 0;
	big n1;
	big v1, v2;
	v1.set(1);
	v2.set(2);
	big v0;
	v0.set(0);

	int high;



	//for (int j = 10; j < 1000; j++)
	{
		//number.set(smallprime[j]);
		while (!flag)
		{
			flag = 1;
			n1 = sub(number, v1);
			for (high = 127; high >= 0; high--)
			{
				if (number.num[high])
					break;
			}
			big temp, d;

			for (int k = 0; k < 5; k++)
			{
				temp.set(a[k]);
				d.set(1);
				for (int i = high * 4 + 3; i >= 0; i--)
				{
					//cout << i << endl;
					big x;
					x.copy(d);
					d = mul(d, d);
					d = mod(d, number);
					if (compare(d, v1) == 0)
					{
						if (compare(x, v1) && compare(x, n1))
						{
							flag = 0;
							break;
						}
					}
					if (n1.getbit(i))
					{

						d = mul(d, temp);
						d = mod(d, number);
					}

				}
				if (compare(d, v1))
				{
					flag = 0;
					//number.print();
				}
				if (!flag)break;
			}

			if (!flag)
			{
				number = add(number, v2);
				check2000();
			}
		}
	}
	//cout << "素数: "; number.print();
}




RSA_::RSA_(big p, big q, big e)
{
	big v1;
	v1.set(1);

	this->p = p;
	this->q = q;
	this->e = e;

	n = mul(p, q);
	big p1, q1;
	p1 = sub(p, v1);
	q1 = sub(q, v1);

	phi = mul(p1, q1);
	d = getinv(phi, e);
}


RSAen_::RSAen_(RSA_ a, big m)
{
	this->n = a.n;
	this->e = a.e;
	this->m = m;

	c = pow(m, e, n);
}

RSAde_::RSAde_(RSA_ a, big c)
{
	this->n = a.n;
	this->d = a.d;
	this->c = c;

	m = pow(c, d, n);
}


int change(char a)
{
	if (a >= '0' && a <= '9')
		return (int)(a - 48);
	switch (a)
	{
	case 'a':
	case 'A':
		return 10;
	case 'b':
	case 'B':
		return 11;
	case 'c':
	case 'C':
		return 12;
	case 'd':
	case  'D':
		return 13;
	case 'e':
	case  'E':
		return 14;
	case 'f':
	case 'F':
		return 15;
	default:
		break;
	}
}


void big::set(string a)//16进制输入
{
	//没有0x
	int len = a.length();
	for (int i = len - 1; i >= 0; i--)
		num[len - i - 1] = change(a[i]);
}

void big::set(string a, int x)//字符串输入
{
	//没有0x
	int len = a.length();
	for (int i = len - 1; i >= 0; i--)
	{
		num[2 * len - 2 * i - 2] = (int)a[i] % 16;
		num[2 * len - 2 * i - 1] = (int)a[i] / 16;
	}
}


