
// MyPlot2D.h: основной файл заголовка для приложения MyPlot2D
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CMyPlot2DApp:
// Сведения о реализации этого класса: MyPlot2D.cpp
//

class CMyPlot2DApp : public CWinApp
{
public:
	CMyPlot2DApp() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMyPlot2DApp theApp;
