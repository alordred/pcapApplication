
// pcapApplicationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "pcapApplication.h"
#include "pcapApplicationDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CpcapApplicationDlg �Ի���



CpcapApplicationDlg::CpcapApplicationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CpcapApplicationDlg::IDD, pParent)
	, pcapFile(_T(""))
	, path(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CpcapApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, pcapFile);
	DDX_Text(pDX, IDC_EDIT2, path);
}

BEGIN_MESSAGE_MAP(CpcapApplicationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CpcapApplicationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CpcapApplicationDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT1, &CpcapApplicationDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CpcapApplicationDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CpcapApplicationDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT2, &CpcapApplicationDlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CpcapApplicationDlg::OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_BUTTON3, &CpcapApplicationDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CpcapApplicationDlg ��Ϣ�������

BOOL CpcapApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CpcapApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CpcapApplicationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CpcapApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CpcapApplicationDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();

	GetDlgItem(IDC_EDIT1)->GetWindowText(pcapFile);
	GetDlgItem(IDC_EDIT2)->GetWindowText(path);

	//CString convert to string
	USES_CONVERSION;
	std::string pcapFileStr (W2A(pcapFile));
	std::string pathStr (W2A(path)); 

	
	if (pcap.start(pathStr, pcapFileStr) == 0)
	{
		MessageBox(_T("��������,Succeeded!"), _T("callBack")); 
		//GetDlgItem(IDC_EDIT3)->SetWindowTextW(L"Succeeded!");
	}
	else
	{
		MessageBox(_T("��������,Failed!"), _T("callBack")); 
		//GetDlgItem(IDC_EDIT3)->SetWindowTextW(L"Failed!");
	}
}


void CpcapApplicationDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CpcapApplicationDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�


	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CpcapApplicationDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

    // ���ù�����   
    TCHAR szFilter[] = _T("�ı��ļ�(*.pcap)|*.pcap|�����ļ�(*.*)|*.*||"); 

    // ������ļ��Ի���   
    CFileDialog fileDlg(TRUE, _T("pcap"), NULL, 0, szFilter, this);   
    CString strFilePath;   
  
    // ��ʾ���ļ��Ի���   
    if (IDOK == fileDlg.DoModal())   
    {   
        // ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����   
        strFilePath = fileDlg.GetPathName();   
        SetDlgItemText(IDC_EDIT1, strFilePath);   
    }
}


void CpcapApplicationDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	WCHAR szPath[MAX_PATH];     //���ѡ���Ŀ¼·�� 
    CString str;

    ZeroMemory(szPath, sizeof(szPath));   

    BROWSEINFO bi;   
    bi.hwndOwner = m_hWnd;   
    bi.pidlRoot = NULL;   
    bi.pszDisplayName = szPath;   
    bi.lpszTitle = L"��ѡ����Ҫ�����Ŀ¼��";   
    bi.ulFlags = 0;   
    bi.lpfn = NULL;   
    bi.lParam = 0;   
    bi.iImage = 0;   
    //����ѡ��Ŀ¼�Ի���
    LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

    if(lp && SHGetPathFromIDList(lp, szPath))
	{
		GetDlgItem(IDC_EDIT2)->SetWindowText(szPath);
    }
}


void CpcapApplicationDlg::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�


	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CpcapApplicationDlg::OnEnChangeEdit3()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�


	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CpcapApplicationDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();

	GetDlgItem(IDC_EDIT1)->GetWindowText(pcapFile);
	GetDlgItem(IDC_EDIT2)->GetWindowText(path);

	//CString convert to string
	USES_CONVERSION;
	std::string pcapFileStr (W2A(pcapFile));
	std::string pathStr (W2A(path)); 

	
	if (sip.start(pathStr, pcapFileStr) == 0)
	{
		MessageBox(_T("��������,Succeeded!"), _T("callBack")); 
		//GetDlgItem(IDC_EDIT3)->SetWindowTextW(L"Succeeded!");
	}
	else
	{
		MessageBox(_T("��������,Failed!"), _T("callBack")); 
		//GetDlgItem(IDC_EDIT3)->SetWindowTextW(L"Failed!");
	}
}
