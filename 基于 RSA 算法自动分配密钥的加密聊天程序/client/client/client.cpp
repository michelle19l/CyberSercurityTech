#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS﻿
//client

#define BUF_SIZE 2048
#define PORT 6666
#pragma comment(lib,"ws2_32.lib")//链接ws2_32.lib库文件到此项目中

#include "tcpfunction.h"
#include "RSA.h"




using namespace std;
DWORD WINAPI handlerRequest1(LPVOID lparam);
DWORD WINAPI handlerRequest2(LPVOID lparam);

//创建socket
SOCKET sockSer, sockCli;//服务器和客户端的socket
SOCKADDR_IN addrSer, addrCli;//ip+端口号
int len = sizeof(SOCKADDR_IN);


int ID;

char sendBuf[BUF_SIZE] = {};
char recvBuf[BUF_SIZE] = {};

//线程
HANDLE hThread1, hThread2;
DWORD dwThreadId1, dwThreadId2;

u_char key_final[16][6] = {};
u_char key_final_[16][6] = {};//解密
//状态码
int cond;
big n, e;
int main()
{
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
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);//地址类型（ipv4），服务类型（流式套接字）

	// 设置超时
	struct timeval timeout;
	timeout.tv_sec = 1;//秒
	timeout.tv_usec = 0;//微秒
	if (setsockopt(sockClient, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == -1) {
		cout << "setsockopt failed:";
	}

	cout << "Client" << endl;

	//client
	addrCli.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrCli.sin_family = AF_INET;
	int port;
	cout << "请输入端口号：";
	cin >> port;
	addrCli.sin_port = htons(port);


	//初始化server地址
	addrSer.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrSer.sin_family = AF_INET;
	
	addrSer.sin_port = htons(PORT);

	//绑定
	//bind(sockSer, (SOCKADDR*)&addrSer, sizeof(SOCKADDR));//强制类型转换，SOCKADDR_IN方便赋值 SOCKADDR方便传输

	////监听
	//cout << "listening" << endl;
	//listen(sockSer, 5);

	


	cout << "connecting" << endl;
	sockCli = connect(sockClient, (SOCKADDR*)&addrSer, sizeof(SOCKADDR));
	
	
	if (sockCli != SOCKET_ERROR)
	{
		cout << "connected" << endl;
		while (1)
		{
			char recvBuf[BUF_SIZE] = {};
			
			recv(sockClient, recvBuf, 50, 0);
			cout << recvBuf << endl;
			ID = recvBuf[10] - 48;
			//接收n
			memset(recvBuf, 0, sizeof(recvBuf));
			recv(sockClient, recvBuf, 1024, 0);
			n.stringtonum(recvBuf);
			//接收e
			memset(recvBuf, 0, sizeof(recvBuf));
			recv(sockClient, recvBuf, 1024, 0);
			e.stringtonum(recvBuf);
			cout << "RSA公钥： ";
			cout << "n= "; n.print();
			cout << "e= "; e.print();
			string key = initial_key();
			
			char mtext[8];
			for (int i = 0; i < 8; i++)
				mtext[i] = key.data()[i];
			//strcpy_s(mtext,key.data());
			char* ciphertext;
			ciphertext = new char[512];
			RSA_ secretkey(n, e);
			rsa_en_text(mtext, ciphertext, secretkey);
			send(sockClient, ciphertext, 512, 0);

			
			//生成密钥
			getkeys((u_char*)key.data(), key_final);
			for (int i = 0; i < 16; i++)
				for (int j = 0; j < 6; j++)
					key_final_[15 - i][j] = key_final[i][j];
			for (int i = 0; i < 8; i++)
				printf("%02X ", recvBuf[i]);
			cout << endl;
			break;
			Sleep(30);
		}
	}
	//while (1)
	{

		//if (sockCli != INVALID_SOCKET)
		while(1)
		{
				hThread1 = ::CreateThread(NULL, NULL, handlerRequest1, LPVOID(sockClient), 0, &dwThreadId1);

				hThread2 = ::CreateThread(NULL, NULL, handlerRequest2, LPVOID(sockClient), 0, &dwThreadId2);
				WaitForSingleObject(hThread1, 200);
				WaitForSingleObject(hThread2, 200);
				/*CloseHandle(hThread2);
				CloseHandle(hThread1);*/
				if (cond) break;
			


		}
	}
	closesocket(sockClient);
	WSACleanup();
	return 0;
}
DWORD WINAPI handlerRequest1(LPVOID lparam)
{
	while (1)
	{
		char sendBuf[BUF_SIZE] = {};
		char buffer[BUF_SIZE] = {};
		SOCKET socketClient = (SOCKET)(LPVOID)lparam;
		//cin >> buffer;
		cin.getline(buffer, 2048, '\n');
		if (buffer[0])
		{
			sendBuf[0] = ID + 48;
			msg_form m = string_to_msg(sendBuf);
			if (!strcmp(m.msg, "quit") || !strcmp(buffer, "quit"))
			{
				sendBuf[1] = ID + 48;
				strcat(sendBuf, buffer);
				send(socketClient, sendBuf, 2048, 0);
				cond = 1;

				return 0;
			}
			sendBuf[1] = buffer[0];
			int i;
			for (i = 1; buffer[i] != 0; i++)
				buffer[i - 1] = buffer[i];
			buffer[i - 1] = 0;
			char buf[BUF_SIZE] = {};
			msg_en(buffer, key_final,buf);
			
			strcat(sendBuf, buf);
			send(socketClient, sendBuf, 2048, 0);

			//msg_form m = string_to_msg(sendBuf);
			//if (!strcmp(m.msg, "quit") || !strcmp(buffer, "quit"))
			//	//recv(socketClient, recvBuf, 50, 0);
			//{
			//	cond = 1;
			//	
			//}
			//else return 0;
		}
		Sleep(200);
	}
	return 0;
}

DWORD WINAPI handlerRequest2(LPVOID lparam)
{
	while (1)
	{
		char recvBuf[BUF_SIZE] = {};
		SOCKET socketClient = (SOCKET)(LPVOID)lparam;
		//send(socketClient, sendBuf, 2048, 0);
		recv(socketClient, recvBuf, 2048, 0);
		if (recvBuf[0])
		{
			cout << recvBuf[0] << ": ";
			int i;
			for (i = 2; recvBuf[i] != 0; i++)
				recvBuf[i - 2] = recvBuf[i];
			recvBuf[i - 2] = 0;
			recvBuf[i-1]=0;
			char buf[BUF_SIZE] = {};
			msg_de(recvBuf, key_final_, buf);
			cout << buf << endl;
			int k = 1;
			//msg_form m = string_to_msg(recvBuf);
			/*if (!strcmp(m.msg, "quit")||!strcmp(recvBuf,"quit"))
			{
				cond = 1;
			}*/
			//cout << m.from_name << ": " << m.msg << endl;

		}
		Sleep(200);
	}
	//closesocket(socketClient);
	return 0;
}
