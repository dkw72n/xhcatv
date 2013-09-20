#include "stdafx.h"
#include "main.h"
#include "commondef.h"
#include "ConnMgr.h"

#define TEMPLATE	"[注意:plan处填写套餐类型,对应官方客户端下拉菜单顺序的1,2,3,4]\r\n"\
					"[INFO]\r\n"\
					"用户名=yourname\r\n"\
					"密码=yourpassword\r\n"\
					"套餐=1/2/3/4\r\n"\
					"物理地址=00-00-00-00-00-00\r\n"\
					"[setting]\r\n"\
					"AutoConnect=0\r\n"

bool g_ExitFlag = false;
bool g_Action = STOP;
bool g_Status = OFFLINE;
extern char *g_szCfgPath;
unsigned __stdcall mainLoop(void* pArguments)
{
	ConnMgr ConnectionMgr;
	ConnectionMgr.Init();
	unsigned uFrameCount = 0;
	if(PathFileExistsA(g_szCfgPath)){
		int autoConnect = GetPrivateProfileIntA("setting", "AutoConnect", 0, g_szCfgPath);
		if(autoConnect)
			DoConnect();
	}
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
	TCHAR szPath[MAX_PATH];
	if(SUCCEEDED(SHGetFolderPath(NULL,
								CSIDL_APPDATA|CSIDL_FLAG_CREATE,
								NULL,
								0,
								szPath)))
	{
		PathAppend(szPath, _T("xhcatv.ini"));
		if(!PathFileExists(szPath)){
			FILE *fp = _tfopen(szPath, _T("w"));
			fwrite(TEMPLATE, sizeof(TEMPLATE), 1, fp);
			fclose(fp);
		}
		STARTUPINFO si = {sizeof(si)};
		PROCESS_INFORMATION pi;
		memset(&pi, sizeof(pi), 0);
		TCHAR cmdline[MAX_PATH * 2] = _T("c:\\windows\\system32\\notepad.exe ");
		_tcscat(cmdline, szPath);
		CreateProcess(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	}
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