#include "stdafx.h"
#include "ConnMgr.h"
#define MAXTRY 200
int ConnMgr::Beat(unsigned cnt)
{
	if(m_Lucky == -1)
		m_Lucky = cnt % (FPS * BEAT_FEQ);
	if(cnt % (FPS * BEAT_FEQ) != m_Lucky){
		return SUCCESS_RET;
	}
	return m_ProtocolHelper->SendBeat();
}

ConnMgr::ConnMgr()
{
	m_Lucky = -1;
	m_ProtocolHelper = 0;
	m_status = STA_CON;
}

int ConnMgr::Start()
{
	m_Lucky = -1;
	static int timeout = 0;
	//×Ô¶¯»ú
	int iStatus = -1;
	switch(m_status){
	case STA_CON:
		if(SUCCESS_RET == m_ProtocolHelper->Connect()){
			m_status = STA_CON_OK;
		}
		else{
			m_ProtocolHelper->CleanUp();
			m_status = STA_CON;
		}
		break;
	case STA_CON_OK:
		iStatus = m_ProtocolHelper->CheckCONReply();
		if(CHK_SUCCESS == iStatus){
			m_status = STA_MAC;
			timeout=-1;
		}
		else if(CHK_FAIL == iStatus || timeout > MAXTRY){
			m_ProtocolHelper->CleanUp();
			m_status = STA_CON;
			timeout=-1;
		}
		timeout++;
		break;
	case STA_MAC:
		iStatus = m_ProtocolHelper->SendMACAddress();
		if(SUCCESS_RET == iStatus){
			m_status = STA_MAC_OK;
		}
		else{
			m_ProtocolHelper->CleanUp();
			m_status = STA_CON;
		}
		break;
	case STA_MAC_OK:
		iStatus = m_ProtocolHelper->CheckMACReply();
		if(CHK_SUCCESS == iStatus){
			m_status = STA_NAT;
			timeout = -1;
		}
		else if(CHK_FAIL == iStatus || timeout > MAXTRY){
			m_ProtocolHelper->CleanUp();
			m_status = STA_CON;
			timeout = -1;
		}
		timeout++;
		break;
	case STA_NAT:
		iStatus = m_ProtocolHelper->SendNATVersion();
		if(SUCCESS_RET == iStatus){
			m_status = STA_NAT_OK;
		}
		else{
			m_ProtocolHelper->CleanUp();
			m_status = STA_CON;
		}
		break;
	case STA_NAT_OK:
		iStatus = m_ProtocolHelper->CheckNATReply();
		if(CHK_SUCCESS == iStatus){
			m_status = STA_INF;
			timeout = -1;
		}
		else if(CHK_FAIL == iStatus || timeout > MAXTRY){
			m_ProtocolHelper->CleanUp();
			m_status = STA_CON;
			timeout = -1;
		}
		timeout++;
		break;
	case STA_INF:
		iStatus = m_ProtocolHelper->SendInformation();
		if(SUCCESS_RET == iStatus){
			m_status = STA_INF_OK;
		}
		else{
			m_ProtocolHelper->CleanUp();
			m_status = STA_CON;
		}
		break;
	case STA_INF_OK:
		iStatus = m_ProtocolHelper->CheckINFReply();
		if(CHK_SUCCESS == iStatus){
			m_status = STA_CHK;
			timeout = -1;
		}
		else if(CHK_FAIL == iStatus || timeout > MAXTRY){
			m_ProtocolHelper->CleanUp();
			m_status = STA_CON;
			timeout = -1;
		}
		timeout++;
		break;
	case STA_CHK:
		iStatus = m_ProtocolHelper->SendCheckNAT();
		if(SUCCESS_RET == iStatus){
			m_status = STA_CHK_OK;
		}
		else{
			m_ProtocolHelper->CleanUp();
			m_status = STA_CON;
		}
		break;
	case STA_CHK_OK:
		iStatus = m_ProtocolHelper->CheckCHKReply();
		if(CHK_SUCCESS == iStatus){
			g_Status = ONLINE;
			onStatusChange();
			timeout = -1;
			m_status = STA_CON;
		}
		else if(CHK_FAIL == iStatus || timeout > MAXTRY){
			m_ProtocolHelper->CleanUp();
			timeout = -1;
			m_status = STA_CON;
		}
		timeout++;
		break;
	}
	return 0;
}

int ConnMgr::Stop()
{
	m_ProtocolHelper->CleanUp();
	g_Status = OFFLINE;
	onStatusChange();
	return 0;
}

int ConnMgr::Init()
{
	int ret = 0;
	m_ProtocolHelper = new ProtocolHelper;
	if(m_ProtocolHelper == NULL)
	{
		ret = 1;
	}
	else
	{
		m_ProtocolHelper->Init();
	}
	return ret;
}

int ConnMgr::UnInit()
{
	int ret = 0;
	if(m_ProtocolHelper != NULL)
	{
		m_ProtocolHelper->UnInit();
		delete m_ProtocolHelper;
		m_ProtocolHelper = NULL;
	}
	return ret;
}