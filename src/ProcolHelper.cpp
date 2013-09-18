#include "stdafx.h"
#include "ProtocolHelper.h"
#include "misc.h"
int ProtocolHelper::Init()
{
	m_buf = new char[1024];
	m_Channel.Init("10.192.4.29",5018);
	return 0;
}
int ProtocolHelper::UnInit()
{
	m_Channel.UnInit();
	delete[] m_buf;
	m_buf = 0;
	return 0;
}

int ProtocolHelper::CheckCHKReply()
{
	int iRet = m_Channel.HasData();
	if(iRet == NODATA)
		return CHK_NODATA;
	else if(iRet == ERR)
		return CHK_FAIL;
	//assert iRet = GOTDATA
	iRet = m_Channel.Recv(m_buf,1024);
	if(iRet == OK && strncmp(CHKOK ,m_buf,strlen(CHKOK)) == 0)
		return CHK_SUCCESS;
	else
		return CHK_FAIL;
}

int ProtocolHelper::CheckCONReply()
{
	int iRet = m_Channel.HasData();
	if(iRet == NODATA)
		return CHK_NODATA;
	else if(iRet == ERR)
		return CHK_FAIL;
	//assert iRet = GOTDATA
	iRet = m_Channel.Recv(m_buf,1024);
	if(iRet == OK && strncmp(CONOK,m_buf,strlen(CONOK)) == 0)
		return CHK_SUCCESS;
	else
		return CHK_FAIL;
}

int ProtocolHelper::CheckINFReply()
{
	int iRet = m_Channel.HasData();
	if(iRet == NODATA)
		return CHK_NODATA;
	else if(iRet == ERR)
		return CHK_FAIL;
	//assert iRet = GOTDATA
	iRet = m_Channel.Recv(m_buf,1024);
	if(iRet == OK && strncmp(INFOK,m_buf,strlen(INFOK)) == 0)
		return CHK_SUCCESS;
	else
		return CHK_FAIL;
}

int ProtocolHelper::CheckMACReply()
{
	int iRet = m_Channel.HasData();
	if(iRet == NODATA)
		return CHK_NODATA;
	else if(iRet == ERR)
		return CHK_FAIL;
	//assert iRet = GOTDATA
	iRet = m_Channel.Recv(m_buf,1024);
	if(iRet == OK && strncmp(MACOK,m_buf,strlen(MACOK)) == 0)
		return CHK_SUCCESS;
	else
		return CHK_FAIL;
}

int ProtocolHelper::CheckNATReply()
{
	int iRet = m_Channel.HasData();
	if(iRet == NODATA)
		return CHK_NODATA;
	else if(iRet == ERR)
		return CHK_FAIL;
	//assert iRet = GOTDATA
	iRet = m_Channel.Recv(m_buf,1024);
	if(iRet == OK && strncmp(NATOK,m_buf,strlen(NATOK)) == 0)
		return CHK_SUCCESS;
	else
		return CHK_FAIL;
}

int ProtocolHelper::CleanUp()
{
	m_Channel.CleanUp();
	Sleep(500);
	return 0;
}

int ProtocolHelper::Connect()
{
	int iRet = m_Channel.Connect();
	return iRet;
}

int ProtocolHelper::SendBeat()
{
	int iRet = m_Channel.Send("qr",2);
	return iRet;
}

int ProtocolHelper::SendCheckNAT()
{
	int iRet = m_Channel.Send("checknat",8);
	return iRet;
}

int ProtocolHelper::SendInformation()
{
	char *myInfo = GetInformationEncoded();
	int iRet = m_Channel.Send(myInfo, strlen(myInfo));
	return iRet;
}

int ProtocolHelper::SendMACAddress()
{
	char *myMac = GetMacAddress();
	int iRet = m_Channel.Send(myMac, strlen(myMac));
	return iRet;
}

int ProtocolHelper::SendNATVersion()
{
	int iRet = m_Channel.Send("natversion:1",12);
	return iRet;
}
