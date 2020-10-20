
// lab1View.h: Clab1View 类的接口
//

#pragma once


class Clab1View : public CView
{
protected: // 仅从序列化创建
	Clab1View() noexcept;
	DECLARE_DYNCREATE(Clab1View)

// 特性
public:
	Clab1Doc* GetDocument() const;
	void swap(int a, int b);
	void DDA_line(CDC* pDC, int x0, int y0, int x1, int y1, COLORREF color);
	void Bre_line(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color);
	void grid(CDC* pDC);

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~Clab1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // lab1View.cpp 中的调试版本
inline Clab1Doc* Clab1View::GetDocument() const
   { return reinterpret_cast<Clab1Doc*>(m_pDocument); }
#endif

