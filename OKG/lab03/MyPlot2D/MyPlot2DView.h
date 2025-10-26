
// MyPlot2DView.h: интерфейс класса CMyPlot2DView
//

#pragma once

class CMyPlot2DView : public CView
{
protected: // создать только из сериализации
	CMyPlot2DView() noexcept;
	DECLARE_DYNCREATE(CMyPlot2DView)
	bool m_drawOctagon{ false };

// Атрибуты
public:
	int Index;
	int N_Bezier;
	CMatrix X, Y;
	CRect RW;	// Прямоугольник в окне
	CRectD RS;	// Прямоугольник области в МСК
	CPlot2D Graph;	// график
	CMyPen PenLine, // Перо для линий
		PenAxis;	// Перо для осей

	CMatrix X8, Y8, Xc, Yc;
	CPlot2D GraphOct, GraphCircle;
	CMyPen PenOct, PenCircle;
	bool m_ShowF3 = false;

// Операции
public:

#pragma region мой код

	double MyF1(double x);
	double MyF2(double x);

#pragma endregion

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Реализация
public:
	virtual ~CMyPlot2DView();

protected:

// Созданные функции схемы сообщений
protected:

#pragma region мой код

	// обработчики меню
	void OnTestsF_F1();
	void OnTestsF_F2();
	void OnTestsF_F3();
	void UpdatePlotRect();
	void OnSize(UINT nType, int cx, int cy);

#pragma endregion

	DECLARE_MESSAGE_MAP()
};