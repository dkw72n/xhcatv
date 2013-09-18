#pragma once

#define FPS 20
extern bool g_ExitFlag;
extern bool g_Action;
extern bool g_Status;
extern char* g_Configfile;
extern void (*onStatusChange)(void); 

#define START true
#define STOP false
#define ONLINE true
#define OFFLINE false
#define SUCCESS_RET 0

#define CONFIGFILE "d:\\xhcatv.ini"