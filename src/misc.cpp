#include "stdafx.h"
#include "misc.h"
#define TENSPACE "          "
char *p1="INFOMATIONENCODED", *p2="MACADDRESS";

char* GetUsername()
{
	static char namebuff[11] = "test";
	GetPrivateProfileStringA("INFO", "USER", namebuff, namebuff, 11, CONFIGFILE);
	namebuff[10]=0;
	return namebuff;
}

char * GetPassword()
{
	static char pswbuff[11] = "test";
	GetPrivateProfileStringA("INFO", "PASSWORD", pswbuff, pswbuff, 11, CONFIGFILE);
	pswbuff[10]=0;
	return pswbuff;
}

UINT GetPlan()
{
	return GetPrivateProfileIntA("INFO", "PLAN", 0, CONFIGFILE);
}
char* GetMacAddress()
{
	static char macbuff[] = "00-00-00-00-00-00";
	GetPrivateProfileStringA("INFO", "MAC", macbuff, macbuff, sizeof(macbuff), CONFIGFILE);
	macbuff[strlen(macbuff)]=0;
	return macbuff;
}
int Encoder(char *from, char *to, int lenTo)
{
	char mp[] = "0123456789ABCDEF";
	int len = strlen(from), i, j;
	BYTE key = (BYTE)((len & 0xff) ^ 0x05);
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
	static char infobuff[] = "myinfomation:" TENSPACE TENSPACE TENSPACE TENSPACE "  ";
	char mp[] = "01234";
	char *e_uid = infobuff + strlen("myinfomation:");
	char *e_psw = e_uid + 20;
	char *plan = e_psw + 20;
	Encoder(user, e_uid, 20);
	Encoder(psw, e_psw, 20);
	plan[0]=mp[GetPlan()%5];
	return infobuff;
}
