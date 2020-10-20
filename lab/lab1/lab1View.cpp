
// lab1View.cpp: Clab1View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "lab1.h"
#endif

#include "lab1Doc.h"
#include "lab1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Clab1View

IMPLEMENT_DYNCREATE(Clab1View, CView)

BEGIN_MESSAGE_MAP(Clab1View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Clab1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Clab1View 构造/析构

Clab1View::Clab1View() noexcept
{
	// TODO: 在此处添加构造代码

}

Clab1View::~Clab1View()
{
}

BOOL Clab1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Clab1View swap函数
void Clab1View::swap(int a, int b)
{
    int temp = a;
    a = b;
    b = temp;
}

// Clab1View DDA算法

void Clab1View::DDA_line(CDC* pDC, int x0, int y0, int x1, int y1, COLORREF color)
{
    double k;//斜率
    if (x1 != x0 && fabs(k = (y1 - y0) * 1.0 / (x1 - x0)) <= 1) //斜率<=1时
    {
        //交换
        if (x0 > x1)
        {
            swap(x0, x1);
            swap(y0, y1);
        }

        double y = y0;
        for (int x = x0; x <= x1; x++)
        {
            pDC->SetPixel(x, int(y + 0.5), color);//绘制像素点
            y += k;
        }
    }
    else //斜率>1时
    {
        k = (x1 - x0) * 1.0 / (y1 - y0);
        //交换
        if (y0 > y1)
        {
            swap(x0, x1);
            swap(y0, y1);
        }

        double x = x0;
        for (int y = y0; y <= y1; y++)
        {
            pDC->SetPixel(int(x + 0.5), y, color);
            x += k;
        }
    }
}

// Clab1View Bresenham算法

void Clab1View::Bre_line(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color)
{
    int dx, dy; // 横轴纵轴差量  
    int e;
    int x, y;
    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    y = y1;
    x = x1;
    int cx, cy; // 表明x、y方向的增量  
    if (x1 > x2)
        cx = -1; // x递减方向  
    else
        cx = 1; // x递增方向  
    if (y1 > y2)
        cy = -1; // y递减方向  
    else
        cy = 1; // y递增方向 
    if (dx == 0 && dy == 0)
    {
        printf("The input is not a line. It is just a point. Please input two different points.!\n");
    }
    else if (dy == 0)
    {
        for (x = x1; (cx == 1 ? x <= x2 : x >= x2); x += cx)
        {
            pDC->SetPixel(x, y, color);

        }
    }
    else if (dx == 0)
    {
        for (y = y1; (cy == 1 ? y <= y2 : y >= y2); y += cy)
        {
            pDC->SetPixel(x, y, color);
        }
    }
    else if (dx >= dy) {
        e = -dx;
        for (x = x1; (cx == 1 ? x <= x2 : x >= x2); x += cx)
        {
            pDC->SetPixel(x, y, color);
            e += dy << 1;
            if (e > 0)
            {
                y += cy;
                e -= dx << 1;
            }
        }
    }
    else {
        e = -dy;
        for (y = y1; (cy >= 0 ? y <= y2 : y >= y2); y += cy)
        {
            pDC->SetPixel(x, y, color);
            e += dx << 1;
            if (e > 0)
            {
                x += cx;
                e -= dy << 1;
            }
        }
    }
}

// Clab1grid 画个网格
void Clab1View::grid(CDC* pDC)
{
    for (int i = 10; i <= 1000; i += 10)
    {
        for (int j = 10; j <= 1000; j++)
        {
            pDC->SetPixel(i, j, RGB(0, 0, 0));
            pDC->SetPixel(j, i, RGB(0, 0, 0));
        }
    }
}


// Clab1View 绘图

void Clab1View::OnDraw(CDC* pDC)
{
    Clab1Doc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // TODO: 在此处为本机数据添加绘制代码
    grid(pDC);
    DDA_line(pDC, 100, 100, 678, 443, RGB(255, 0, 0));
    Bre_line(pDC, 100, 100, 678, 443, RGB(0, 255, 0));
}


// Clab1View 打印


void Clab1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Clab1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Clab1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Clab1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void Clab1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Clab1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Clab1View 诊断

#ifdef _DEBUG
void Clab1View::AssertValid() const
{
	CView::AssertValid();
}

void Clab1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Clab1Doc* Clab1View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Clab1Doc)));
	return (Clab1Doc*)m_pDocument;
}
#endif //_DEBUG


// Clab1View 消息处理程序
