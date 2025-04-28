
// MyClientDlg.h : ��� ����
//

#pragma once

#include "SimpleClient.h"
#include "afxwin.h"

// CMyClientDlg ��ȭ ����
class CMyClientDlg : public CDialogEx
{
	CSimpleClient *m_pClient;

// �����Դϴ�.
public:
	CMyClientDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	~CMyClientDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT wmClientReceived(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnClear();
	CEdit m_editMsgList;
};
