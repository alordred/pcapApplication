
// pcapApplicationDlg.h : ͷ�ļ�
//

#pragma once
#include "pcapToSip.h"


// CpcapApplicationDlg �Ի���
class CpcapApplicationDlg : public CDialogEx
{
// ����
public:
	CpcapApplicationDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PCAPAPPLICATION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	PCAP pcap;
	SIP sip;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CString pcapFile;
	CString path;
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickedButton3();
	//afx_msg void OnBnClickedButton1();
};
