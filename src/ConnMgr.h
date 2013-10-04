#pragma once
#include "ProtocolHelper.h"
#define BEAT_FEQ 5
#define STA_CON 2
#define STA_CON_OK 3
#define STA_MAC 4
#define STA_MAC_OK 5
#define STA_NAT 6
#define STA_NAT_OK 7
#define STA_INF 8
#define STA_INF_OK 9
#define STA_CHK 10
#define STA_CHK_OK 11
#define STA_FIN 12

class ConnMgr{
public:
	ConnMgr();
	int Init();
	int Start();
	int Stop();
	int Beat(unsigned cnt);
	int UnInit();
private:
	unsigned m_Lucky;
	unsigned m_status;
	void AskForHelp(_TCHAR *szMsg);
	ProtocolHelper *m_ProtocolHelper;
};


DWORD DoConnect();
DWORD DoDisconnect();