#pragma comment(lib,"ws2_32.lib")//链接ws2_32.lib库文件到此项目中﻿
//server.cpp


#include "tcpfunction.h"

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


SOCKET sockConn[CLIENTNUM];//和两个client连接，可以改成多个

//server端状态码指的是当前在线人数
int cond;
//client和socket标号对应表

int main()
{


	//msg_form a('8','j',"assd");
	//cout << msg_to_string(a) << endl;
	//int g = 0;



	cout << "Server" << endl;
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

	//初始化地址
	addrSer.sin_addr.s_addr = inet_addr("192.168.89.1");
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons(PORT);

	//绑定
	if (bind(sockSer, (SOCKADDR*)&addrSer, sizeof(SOCKADDR)) == -1)
		cout << "bind error" << endl;//强制类型转换，SOCKADDR_IN方便赋值 SOCKADDR方便传输


	//监听
	cout << "listening" << endl;
	u_char *key=new u_char[8]; 
	//getkey(key);
	for (int i = 0; i < CLIENTNUM; i++)
	{
		listen(sockSer, 5);
		sockConn[i] = accept(sockSer, (SOCKADDR*)&addrCli, &len);//失败sockConn=INVALID_SOCKET
		if (sockConn[i] != INVALID_SOCKET)
		{
			cond++;//人数加一
			char buf[20] = "你的id是：";
			buf[10] = 48 + i;//简化，最多九个人
			for (int i = 11; i < 19; i++)
			{
				buf[i] = key[i - 11];
			}
			buf[19] = 0;
			send(sockConn[i], buf, 50, 0);
			cout << CLIENTNUM << " clients have connected" << endl;
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
	////while (1)
	//{
	//	//if (sockConn != INVALID_SOCKET)
	//	{
	//		while (1)
	//		{
	//			hThread1 = ::CreateThread(NULL, NULL, handlerRequest1, LPVOID(sockConn), 0, &dwThreadId1);
	//			hThread2 = ::CreateThread(NULL, NULL, handlerRequest2, LPVOID(sockConn), 0, &dwThreadId2);
	//			WaitForSingleObject(hThread1, 20);
	//			WaitForSingleObject(hThread2, 20);
	//			CloseHandle(hThread2);
	//			CloseHandle(hThread1);
	//			if (cond)break;

	//		}

	//	}

	//}
	closesocket(sockSer);
	WSACleanup();
	return 0;
}

DWORD WINAPI handlerRequest(LPVOID lparam)
{
	SOCKET sockettemp = sockConn[(int)lparam];
	char recvBuf[BUF_SIZE] = {};
	recv(sockettemp, recvBuf, 1000, 0);
	if (recvBuf[0])
	{
		/*if (!strcmp(recvBuf, "quit"))
		{
			cond = 1;
		}*/
		cout << recvBuf << endl;
		msg_form m = string_to_msg(recvBuf);
		int id = m.to_name - 48;
		cout << id << endl;
		send(sockConn[id], recvBuf, 50, 0);
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