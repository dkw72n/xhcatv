#include "stdafx.h"
#include "main.h"
#include "commondef.h"
#include "ConnMgr.h"

bool g_ExitFlag = false;
bool g_Action = STOP;
bool g_Status = OFFLINE;
unsigned __stdcall mainLoop(void* pArguments)
{
	ConnMgr ConnectionMgr;
	ConnectionMgr.Init();
	unsigned uFrameCount = 0;
	while(!g_ExitFlag)
	{
		if(g_Action == START && g_Status == ONLINE)
		{
			if(SUCCESS_RET != ConnectionMgr.Beat(uFrameCount)){
				ConnectionMgr.Stop();
			}
		}
		else if(g_Action == START && g_Status == OFFLINE)
		{
			ConnectionMgr.Start();
		}
		else if(g_Action == STOP && g_Status == ONLINE)
		{
			ConnectionMgr.Stop();
		}
		else if(g_Action == STOP && g_Status == OFFLINE)
		{
			;
		}
		Sleep(1000/FPS);
		uFrameCount++;
	}
	ConnectionMgr.UnInit();
	_endthreadex( 0 );
	return 0;
}
DWORD DoSetting()
{
	STARTUPINFO si = {sizeof(si)};
	PROCESS_INFORMATION pi;
	memset(&pi, sizeof(pi), 0);
	TCHAR cmdline[] = _T("c:\\windows\\system32\\notepad.exe ")_T(CONFIGFILE);
	CreateProcess(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
/*	if(0)
	{
		MessageBox(NULL, _T("OK"), _T("DONE"),MB_OK);
	}
	else
	{
		DWORD s = GetLastError();
		MessageBox(NULL, _T("NO OK"), _T("DONE"),MB_OK);
	}*/
	return 0;
}
DWORD DoConnect()
{
	//MessageBox(NULL, _T("connect"), _T(""), MB_OK);
	g_Action = START;
	onStatusChange();
	return 0;
}
DWORD DoDisconnect()
{
	//MessageBox(NULL, _T("disconnect"), _T(""), MB_OK);
	g_Action = STOP;
	onStatusChange();
	return 0;
}