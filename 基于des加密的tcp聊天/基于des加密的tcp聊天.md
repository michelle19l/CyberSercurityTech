# 网络安全技术实验报告

学  院		网安学院
年  级		大三
班  级		信息安全单学位班 
学  号	 	1811494
姓  名		刘旭萌
手机号		17320205058

[TOC]

## 一、实验目的

​		DES（Data Encryption Standard）算法是一种用 56 位有效密钥来加密 64 位数据的对称 分组加密算法，该算法流程清晰，已经得到了广泛的应用，算是应用密码学中较为基础的加 密算法。TCP（传输控制协议）是一种面向链接的、可靠的传输层协议。TCP 协议在网络层 IP 协议的基础上，向应用层用户进程提供可靠的、全双工的数据流传输。 本章训练的目的如下。 
​	①理解 DES 加解密原理。 
​	②理解 TCP 协议的工作原理。 
​	③掌握 linux 或Windows下基于 socket 的编程方法。 

本章训练的要求如下。 
​	①利用 socket 编写一个 TCP 聊天程序。 
​	②通信内容经过 DES 加密与解密。

（正文采用宋体小四，间距20磅）

 

## 二、实验内容

在 Linux 或Windows平台下，实现基于 DES 加密的 TCP 通信，具体要求如下。 

​	① 能够在了解 DES 算法原理的基础上，编程实现对字符串的 DES 加密解密操作。 
​	② 能够在了解 TCP 和 Linux 平台下的 Socket 运行原理的基础上，编程实现简单的 TCP 通信，为简化编程细节，不要求实现一对多通讯。 
​	③ 将上述两部分结合到一起，编程实现通信内容事先通过 DES 加密的 TCP 聊天程序， 要求双方事先互通密钥，在发送方通过该密钥加密，然后由接收方解密，保证在网络上传输 的信息的保密性。

 

 

## 三、实验步骤及实验结果

### DES加密



### TCP通信

#### 通信模型图

本次实验为了满足**多对多通信**，采用**C/S**模型进行设计，利用**server**对消息进行==**转发**==以实现**多个client端**之间的通信

<img src="%E5%9F%BA%E4%BA%8Edes%E5%8A%A0%E5%AF%86%E7%9A%84tcp%E8%81%8A%E5%A4%A9.assets/image-20210330201611866.png" alt="image-20210330201611866" style="zoom:33%;" />

#### TCP通信原理

![image-20210330202444178](%E5%9F%BA%E4%BA%8Edes%E5%8A%A0%E5%AF%86%E7%9A%84tcp%E8%81%8A%E5%A4%A9.assets/image-20210330202444178.png)

#### 代码

##### 初始设置

```C++
#include <WinSock2.h>//windows socket 编程头文件
#pragma comment(lib,"ws2_32.lib")//链接ws2_32.lib库文件到此项目中﻿
```

```C++
//加载socket库
WSADATA wsaData;
//MAKEWORD(2.2),成功返回0
if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
{
    cout << "socket初始化失败" << endl;
    return 0;
```

##### socket的创建 socket()

```c++
//创建一个socket，并将该socket绑定到一个特定的传输层
sockSer = socket(AF_INET, SOCK_STREAM, 0);//地址类型（ipv4），服务类型（流式套接字）
```

##### server绑定套接字和地址 bind()

```C++
//初始化地址，client端地址初始化方法相同
addrSer.sin_addr.s_addr = inet_addr("127.0.0.1");//本次由于只在内网中测试，使用私有地址
addrSer.sin_family = AF_INET;
addrSer.sin_port = htons(PORT);

//绑定
if (bind(sockSer, (SOCKADDR*)&addrSer, sizeof(SOCKADDR)) == -1)
    cout << "bind error" << endl;//强制类型转换，SOCKADDR_IN方便赋值 SOCKADDR方便传输
```

##### server生成初始密钥

本次加密统一采用**64位**的二进制串作为初始密钥

```C++
string initial_key()//server生成64位初始密钥
{
	string key="";
	for (int i = 0; i < 8; i++)
	{
		key += rand() % 128;
	}
	return key;
}
```

```C++
string key = initial_key();
```

##### server进行监听 listen()

```C++
listen(sockSer, 5);//等待队列最大长度为5
```

##### client发起连接请求 connect()



##### server端受连接请求 accept()

```C++
SOCKET sockConn[CLIENTNUM];//和多个client连接
```
server端接受连接后，向client发送**clientID**和**会话密钥**
```C++
sockConn[i] = accept(sockSer, (SOCKADDR*)&addrCli, &len);//失败sockConn=INVALID_SOCKET
if (sockConn[i] != INVALID_SOCKET)
{
    cond++;//人数加一
    string buf = "你的id是：";
    buf+= 48 + i;//简化，最多九个人
    send(sockConn[i], buf.data(), 50, 0);
    send(sockConn[i], key.data(), 50, 0);
    cout << "clients "<<i<<" have connected" << endl;
}
```

为了便于实验观察，这里client会将密钥打印出来

<img src="%E5%9F%BA%E4%BA%8Edes%E5%8A%A0%E5%AF%86%E7%9A%84tcp%E8%81%8A%E5%A4%A9.assets/image-20210330205801454.png" alt="image-20210330205801454" style="zoom:80%;" />



##### client加工初始密钥



##### client发送/接受数据



##### server转发报文

收到的message格式为，所以第一位是**发送方ID**，第二位是**接收方ID**

```C++
class msg_form
{
public:

	char from_name;//发送方标号
	char to_name;//接收方
	char msg[BUF_SIZE];//消息内容
    ...
}
```

需要提取接收方ID，并顶向传送消息

```C++
char recvBuf[BUF_SIZE] = {};
recv(sockettemp, recvBuf, 1000, 0);
if (recvBuf[0])
{
    cout << recvBuf << endl;
    msg_form m = string_to_msg(recvBuf);
    int id = m.to_name - 48;
    send(sockConn[id], recvBuf, 50, 0);
    ...
} 
```



通信过程样例：

1. 输入目的clientid和消息内容

   <img src="%E5%9F%BA%E4%BA%8Edes%E5%8A%A0%E5%AF%86%E7%9A%84tcp%E8%81%8A%E5%A4%A9.assets/image-20210330205932982.png" alt="image-20210330205932982" style="zoom:80%;" />

2. server收到消息第一位为发送方ID，第二位为目的方ID，后面为消息密文，可以看到，**消息已经成功加密并以密文形式传输**

   ![image-20210330210048676](%E5%9F%BA%E4%BA%8Edes%E5%8A%A0%E5%AF%86%E7%9A%84tcp%E8%81%8A%E5%A4%A9.assets/image-20210330210048676.png)

3. client0收到消息并进行解密

   <img src="%E5%9F%BA%E4%BA%8Edes%E5%8A%A0%E5%AF%86%E7%9A%84tcp%E8%81%8A%E5%A4%A9.assets/image-20210330210214286.png" alt="image-20210330210214286" style="zoom:80%;" />

##### 程序的退出

client端输入`quit`，向server发送quit消息后主动退出

server端收到client发送的`quit`指令判断该client退出，当所有client都断开连接时退出程序，关闭socket

```C++
if (!strcmp(m.msg, "quit"))
{
    cout << "用户" << (int)lparam << "退出聊天" << endl;
    cond--; if (!cond)return 0;
}
```

关闭socket

```C++
closesocket(sockSer);
WSACleanup();
```

例如，

1. client1发送`quit`,发送后客户端退出，server显示该用户断开连接![image-20210330210327868](%E5%9F%BA%E4%BA%8Edes%E5%8A%A0%E5%AF%86%E7%9A%84tcp%E8%81%8A%E5%A4%A9.assets/image-20210330210327868.png)a

2. client0发送`quit`，所有连接断开，server退出<img src="%E5%9F%BA%E4%BA%8Edes%E5%8A%A0%E5%AF%86%E7%9A%84tcp%E8%81%8A%E5%A4%A9.assets/image-20210330211017141.png" alt="image-20210330211017141" style="zoom:80%;" />



 

 

## 四、实验遇到的问题及其解决方法

本次实验中。。。。

 

## 五、实验结论

实验感想、从实验中学到了什么。。。。

 

备注：（正文采用宋体小四，间距20磅）

以上说明仅供参考。实验报告从这几部分展开，具体内容可自由发挥。如有雷同，均按零分处理。

 

 

 