#include "stdafx.h"
#include "TcpClient.h"
#define PROCESS_ERROR(x) do{if(!(x))goto Exit1;}while(0)


int TcpClient::Init(const char *szIP, u_short nPort)
{
	memset(&m_Server, 0 ,sizeof(m_Server));
	m_Server.sin_family = AF_INET;
	m_Server.sin_addr.S_un.S_addr = inet_addr(szIP);
	m_Server.sin_port = htons(nPort);
	m_InitFlag = 1;
	//puts("TcpClient Init");
	return 0;
}

int TcpClient::Connect()
{
	int ret = ERR, tmp;
	PROCESS_ERROR(m_InitFlag);

	m_Client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	PROCESS_ERROR(m_Client);

	tmp = connect(m_Client, (struct sockaddr *)&m_Server, sizeof(SOCKADDR_IN));
	PROCESS_ERROR(tmp != SOCKET_ERROR);

	ret = OK;
Exit1:
	if(ERR == ret){
		CleanUp();
	}
	//printf("TcpClient Connect: result %d\n", ret);
	return ret;
}

int TcpClient::Send(const char *szBuf, int nLen)
{
	int ret = ERR, tmp;
	PROCESS_ERROR(m_Client);
	PROCESS_ERROR(nLen > 0);
	tmp = send(m_Client, szBuf, nLen, 0);
	PROCESS_ERROR(SOCKET_ERROR != tmp);

	ret = OK;
Exit1:
	if(ERR == ret){
		CleanUp();
	}
	//printf("Tcpclient Senf: %d\n", ret);
	return ret;
}

int TcpClient::Recv(char *szBuf, int nLen)
{
	int ret = ERR, tmp;
	PROCESS_ERROR(m_Client);
	PROCESS_ERROR(nLen > 1);

	tmp = recv(m_Client, szBuf, nLen-1, 0);
	PROCESS_ERROR(SOCKET_ERROR != tmp);

	szBuf[tmp]='\0';
	ret = OK;
Exit1:
	if(ERR == ret){
		CleanUp();
	}
	//printf("Tcpclient Recv: %d\n", ret);
	return ret;
}

int TcpClient::HasData()
{
	timeval tv = {0,0};
	int ret = ERR, tmp;
	PROCESS_ERROR(m_Client);
	FD_ZERO(&m_fdread);
	FD_SET(m_Client, &m_fdread);
	tmp = select(0, &m_fdread, NULL, NULL, &tv);
	PROCESS_ERROR(SOCKET_ERROR != tmp);

	ret = tmp?GOTDATA:NODATA;
Exit1:
	if(ERR == ret){
		CleanUp();
	}
	//printf("Tcpclient hasdata: %d\n", ret);
	return ret;
}

int TcpClient::CleanUp()
{
	FD_ZERO(&m_fdread);
	PROCESS_ERROR(m_Client);
	closesocket(m_Client);
	m_Client = 0;
Exit1:
	return OK;
}

int TcpClient::UnInit()
{
	FD_ZERO(&m_fdread);
	if(m_Client){
		closesocket(m_Client);
		m_Client = 0;
	}
	return 0;
}
