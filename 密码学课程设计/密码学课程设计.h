
// ����ѧ�γ����.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// C����ѧ�γ����App:
// �йش����ʵ�֣������ ����ѧ�γ����.cpp
//

class C����ѧ�γ����App : public CWinApp
{
public:
	C����ѧ�γ����App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern C����ѧ�γ����App theApp;