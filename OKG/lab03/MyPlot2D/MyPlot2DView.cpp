
// MyPlot2DView.cpp: реализация класса CMyPlot2DView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#include "MyPlot2D.h"


#include "MyPlot2DDoc.h"
#include "MyPlot2DView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyPlot2DView

IMPLEMENT_DYNCREATE(CMyPlot2DView, CView)

// добавил обработчики для f1 2 3
BEGIN_MESSAGE_MAP(CMyPlot2DView, CView)
	ON_COMMAND(ID_TESTS_F_F1, &CMyPlot2DView::OnTestsF_F1)
	ON_COMMAND(ID_TESTS_F_F2, &CMyPlot2DView::OnTestsF_F2)
	ON_COMMAND(ID_TESTS_F_F3, &CMyPlot2DView::OnTestsF_F3)
END_MESSAGE_MAP()

// Создание или уничтожение CMyPlot2DView

CMyPlot2DView::CMyPlot2DView() noexcept
{
}

CMyPlot2DView::~CMyPlot2DView()
{
}

BOOL CMyPlot2DView::PreCreateWindow(CREATESTRUCT& cs)
{

	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), nullptr);

	return TRUE;
}

// Рисование CMyPlot2DView

#pragma region математика


double CMyPlot2DView::MyF1(double x)
{
	if (x == 0) return 1;
    return sin(x) / x;
}

double CMyPlot2DView::MyF2(double x)
{
    return sqrt( fabs(x)) * sin(x);
}


#pragma endregion

void CMyPlot2DView::OnDraw(CDC* pDC)
{
	CRect rc; GetClientRect(&rc);
	pDC->FillSolidRect(rc, ::GetSysColor(COLOR_WINDOW)); // очистили фон

	if(m_ShowF3)
	{
		int w = rc.Width() - 2;
		int h = rc.Height() - 2 ;
		OnSize(1, w, h);
		GraphCircle.Draw(*pDC, 0, 0);
		GraphOct.Draw(*pDC, 0, 0);
		return;
	}
	
	Graph.Draw(*pDC, 1, 1);
}

// Обработчики сообщений CMyPlot2DView
void CMyPlot2DView::OnTestsF_F1() 
{
	m_ShowF3 = false;
	double xL = -3 * pi;
	double xH = -xL;
	double dx = pi / 36;
	int N = (xH - xL) / dx;
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);
	for (int i = 0; i <= N; i++)
	{
		X(i) = xL + i * dx;
		Y(i) = MyF1(X(i));
	}
	PenLine.Set(PS_SOLID, 1, RGB(255, 0, 0));
	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 255));
	RW.SetRect(500, 150, 900, 550);
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
	this->Invalidate();
}
void CMyPlot2DView::OnTestsF_F2() 
{
	m_ShowF3 = false;
	double xL = -4 * pi;
	double xH = -xL;
	double dx = pi / 36;
	int N = (xH - xL) / dx;
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);
	for (int i = 0; i <= N; i++)
	{
		X(i) = xL + i * dx;
		Y(i) = MyF2(X(i));
	}
	PenLine.Set(PS_DASHDOT, 1, RGB(255, 0, 0));
	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 0));
	RW.SetRect(500, 150, 900, 550);
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
	this->Invalidate();
}

void CMyPlot2DView::OnTestsF_F3()
{
	const double R = 10.0;

	const int N8 = 8;
	X8.RedimMatrix(N8 + 1);
	Y8.RedimMatrix(N8 + 1);
	for (int i = 0; i <= N8; ++i)
	{
		double t = (2.0 * pi * i) / N8;
		X8(i) = R * cos(t);
		Y8(i) = R * sin(t);
	}

	const int Nc = 180;
	Xc.RedimMatrix(Nc + 1);
	Yc.RedimMatrix(Nc + 1);
	for (int i = 0; i <= Nc; ++i)
	{
		double t = (2.0 * pi * i) / Nc;
		Xc(i) = R * cos(t);
		Yc(i) = R * sin(t);
	}

	PenOct.Set(PS_SOLID, 3, RGB(255, 0, 0));
	PenCircle.Set(PS_SOLID, 2, RGB(0, 0, 255));


	UpdatePlotRect();

	GraphOct.SetParams(X8, Y8, RW);
	GraphOct.SetPenLine(PenOct);
	
	GraphCircle.SetParams(Xc, Yc, RW);
	GraphCircle.SetPenLine(PenCircle);
	
	CRect rc; GetClientRect(&rc);
	const int cx = rc.Width() / 2;
	const int cy = rc.Height() / 2;

	m_ShowF3 = true;
	Invalidate(TRUE);
	UpdateWindow();
}


void CMyPlot2DView::UpdatePlotRect()
{
	CRect rc; GetClientRect(&rc);
	const int pad = 20;

	int w = rc.Width() - 2 * pad;
	int h = rc.Height() - 2 * pad;
	int s = max(10, min(w, h));            // сторона квадрата (не даем уйти < 10)

	int cx = rc.left + rc.Width() / 2;     // центр окна
	int cy = rc.top + rc.Height() / 2;

	// квадрат, центрированный по окну
	int left = cx - s / 2;
	int top = cy - s / 2;
	int right = left + s;
	int bottom = top + s;

	RW.SetRect(left, top, right, bottom);
}


void CMyPlot2DView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	if (!m_ShowF3) return;

	static CSize last;            // кэш размера
	CSize now(cx, cy);
	if (now == last) return;      // размер не менялся — ничего не делаем
	last = now;

	UpdatePlotRect();
	GraphOct.SetParams(X8, Y8, RW);
	GraphCircle.SetParams(Xc, Yc, RW);

	Invalidate(TRUE);             // разовая перерисовка
}
