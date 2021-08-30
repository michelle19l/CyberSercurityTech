#define _CRT_SECURE_NO_WARNINGS
#include "function.h"

int main()
{

	//for (int j = 0; j < 7; j++)
	//{
	//	char s[2048];
	//	memset(s, 0, 2048);
	//	for (int i = 0; tests[j].msg[i]; i++)
	//		s[i] = tests[j].msg[i];
	//	text txt;
	//	in(s, txt);
	//	encrypt(txt);
	//	cout << "明文：" << tests[j].msg << endl;
	//	cout << "密文:";
	//	txt.cipherprint();


	//}
	//cout << "-----------------------------" << endl;
	while (1)
	{
		char c;
		cout << "请输入操作选项\n1.计算字符串md5；\n2.计算文件md5；\n3.验证文件完整性；\n输入其它字符退出\n";
		cin >> c;
		if (c == '1')
		{
			cout << "字符串md5计算" << endl;
			char s[2048];
			memset(s, 0, 2048);
			cout << "请输入明文字符串：" << endl;
			cin>>s;
			text txt;
			in(s, txt);
			encrypt(txt);
			cout << "密文:";
			txt.cipherprint();
		}
		else if (c == '2')
		{
			cout << "文件md5计算\n请输入文件名" << endl;
			char filename[20] = {};
			cin >> filename;
			char file[50000] = {};
			int len = readfile(file, filename);
			text txt;
			in(file, txt, len);
			encrypt(txt);
			cout << "md5:";
			txt.cipherprint();
		}
		else if (c == '3')
		{
			cout << "文件完整性校验\n请输入md5" << endl;
			char inputmd5[33];
			cin >> inputmd5;
			cout << "请输入文件名\n";
			char filename[20] = {};
			cin >> filename;
			char file[50000] = {};
			int len = readfile(file, filename);
			text txt;
			in(file, txt, len);
			encrypt(txt);
			cout << "md5:";
			txt.cipherprint();

			char buf[35] = {};
			txt.tostring(buf);
			if (strcmp(buf, inputmd5) == 0)
			{
				cout << "文件完整未破坏" << endl;
			}
			else cout << "文件破坏" << endl;

		}
		else break;
	}

}


int readfile(char* file, char* filename)
{
	ifstream in(filename,ifstream::binary);
	if (!in)
	{
		cout << "error" << endl;
	}
	int len = 0;
	char t = in.get();
	while (in) {
		file[len++] = t;
		t = in.get();
	}
	cout <<"文件内容为："<< file << endl;
	in.close();
	return len;
}


int compare(newint a[4], newint b[4])
{
	int sum = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			int temp = a[i].a[j] ^ b[i].a[j];
			for (int k = 0; k < 8; k++)
			{
				if (temp & (1 << k))
				{
					sum++;
				}
			}
		}
	}
	return sum;
}




void newint::copy(newint b)
{
	for (int i = 0; i < 4; i++)
	{
		this->a[i] = b.a[i];
	}
}



void set(char m, int a, int i)//用于char和int的转换
{

}

void smallbit(char* temp, int a)
{
	a *= 8;
	int i = 0;
	if (a % 512 < 448)
	{
		i = a / 512 * 512 + 448;
	}
	else i = (a + 512) / 512 * 512 + 448;
	while (a)
	{
		temp[i] = (int)a % 2;
		a /= 2;
		i++;
	}


}

void smallbit(int len, newint& a)
{
	int i = 0;
	len *= 8;
	for (int i = 0; i < 4; i++)
	{
		a.a[i] = len % 256;
		len /= 256;
	}
}
void in(char* a, text& txt,int len)//输入明文
{
	memset(txt.m, 0, sizeof(txt.m));

	int index = 0;
	int i;

	
	i = len;
	//a[len]= char(128);
	//int len_t = len;
	int j;
	for (j = 0; j < i + 64; j++)
	{
		txt.m[j / 64][(j%64)/4].a[j % 4] = a[j]&0xff;
	}
	/*txt.m[i / 512][i / 4].a[i % 4] = a[i];
	i++;*/
	txt.m[i / 64][(i % 64) / 4].a[i % 4] = 128;
	i += 1;
	//int len = a.length();
	//small(len, txt.m[i / 512][14]);
	//set(len, txt.m[i / 512][14]);
	int t = i * 8 / 512;
	if (i * 8 % 512 > 448)
		t++;
	smallbit(len, txt.m[t][14]);
	big(A, txt.fabcd[0][0]);
	big(B, txt.fabcd[0][1]);
	big(C, txt.fabcd[0][2]);
	big(D, txt.fabcd[0][3]);
	txt.round = t;

}
void in(char* a, text& txt)//输入明文
{
	memset(txt.m, 0, sizeof(txt.m));

	int index = 0;
	int i;

	int len = strlen(a);
	i = len;
	//a[len]= char(128);
	//int len_t = len;

	for (int j = 0; j < i + 64; j++)
	{
		txt.m[j / 512][j / 4].a[j % 4] = a[j];
	}
	/*txt.m[i / 512][i / 4].a[i % 4] = a[i];
	i++;*/
	txt.m[i / 512][i / 4].a[i % 4] = 128;
	i += 1;
	//int len = a.length();
	//small(len, txt.m[i / 512][14]);
	//set(len, txt.m[i / 512][14]);
	int t = i * 8 / 512;
	if (i * 8 % 512 > 448)
		t++;
	smallbit(len, txt.m[t][14]);
	big(A, txt.fabcd[0][0]);
	big(B, txt.fabcd[0][1]);
	big(C, txt.fabcd[0][2]);
	big(D, txt.fabcd[0][3]);
	txt.round = t;

}
void big(unsigned int a, newint& b)
{
	memset(&b, 0, sizeof(b));
	int i = 0;
	while (a)
	{
		b.a[3 - i] = a % 256;
		a /= 256;
		i++;
	}
}
void small(unsigned int a, newint& b)
{
	memset(&b, 0, sizeof(b));
	int i = 0;
	while (a)
	{
		b.a[i] = a % 256;
		a /= 256;
		i++;
	}
}
newint::newint()
{
	memset(this, 0, sizeof(newint));
}

newint add_(newint a, newint b)
{
	newint c;
	for (int i = 0; i < 4; i++)
		c.a[i] = a.a[i] + b.a[i];
	for (int i = 0; i < 3; i++)
	{
		c.a[i + 1] += c.a[i] / 256;
		c.a[i] %= 256;
	}
	c.a[3] %= 256;
	return c;
}
newint add_big(newint a, newint b)
{
	newint c;
	for (int i = 0; i < 4; i++)
		c.a[i] = a.a[i] + b.a[i];
	for (int i = 3; i > 0; i--)
	{
		c.a[i - 1] += c.a[i] / 256;
		c.a[i] %= 256;
	}
	c.a[0] %= 256;
	return c;
}
newint and_(newint a, newint b)
{
	newint c;
	for (int i = 0; i < 4; i++)
	{
		c.a[i] = a.a[i] & b.a[i];
	}
	return c;
}

newint or_(newint a, newint b)
{
	newint c;
	for (int i = 0; i < 4; i++)
	{
		c.a[i] = a.a[i] | b.a[i];
	}
	return c;
}


newint xor_(newint a, newint b)
{
	newint c;
	for (int i = 0; i < 4; i++)
	{
		c.a[i] = a.a[i] ^ b.a[i];
	}
	return c;
}

newint not_(newint a)
{
	newint c;
	for (int i = 0; i < 4; i++)
	{
		c.a[i] = ~a.a[i] & 0xff;
	}
	return c;
}


unsigned int leftmove(unsigned int a, int s)
{
	unsigned int t1 = a >> (32 - s);
	unsigned int t2 = a << s;
	unsigned int temp = t2 + t1;
	return temp;
}

newint leftmove(newint a, int s)
{

	unsigned int temp = 0;
	for (int i = 0; i < 4; i++)
	{
		temp = a.a[3 - i] + temp * 256;
	}
	unsigned int t1 = temp >> (32 - s);
	unsigned int t2 = temp << s;
	temp = t2 + t1;

	newint c;
	small(temp, c);

	//newint c;
	//unsigned int temp[4];
	//for (int i = 0; i < 4; i++)
	//{
	//	temp[i]= a.a[i] >> (32 - s);
	//	c.a[i] = a.a[i]<<s;
	//}
	//for (int i = 0; i < 4; i++)
	//{
	//	c.a[(i + 1) % 4] += temp[i];
	//}
	return c;
}

unsigned int newint::value()
{
	unsigned temp = 0;
	for (int i = 3; i >= 0; i--)
	{
		temp = temp * 256 + a[i];
	}
	return temp;
}
void newint::set(unsigned int s)
{
	memset(a, 0, sizeof(a));
	int i = 0;
	while (s)
	{
		a[i] = s % 256;
		s /= 256;
		i++;
	}
}


void enF(int index, text& txt)//加密F
{
	for (int i = 0; i < 16; i++)
	{
		unsigned int a = txt.fabcd[i][0].value();
		unsigned int b = txt.fabcd[i][1].value();
		unsigned int c = txt.fabcd[i][2].value();
		unsigned int d = txt.fabcd[i][3].value();


		unsigned int temp = (b & c) | (~b & d);
		temp += a;
		unsigned int value = txt.m[index][i].value();
		temp += value;
		unsigned int tt = abs(sin(i + 1)) * pow(2, 32);
		temp += tt;
		temp = leftmove(temp, s[0][i]);
		temp += b;


		txt.fabcd[i + 1][0].copy(txt.fabcd[i][3]);//A.copy(D
		txt.fabcd[i + 1][2].copy(txt.fabcd[i][1]);//C.copy(B
		txt.fabcd[i + 1][3].copy(txt.fabcd[i][2]);//D.copy(C
		txt.fabcd[i + 1][1].set(temp);

	}
	for (int i = 0; i < 4; i++)
	{
		txt.gabcd[0][i].copy(txt.fabcd[16][i]);
	}
}
void enG(int index, text& txt)
{
	newint e[16];
	for (int i = 0; i < 16; i++)
	{
		e[i].copy(txt.m[index][(1 + 5 * i) % 16]);
	}
	for (int i = 0; i < 16; i++)
	{
		unsigned int a = txt.gabcd[i][0].value();
		unsigned int b = txt.gabcd[i][1].value();
		unsigned int c = txt.gabcd[i][2].value();
		unsigned int d = txt.gabcd[i][3].value();


		unsigned int temp = (b & d) | (c & ~d);
		temp += a;
		unsigned int value = e[i].value();
		temp += value;
		unsigned int tt = abs(sin(i + 17)) * pow(2, 32);
		temp += tt;
		temp = leftmove(temp, s[1][i]);
		temp += b;


		txt.gabcd[i + 1][0].copy(txt.gabcd[i][3]);//A.copy(D
		txt.gabcd[i + 1][2].copy(txt.gabcd[i][1]);//C.copy(B
		txt.gabcd[i + 1][3].copy(txt.gabcd[i][2]);//D.copy(C
		txt.gabcd[i + 1][1].set(temp);

	}

	for (int i = 0; i < 4; i++)
	{
		txt.habcd[0][i].copy(txt.gabcd[16][i]);
	}
}

void enH(int index, text& txt)
{
	newint e[16];
	for (int i = 0; i < 16; i++)
	{
		e[i].copy(txt.m[index][(5 + 3 * i) % 16]);
	}
	for (int i = 0; i < 16; i++)
	{
		unsigned int a = txt.habcd[i][0].value();
		unsigned int b = txt.habcd[i][1].value();
		unsigned int c = txt.habcd[i][2].value();
		unsigned int d = txt.habcd[i][3].value();


		unsigned int temp = (b ^ c ^ d);
		temp += a;
		unsigned int value = e[i].value();
		temp += value;
		unsigned int tt = abs(sin(i + 33)) * pow(2, 32);
		temp += tt;
		temp = leftmove(temp, s[2][i]);
		temp += b;


		txt.habcd[i + 1][0].copy(txt.habcd[i][3]);//A.copy(D
		txt.habcd[i + 1][2].copy(txt.habcd[i][1]);//C.copy(B
		txt.habcd[i + 1][3].copy(txt.habcd[i][2]);//D.copy(C
		txt.habcd[i + 1][1].set(temp);

	}

	for (int i = 0; i < 4; i++)
	{
		txt.iabcd[0][i].copy(txt.habcd[16][i]);
	}
}

void enI(int index, text& txt)
{
	newint e[16];
	for (int i = 0; i < 16; i++)
	{
		e[i].copy(txt.m[index][(7 * i) % 16]);
	}
	for (int i = 0; i < 16; i++)
	{
		unsigned int a = txt.iabcd[i][0].value();
		unsigned int b = txt.iabcd[i][1].value();
		unsigned int c = txt.iabcd[i][2].value();
		unsigned int d = txt.iabcd[i][3].value();


		unsigned int temp = (c ^ (b | (~d)));
		temp += a;
		unsigned int value = e[i].value();
		temp += value;
		unsigned int tt = abs(sin(i + 49)) * pow(2, 32);
		temp += tt;
		temp = leftmove(temp, s[3][i]);
		temp += b;


		txt.iabcd[i + 1][0].copy(txt.iabcd[i][3]);//A.copy(D
		txt.iabcd[i + 1][2].copy(txt.iabcd[i][1]);//C.copy(B
		txt.iabcd[i + 1][3].copy(txt.iabcd[i][2]);//D.copy(C
		txt.iabcd[i + 1][1].set(temp);

	}

	//for (int i = 0; i < 4; i++)
	//{
	//	txt.cipher[i].copy(txt.iabcd[16][i]);
	//}
}

void encrypt(text& txt)
{
	for (int i = 0; i <= txt.round; i++)
	{
		enF(i, txt);
		enG(i, txt);
		enH(i, txt);
		enI(i, txt);
		for (int j = 0; j < 4; j++)
		{

			txt.fabcd[0][j] = add_(txt.fabcd[0][j], txt.iabcd[16][j]);
		}
	}
}

void newint::print()
{
	for (int i = 0; i < 4; i++)
		printf("%02X", a[i]);
}
void text::cipherprint()
{
	for (int i = 0; i < 4; i++)
	{
		fabcd[0][i].print();
	}
	cout << endl;
}

void text::tostring(char* cipher)
{
	int len = 0;
	for (int i = 0; i < 4; i++)
	{
		char str[5] = {};
		for (int j = 0; j < 4; j++)
		{
			sprintf(str, "%02X", fabcd[0][i].a[j]);
			int s = strlen(str);
			for (int k = 0; k < s; k++)
			{
				cipher[len++] = str[k];
			}
		}
	}
}