#pragma comment(lib,"ws2_32.lib")//链接ws2_32.lib库文件到此项目中﻿
//server.cpp


#include "tcpfunction.h"
#include "RSA.h"

using namespace std;


//
DWORD WINAPI handlerRequest(LPVOID lparam);
//DWORD WINAPI handlerRequest2(LPVOID lparam);
//创建socket
SOCKET sockSer, sockCli;//服务器和客户端的socket
SOCKADDR_IN addrSer, addrCli;//ip+端口号
int len = sizeof(SOCKADDR_IN);

HANDLE hThread, hThread2;
DWORD dwThreadId, dwThreadId2;


SOCKET* sockConn;//和两个client连接，可以改成多个
u_char** deskey;//每个socket的deskey

struct finalkey {
	u_char key_final[16][6] = {};
	u_char key_final_[16][6] = {};
}*deskey_final;

//server端状态码指的是当前在线人数
int cond;
//client和socket标号对应表
int CLIENTNUM = 2;
int main()
{

	cout << "Server" << endl;
	cout << "请输入聊天室人数：" << endl;
	cin >> CLIENTNUM;
	sockConn = new SOCKET[CLIENTNUM];
	deskey = new unsigned char* [CLIENTNUM];
	deskey_final = new finalkey [CLIENTNUM];
	for (int i = 0; i < CLIENTNUM; i++)
	{
		deskey[i] = new unsigned char[8];
	}
	
	//加载socket库
	WSADATA wsaData;
	//MAKEWORD(2.2),成功返回0
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "socket初始化失败" << endl;
		return 0;
	}

	//创建Socket
	//创建一个socket，并将该socket绑定到一个特定的传输层
	sockSer = socket(AF_INET, SOCK_STREAM, 0);//地址类型（ipv4），服务类型（流式套接字）

	// 设置超时
	struct timeval timeout;
	timeout.tv_sec = 1;//秒
	timeout.tv_usec = 0;//微秒
	if (setsockopt(sockSer, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == -1) {
		cout << "setsockopt failed:";
	}


	//初始化地址
	addrSer.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons(PORT);

	//绑定
	if (bind(sockSer, (SOCKADDR*)&addrSer, sizeof(SOCKADDR)) == -1)
		cout << "bind error" << endl;//强制类型转换，SOCKADDR_IN方便赋值 SOCKADDR方便传输


	//监听
	cout << "listening" << endl;
	//string key = initial_key();
	cout << "开始生成密钥" << endl;
	big e;
	e.set(0x10001);
	cout << "tips 本程序将生成512bit的p和q，生成和计算时间较长" << endl;
	//e.set(5);
	cout << "是否使用默认密钥对？1是 2否" << endl;
	int flag;
	cin >> flag;
	prime p, q;
	if (flag == 1)
	{
		p.set("d0af256c8c72facbc0051054813505a340f899bcb05f4dfb83f2a9a4d14eabd312daa80c24bd772b37fe7cf4a39b8803c37a905cab66365b1721ca4400005a49");
		q.set("a698494d2c26ca02ebf5284584c6224ab88e46bd27242f12dfec9cb577df385a3878a22d32aa06224e724f25ce93e06a96691b9d56d08f321733ff9500005d11");
	}
	else if(flag==2)
	{
		p.getprime();
		q.getprime();
	}
	//p.number.set(3);
	cout << "p= "; p.number.print();
	//q.number.set(7);
	cout << "q= "; q.number.print();
	cout << "开始生成密钥对" << endl;
	RSA_ rsa(p.number, q.number, e);
	cout << "n= "; rsa.n.print();
	cout << "d= "; rsa.d.print();
	cout << "e= "; rsa.e.print();
	cout << "密钥对已生成" << endl;
	for (int i = 0; i < CLIENTNUM; i++)
	{
		listen(sockSer, 5);
		sockConn[i] = accept(sockSer, (SOCKADDR*)&addrCli, &len);//失败sockConn=INVALID_SOCKET
		if (sockConn[i] != INVALID_SOCKET)
		{
			cond++;//人数加一
			string buf = "你的id是：";

			buf += 48 + i;//简化，最多九个人

			send(sockConn[i], buf.data(), 50, 0);
			//send(sockConn[i], key.data(), 50, 0);
			

			char buf_[1024] = {};
			rsa.n.numtostring(buf_);
			send(sockConn[i], buf_,1024, 0);//发送n
			memset(buf_,1024, 0);
			e.numtostring(buf_);
			send(sockConn[i], buf_, 512, 0);
			
			//接收加密后的密钥
			memset(buf_, 0, sizeof(buf_));
			while (1)
			{
				recv(sockConn[i], buf_, 1024, 0);
				//对密钥进行解密
				if (buf_[0])
				{
					char* mtext = new char[512];
					rsa_de_text(mtext, buf_, rsa);
					//for (int i = 0; i < 16; i++)
					//	mtext[i] = buf_[i];
					cout << "DES密钥为";
					memset(deskey[i], 0, 8);
					for (int j = 0; j < 8; j++)
					{
						deskey[i][j] += mtext[j * 2];
						deskey[i][j] += mtext[j * 2 + 1] * 16;
						cout << hex << (int)deskey[i][j]<<" ";
					}
					cout << endl;
					getkeys((u_char*)deskey[i], deskey_final[i].key_final);
					for (int k = 0; k < 16; k++)
						for (int j = 0; j < 6; j++)
							deskey_final[i].key_final_[15 - k][j] = deskey_final[i].key_final[k][j];
					break;
				}
			}
			
			cout << "clients " << i << " have connected" << endl;
		}

	}
	if (cond == CLIENTNUM)
	{
		
		while (1)
		{
			for (int i = 0; i < CLIENTNUM; i++)
			{

				hThread = ::CreateThread(NULL, NULL, handlerRequest, LPVOID(i), 0, &dwThreadId);
				WaitForSingleObject(hThread, 200);

			}
			if (!cond)break;
		}
	}
	cout << "all clinets closed up" << endl;
	closesocket(sockSer);
	WSACleanup();
	return 0;
}

DWORD WINAPI handlerRequest(LPVOID lparam)
{
	SOCKET sockettemp = sockConn[(int)lparam];
	char recvBuf[BUF_SIZE] = {};
	recv(sockettemp, recvBuf, 2048, 0);
	if (recvBuf[0])
	{
		/*if (!strcmp(recvBuf, "quit"))
		{
			cond = 1;
		}*/

		//cout << recvBuf << endl;
		msg_form m = string_to_msg(recvBuf);
		int id = m.to_name - 48;
		//cout << id << endl;
		msg_form tmp;
		char buf[BUF_SIZE] = {};

		msg_de(m.msg, deskey_final[(int)lparam].key_final_, buf);
		cout << buf << endl;
		msg_en(buf, deskey_final[id].key_final, tmp.msg);
		tmp.from_name = m.from_name;
		tmp.to_name = m.to_name;
		send(sockConn[id], (const char*)&tmp, 2048, 0);
		//send(sockConn[id], recvBuf, 2048, 0);
		if (!strcmp(m.msg, "quit"))
		{
			cout << "用户" << (int)lparam << "退出聊天" << endl;
			cond--; if (!cond)return 0;
		}
	}
	return 0;
}




DWORD WINAPI handlerRequest1(LPVOID lparam)
{
	char sendBuf[BUF_SIZE] = {};
	//cout << "send" << endl;
	SOCKET socketClient = (SOCKET)(LPVOID)lparam;
	cin.getline(sendBuf, 2048, '\n');
	send(socketClient, sendBuf, 2048, 0);
	if (!strcmp(sendBuf, "quit"))
		//recv(socketClient, recvBuf, 50, 0);
		cond = 1;
	return 0;
}

DWORD WINAPI handlerRequest2(LPVOID lparam)
{
	char recvBuf[BUF_SIZE] = {};
	//cout << "recv" << endl;
	SOCKET socketClient = (SOCKET)(LPVOID)lparam;
	//send(socketClient, sendBuf, 2048, 0);
	recv(socketClient, recvBuf, 50, 0);
	if (recvBuf[0])
	{
		if (!strcmp(recvBuf, "quit"))
		{
			cond = 1;
		}
		cout << endl << "收到信息: " << recvBuf << endl;
	}
	//if (!strcmp(recvBuf, "我要下线了"))
	//{
	//	send(socketClient, "88", 2048, 0);
	//	return 0;
	//}
	//if (!strcmp(recvBuf, "88"))
	//	return 0;
		//closesocket(socketClient);
	return 0;
}







//cout << "connected" << endl;
//recv(sockConn, recvBuf, 2048, 0);
//cout << "收到消息：" << recvBuf << endl;
//cin >> sendBuf;
//cout << sendBuf << endl;
//send(sockConn, sendBuf, strlen(sendBuf), 0);
//cout << "connected" << endl;


//#include"deshead.h"
//#include"testdata.h"
//
//
//
//int main()
//{
//	for (int i = 0; i < 20; i++)
//	{
//		if (cases[i].mode == 1)
//		{
//			cout << "加密" << cases[i].num << endl;
//			funcEn(cases[i]);
//			cout << "=====================" << endl;
//		}
//		else
//		{
//			cout << "解密" << cases[i].num << endl;
//			funcDe(cases[i]);
//			cout << "=====================" << endl;
//		}
//	}
//}