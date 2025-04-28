// SimpleClient.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyClient.h"
#include "SimpleClient.h"

// CSimpleClient

IMPLEMENT_DYNAMIC(CSimpleClient, CWnd)

CSimpleClient::CSimpleClient(CString sServerIp, int nPort, CWnd* pParent/*=NULL*/)
{
	m_pParent = pParent;
	if(pParent)
		m_hParentWnd = pParent->GetSafeHwnd();

	m_pReadBuffer = new char[BUFSIZE]; // 1mb

	//윈도우 소켓(원속) 초기화
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		AfxMessageBox(_T("실패 - 윈도우 소켓(원속) 초기화"));
		return;
	}

	//소켓 확인(connect)
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		AfxMessageBox(_T("socket() Error"));
		return;
	}

	//연결 확인
	int nLen = sServerIp.GetLength() + 1; // for '\0'
	char* cServerIp = new char[nLen];
	StringToChar(sServerIp, cServerIp);

	int recval;
	SOCKADDR_IN clientAddr;
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(nPort);
	clientAddr.sin_addr.S_un.S_addr = inet_addr(cServerIp);
	recval = connect(clientSocket, (SOCKADDR *)&clientAddr, sizeof(clientAddr));
	if (recval == SOCKET_ERROR)
		AfxMessageBox(_T("connect() Error"));
	delete cServerIp;
	
	StartThread();
}

CSimpleClient::~CSimpleClient()
{
	StopThread();
	Sleep(30);
	t1.join();

	//소켓 종료
	closesocket(clientSocket);
	WSACleanup();

	delete m_pReadBuffer;
}


BEGIN_MESSAGE_MAP(CSimpleClient, CWnd)
END_MESSAGE_MAP()



// CSimpleClient 메시지 처리기입니다.

BOOL CSimpleClient::Send(CString sSend)
{
	int nLen = sSend.GetLength() + 1; // for '\0'
	char* cSend = new char[nLen];
	StringToChar(sSend, cSend);
	int recval = send(clientSocket, cSend, strlen(cSend), 0);
	if (recval == SOCKET_ERROR)
	{
		AfxMessageBox(_T("send() Error"));
		return FALSE;
	}
	delete cSend;
	return TRUE;
}

void CSimpleClient::funcReceive(const LPVOID lpContext)
{
	CSimpleClient* pSimpleClient = reinterpret_cast<CSimpleClient*>(lpContext);

	while (pSimpleClient->IsAliveThread())
	{
		if (!pSimpleClient->Receive())
			break;
	}

	pSimpleClient->EndThread();
}

BOOL CSimpleClient::Receive()
{
	ClearReadBuffer();

	char* readBuffer = m_pReadBuffer;
	fd_set	fdRead = { 0 };
	TIMEVAL	*pstTime = NULL;
	TIMEVAL	stTime;
	DWORD dwTimeout = 0; // 0 second
	int nRecSize = -1;

	if (INFINITE != dwTimeout)
	{
		stTime.tv_sec = dwTimeout / 1000;
		stTime.tv_usec = (dwTimeout * 1000) % 1000000;
		pstTime = &stTime;
	}

	// Set Descriptor
	if (!FD_ISSET(clientSocket, &fdRead))
		FD_SET(clientSocket, &fdRead);

	// Select function set read timeout
	DWORD dwBytesRead = 0L;
	int res = select(clientSocket + 1, &fdRead, NULL, NULL, pstTime);
	if (res > 0)
	{
		char buffer[BUFSIZE] = { 0, };

		nRecSize = recv(clientSocket, buffer, BUFSIZE, 0);
		if (nRecSize > 0)
		{
			memcpy(readBuffer, buffer, nRecSize);
			readBuffer[nRecSize] = _T('\0');
			CString sMsg = CharToString(readBuffer);
			::SendMessage(m_hParentWnd, WM_CLIENT_RECEIVED, (WPARAM)0, (LPARAM)(LPCTSTR)sMsg);
		}
		else if (nRecSize == 0)
		{
			return FALSE;
		}
	}

	return TRUE;
}

void CSimpleClient::StartThread()
{
	m_bEndThread = FALSE;
	m_bAliveThread = TRUE;
	t1 = std::thread(funcReceive, this);
}

void CSimpleClient::StopThread()
{
	m_bAliveThread = FALSE;
	MSG message;
	const DWORD dwTimeOut = 1000*60*3; // 3 Minute
	DWORD dwStartTick = GetTickCount();
	Sleep(30);
	while (!m_bEndThread)
	{
		if (GetTickCount() >= (dwStartTick + dwTimeOut))
		{
			AfxMessageBox(_T("WaitUntilThreadEnd() Time Out!!!", NULL, MB_OK | MB_ICONSTOP));
			return;
		}
		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
		Sleep(30);
	}
}

void CSimpleClient::EndThread()
{
	m_bEndThread = TRUE;
}

SOCKET& CSimpleClient::GetSocket()
{
	return clientSocket;
}

BOOL CSimpleClient::IsAliveThread()
{
	return m_bAliveThread;
}

void EndThread()
{

}

void CSimpleClient::ClearReadBuffer()
{
	ZeroMemory(m_pReadBuffer, sizeof(m_pReadBuffer));
}

void CSimpleClient::StringToChar(CString str, char* szStr)  // char* returned must be deleted... 
{
	int nLen = str.GetLength();
	strcpy(szStr, CT2A(str));
	szStr[nLen] = _T('\0');
}

void CSimpleClient::StringToTChar(CString str, TCHAR* tszStr) // TCHAR* returned must be deleted... 
{
	int nLen = str.GetLength() + 1;
	memset(tszStr, 0x00, nLen * sizeof(TCHAR));
	_tcscpy(tszStr, str);
}

CString CSimpleClient::CharToString(char *szStr)
{
	CString strRet;

	int nLen = strlen(szStr) + sizeof(char);
	wchar_t *tszTemp = NULL;
	tszTemp = new WCHAR[nLen];

	MultiByteToWideChar(CP_ACP, 0, szStr, -1, tszTemp, nLen * sizeof(WCHAR));

	strRet.Format(_T("%s"), (CString)tszTemp);
	if (tszTemp)
	{
		delete[] tszTemp;
		tszTemp = NULL;
	}
	return strRet;
}


