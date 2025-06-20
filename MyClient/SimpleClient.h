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
	HWND m_hParent;
	//SOCKET clientSocket;
	BOOL m_bThreadAlive, m_bThreadStateEnd;
	std::thread t1;

	BOOL CreateWndForm(DWORD dwStyle);

	void StringToChar(CString str, char* szStr);
	void StringToTChar(CString str, TCHAR* tszStr);
	CString CharToString(char *szStr);
	void ThreadStart();
	void ThreadStop();
	void ClearReadBuffer();

public:
	CSimpleClient(CString sServerIp, int nPort, CWnd* pParent = NULL);
	virtual ~CSimpleClient();
	
	char* m_pReadBuffer;
	SOCKET clientSocket;

	static void ProcThrd(const LPVOID lpContext);
	BOOL ProcReceive();
	BOOL Send(CString sSend);
	BOOL ThreadIsAlive();

protected:
	void ThreadEnd();

protected:
	DECLARE_MESSAGE_MAP()
};


