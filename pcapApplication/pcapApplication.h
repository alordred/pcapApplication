
// pcapApplication.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CpcapApplicationApp:
// �йش����ʵ�֣������ pcapApplication.cpp
//

class CpcapApplicationApp : public CWinApp
{
public:
	CpcapApplicationApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CpcapApplicationApp theApp;