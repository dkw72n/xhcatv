#pragma once
#include "TcpClient.h"
#define CHK_FAIL 1
#define CHK_NODATA 2
#define CHK_SUCCESS 0
class ProtocolHelper
{
public:
	int Init();
	int UnInit();
	int SendBeat();
	int SendMACAddress();
	int SendNATVersion();
	int SendInformation();
	int SendCheckNAT();
	int CheckMACReply();
	int CheckNATReply();
	int CheckINFReply();
	int CheckCHKReply();
	int Connect();
	int CheckCONReply();
	int CleanUp();
private:
	TcpClient m_Channel;
	char *m_buf;
};

#define MACOK "mac-ippassok"
#define NATOK ""
#define INFOK "passok"
#define CHKOK "0"
#define CONOK "ok"