#include "stdafx.h"
#include "RouteSetter.h"
#include <Iphlpapi.h>
#include "misc.h"
#pragma comment(lib, "Iphlpapi.lib")
bool IsAlreadySet()
{
	// Declare and initialize variables
	bool ret = false;
    PMIB_IPFORWARDTABLE pIpForwardTable = NULL;
    DWORD dwSize = 0;
    BOOL bOrder = FALSE;
    DWORD dwStatus = 0;
   
    unsigned int i;

	// Find out how big our buffer needs to be.
    dwStatus = GetIpForwardTable(pIpForwardTable, &dwSize, bOrder);
    if (dwStatus == ERROR_INSUFFICIENT_BUFFER) {
        // Allocate the memory for the table
        if (!(pIpForwardTable = (PMIB_IPFORWARDTABLE) malloc(dwSize))) {
            printf("Malloc failed. Out of memory.\n");
            exit(1);
        }
        // Now get the table.
        dwStatus = GetIpForwardTable(pIpForwardTable, &dwSize, bOrder);
    }

    if (dwStatus != ERROR_SUCCESS) {
        printf("getIpForwardTable failed.\n");
        if (pIpForwardTable)
            free(pIpForwardTable);
        exit(1);
    }
    // Search for the row in the table we want.
    for (i = 0; i < pIpForwardTable->dwNumEntries; i++) {
        if (pIpForwardTable->table[i].dwForwardDest == 0x0a) {
			ret = true;
			break;
        }
    }

    if (dwStatus == NO_ERROR)
        printf("Gateway changed successfully\n");
    else if (dwStatus == ERROR_INVALID_PARAMETER)
        printf("Invalid parameter.\n");
    else
        printf("Error: %d\n", dwStatus);

    // Free resources
    if (pIpForwardTable)
        free(pIpForwardTable);
	return ret;
}

void Setit()
{
	    // Declare and initialize variables

    PMIB_IPFORWARDTABLE pIpForwardTable = NULL;
    PMIB_IPFORWARDROW pRow = NULL;
    DWORD dwSize = 0;
    BOOL bOrder = FALSE;
    DWORD dwStatus = 0;
    DWORD NewDest = 0x0000000A;  // this is in host order Ip Address AA.BB.CC.DD is DDCCBBAA
    
    unsigned int i;

// Find out how big our buffer needs to be.
    dwStatus = GetIpForwardTable(pIpForwardTable, &dwSize, bOrder);
    if (dwStatus == ERROR_INSUFFICIENT_BUFFER) {
        // Allocate the memory for the table
        if (!(pIpForwardTable = (PMIB_IPFORWARDTABLE) malloc(dwSize))) {
            printf("Malloc failed. Out of memory.\n");
            exit(1);
        }
        // Now get the table.
        dwStatus = GetIpForwardTable(pIpForwardTable, &dwSize, bOrder);
    }

    if (dwStatus != ERROR_SUCCESS) {
        printf("getIpForwardTable failed.\n");
        if (pIpForwardTable)
            free(pIpForwardTable);
        exit(1);
    }
    // Search for the row in the table we want. The default gateway has a destination
    // of 0.0.0.0. Notice that we continue looking through the table, but copy only 
    // one row. This is so that if there happen to be multiple default gateways, we can
    // be sure to delete them all.
    for (i = 0; i < pIpForwardTable->dwNumEntries; i++) {
        if (pIpForwardTable->table[i].dwForwardDest == 0) {
            // We have found the default gateway.
            if (!pRow) {
                // Allocate some memory to store the row in; this is easier than filling
                // in the row structure ourselves, and we can be sure we change only the
                // gateway address.
                pRow = (PMIB_IPFORWARDROW) malloc(sizeof (MIB_IPFORWARDROW));
                if (!pRow) {
                    printf("Malloc failed. Out of memory.\n");
                    exit(1);
                }
                // Copy the row
                memcpy(pRow, &(pIpForwardTable->table[i]),
                       sizeof (MIB_IPFORWARDROW));
            }

        }
    }

    // Set the nexthop field to our new gateway - all the other properties of the route will
    // be the same as they were previously.
	pRow->dwForwardDest = NewDest;
	pRow->dwForwardMask = 0x000000ff;

    // Create a new route entry for the default gateway.
    dwStatus = CreateIpForwardEntry(pRow);

    if (dwStatus == NO_ERROR)
        printf("Gateway changed successfully\n");
    else if (dwStatus == ERROR_INVALID_PARAMETER)
        printf("Invalid parameter.\n");
	else if (dwStatus == ERROR_ACCESS_DENIED)
		ErrAlert(_T("设置路由表失败. 你可能没有权限执行此操作, 如果你不需要使用VPN或者网游加速器, 这并不影响使用; 如需使用,请退出并以\"管理员权限\"运行."));
	else
        printf("Error: %d\n", dwStatus);

    // Free resources
    if (pIpForwardTable)
        free(pIpForwardTable);
    if (pRow)
        free(pRow);

}
void SetRouteTable()
{
	if(g_SetRoute && !IsAlreadySet())
		Setit();
}

