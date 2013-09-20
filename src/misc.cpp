#include "stdafx.h"
#include "misc.h"

#define TENSPACE "          "
#define MAC_PREFIX "mymac-address:"
#define INFO_PREFIX "myinformation:"
char *p1="INFOMATIONENCODED", *p2="MACADDRESS";

char* GetUsername()
{
	static char namebuff[11] = "test";
	if(PathFileExistsA(g_szCfgPath))
		GetPrivateProfileStringA("INFO", "用户名", namebuff, namebuff, 11, g_szCfgPath);
	namebuff[10]=0;
	return namebuff;
}

char * GetPassword()
{
	static char pswbuff[11] = "test";
	if(PathFileExistsA(g_szCfgPath))
		GetPrivateProfileStringA("INFO", "密码", pswbuff, pswbuff, 11, g_szCfgPath);
	pswbuff[10]=0;
	return pswbuff;
}

UINT GetPlan()
{
	return GetPrivateProfileIntA("INFO", "套餐", 0, g_szCfgPath);
}
char* GetMacAddress()
{
	static char macbuff[] = MAC_PREFIX"00-00-00-00-00-00";
	if(PathFileExistsA(g_szCfgPath))
		GetPrivateProfileStringA("INFO", "物理地址", "00-00-00-00-00-00", macbuff + strlen(MAC_PREFIX), sizeof("00-00-00-00-00-00"), g_szCfgPath);
	macbuff[strlen(macbuff)]=0;
	return macbuff;
}
int Encoder(char *from, char *to, int lenTo)
{
	char mp[] = "0123456789ABCDEF";
	int len = strlen(from), i, j;
	BYTE key = (BYTE)((len & 0xff) ^ 0xa5);
	if(!to)
		return -1;
	for(i=0,j=0; i<len && j+1 < lenTo; i++)
	{
		key = from[i] ^ key;
		to[j++] = mp[(key&0xf0)>>4];
		to[j++] = mp[key&0xf];
	}
	return 0;
}
char* GetInformationEncoded()
{
	char *user = GetUsername();
	char *psw = GetPassword();
	static char infobuff[] = INFO_PREFIX TENSPACE TENSPACE TENSPACE TENSPACE "  ";
	char mp[] = "01234";
	char *e_uid = infobuff + strlen(INFO_PREFIX);
	char *e_psw = e_uid + 20;
	char *plan = e_psw + 20;
	Encoder(user, e_uid, 20);
	Encoder(psw, e_psw, 20);
	plan[0]=mp[GetPlan()%5];
	return infobuff;
}
