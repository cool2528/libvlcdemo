
// mfcvlc.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CmfcvlcApp: 
// �йش����ʵ�֣������ mfcvlc.cpp
//

class CmfcvlcApp : public CWinApp
{
public:
	CmfcvlcApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CmfcvlcApp theApp;