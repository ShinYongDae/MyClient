
// MyClientDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MyClient.h"
#include "MyClientDlg.h"
#include "afxdialogex.h"

#include "SimpleClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyClientDlg ��ȭ ����



CMyClientDlg::CMyClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MYCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pClient = NULL;
}

CMyClientDlg::~CMyClientDlg()
{
	if (m_pClient)
	{
		delete m_pClient;
		m_pClient = NULL;
	}
}

void CMyClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE_LIST, m_editMsgList);
}

BEGIN_MESSAGE_MAP(CMyClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CMyClientDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_SEND, &CMyClientDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_STOP, &CMyClientDlg::OnBnClickedBtnStop)
	ON_MESSAGE(WM_CLIENT_RECEIVED, wmClientReceived)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CMyClientDlg::OnBnClickedBtnClear)
END_MESSAGE_MAP()


// CMyClientDlg �޽��� ó����

BOOL CMyClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CString sPortCli = _T("12345"), sPortSvr = _T("12345"), sAddrCli = _T("127.0.0.2"), sAddrSvr = _T("127.0.0.1");

	GetDlgItem(IDC_IPADDRESS_CLI)->SetWindowText(sAddrCli);
	GetDlgItem(IDC_SVR_PORT)->SetWindowText(sPortSvr);
	GetDlgItem(IDC_IPADDRESS_SVR)->SetWindowText(sAddrSvr);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CMyClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMyClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CMyClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



LRESULT CMyClientDlg::wmClientReceived(WPARAM wParam, LPARAM lParam)
{
	CString sReceived = (LPCTSTR)lParam;

	CString sDisp;
	GetDlgItem(IDC_MESSAGE_LIST)->GetWindowText(sDisp);
	sDisp += sReceived + _T("\r\n");
	GetDlgItem(IDC_MESSAGE_LIST)->SetWindowText(sDisp);

	return (LRESULT)1;
}

void CMyClientDlg::OnBnClickedBtnStart()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPortCli, sPortSvr, sAddrCli, sAddrSvr;
	GetDlgItem(IDC_IPADDRESS_CLI)->GetWindowText(sAddrCli);
	GetDlgItem(IDC_SVR_PORT)->GetWindowText(sPortSvr);
	GetDlgItem(IDC_IPADDRESS_SVR)->GetWindowText(sAddrSvr);

	if(!m_pClient)
		m_pClient = new CSimpleClient(sAddrSvr, _tstoi(sPortSvr), this);
}


void CMyClientDlg::OnBnClickedBtnSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sMsg;
	GetDlgItem(IDC_TXT_MESSAGE)->GetWindowText(sMsg);
	if (!sMsg.IsEmpty())
	{
		if (m_pClient)
		{
			m_pClient->Send(sMsg);
		}
	}
}


void CMyClientDlg::OnBnClickedBtnStop()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_pClient)
	{
		delete m_pClient;
		m_pClient = NULL;
	}
}


void CMyClientDlg::OnBnClickedBtnClear()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (::IsWindow(m_editMsgList.GetSafeHwnd()))
	{
		int nLen = m_editMsgList.GetWindowTextLength();
		m_editMsgList.SetSel(0, -1);
		m_editMsgList.Clear();
	}
}
