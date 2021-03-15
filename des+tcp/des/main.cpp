#include"funcDef.cpp"
#include"testdata.h"
int test(u_char a[8], u_char b[8])//对比两个数组的元素是否相同
{
	int flag = 1;
	for (int i = 0; i < 8; i++)
		if (a[i] != b[i])
		{
			flag = 0;
			break;
		}
	return flag;
}

void funcEn(des_test_case m)//加密过程
{
	u_char key_final[16][6] = {};
	//生成密钥
	getkeys(m.key, key_final);
	u_char cipher[8] = {};
	round(m.txt, key_final, cipher);
	cout<<test(m.out, cipher)<<endl;
}

void funcDe(des_test_case c)
{
	u_char key_final_[16][6] = {};
	u_char key_final[16][6] = {};
	getkeys(c.key, key_final_);
	for (int i = 0; i < 16; i++)
		for (int j = 0; j < 6; j++)
			key_final[15 - i][j] = key_final_[i][j];
	u_char m[8] = {};
	round(c.txt, key_final, m);
	cout << test(c.out, m) << endl;
}

int main()
{
	for (int i = 0; i < 20; i++)
	{
		if (cases[i].mode == 1)
		{
			cout << "加密" << cases[i].num << endl;
			funcEn(cases[i]);
			cout << "=====================" << endl;
		}
		else 
		{ 
			cout << "解密" << cases[i].num << endl;
			funcDe(cases[i]); 
			cout << "=====================" << endl;
		}
	}
}