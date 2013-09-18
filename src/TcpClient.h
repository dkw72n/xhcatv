#pragma once
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define MAX_BUF_SIZE 1024
#define NODATA 0
#define GOTDATA 2
#define ERR 1
#define OK 0
class TcpClient
{
public:
	TcpClient(){m_InitFlag = 0;m_Client=0;}
	int Init(const char *, u_short);
	int Connect();
	int Send(const char *, int);
	int Recv(char *,int);
	int HasData();
	int CleanUp();
	int UnInit();
private:
	int m_InitFlag;
	SOCKET m_Client;
	SOCKADDR_IN m_Server;
	WSADATA m_WSAData;
	fd_set m_fdread;
};