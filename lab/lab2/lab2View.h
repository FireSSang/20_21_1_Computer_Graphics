
// lab2View.h: Clab2View 类的接口
//

#pragma once


class Clab2View : public CView
{
protected: // 仅从序列化创建
	Clab2View() noexcept;
	DECLARE_DYNCREATE(Clab2View)

// 特性
public:
	Clab2Doc* GetDocument() const;

	CPoint start_point;//起始点
	CPoint end_point;//终止点
	CPoint first;//第一个点
	CPoint Point[100];
	int if_Press = 0;
	int Number = 0; //多边形的点数

	CPoint New_Point[100];

	int XL = 100, XR = 300, YB = 100, YT = 200;//分别是左边的x值，右边的x值，下边的y值，上边的y值
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
	virtual ~Clab2View();
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
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSutherHodgman();
};

#ifndef _DEBUG  // lab2View.cpp 中的调试版本
inline Clab2Doc* Clab2View::GetDocument() const
   { return reinterpret_cast<Clab2Doc*>(m_pDocument); }
#endif

