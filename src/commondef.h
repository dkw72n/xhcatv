#pragma once

#define FPS 5
extern bool g_ExitFlag;
extern bool g_Action;
extern bool g_Status;
extern bool g_SetRoute;
extern char* g_Configfile;
extern void (*onStatusChange)(void); 

#define START true
#define STOP false
#define ONLINE true
#define OFFLINE false
#define SUCCESS_RET 0
