
// MInterpreter.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMInterpreterApp: 
// �йش����ʵ�֣������ MInterpreter.cpp
//

class CMInterpreterApp : public CWinApp
{
public:
	CMInterpreterApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMInterpreterApp theApp;