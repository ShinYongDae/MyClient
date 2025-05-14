#pragma once

#include <thread>
#include <WinSock2.h>

#define WM_CLIENT_RECEIVED	(WM_USER + 0x73FF)	// 0x7FFF (Range of WM_USER is 0x400 ~ 0x7FFF)

#define BUFSIZE 16384

// CSimpleClient

class CSimpleClient : public CWnd
{
	DECLARE_DYNAMIC(CSimpleClient)

	CWnd* m_pParent;
	//SOCKET clientSocket;
	BOOL m_bAliveThread, m_bEndThreadState;
	std::thread t1;

	void StringToChar(CString str, char* szStr);
	void StringToTChar(CString str, TCHAR* tszStr);
	CString CharToString(char *szStr);
	void StartThread();
	void StopThread();
	void ClearReadBuffer();

public:
	CSimpleClient(CString sServerIp, int nPort, CWnd* pParent = NULL);
	virtual ~CSimpleClient();
	
	HWND m_hParentWnd;
	char* m_pReadBuffer;
	SOCKET clientSocket;

	static void thrdReceive(const LPVOID lpContext);
	BOOL Receive();
	BOOL Send(CString sSend);
	BOOL IsAliveThread();

protected:
	void EndThread();

protected:
	DECLARE_MESSAGE_MAP()
};


