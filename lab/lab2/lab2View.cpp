
// lab2View.cpp: Clab2View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "lab2.h"
#endif

#include "lab2Doc.h"
#include "lab2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Clab2View

IMPLEMENT_DYNCREATE(Clab2View, CView)

BEGIN_MESSAGE_MAP(Clab2View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Clab2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_32780, &Clab2View::OnSutherHodgman)
END_MESSAGE_MAP()

// Clab2View 构造/析构

Clab2View::Clab2View() noexcept
{
	// TODO: 在此处添加构造代码

}

Clab2View::~Clab2View()
{
}

BOOL Clab2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Clab2View 绘图

void Clab2View::OnDraw(CDC* pDC)
{
	Clab2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	CPen newpen(PS_SOLID, 3, RGB(0, 255, 0));
	CPen* old = pDC->SelectObject(&newpen);
	pDC->MoveTo(100, 100);
	pDC->LineTo(100, 200);

	pDC->MoveTo(100, 200);
	pDC->LineTo(300, 200);

	pDC->MoveTo(300, 200);
	pDC->LineTo(300, 100);

	pDC->MoveTo(300, 100);
	pDC->LineTo(100, 100);
	pDC->SelectObject(old);
	//ReleaseDC(pDC);
}


// Clab2View 打印


void Clab2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Clab2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Clab2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Clab2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void Clab2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Clab2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Clab2View 诊断

#ifdef _DEBUG
void Clab2View::AssertValid() const
{
	CView::AssertValid();
}

void Clab2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Clab2Doc* Clab2View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Clab2Doc)));
	return (Clab2Doc*)m_pDocument;
}
#endif //_DEBUG


// Clab2View 消息处理程序


void Clab2View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC* pDC = this->GetWindowDC();
	if (if_Press)
	{
		pDC->SetROP2(R2_NOT);  //添加橡皮绳（只要没左击，鼠标在桌面上移动，线条就会跟到哪去）
		pDC->MoveTo(start_point); //将点坐标移到上一个点击的点的位置
		pDC->LineTo(end_point);  //没有画出线，此时的start_point = end_point
		if ((point.x - first.x) < 0.1 && (point.y - first.y) < 0.1)//当鼠标指着的点离第一个点的距离小于一个值的时候，两点重合（这里两个点会自动重合，不用再次点击左键）
		{
			point = first;
			if_Press = 0;
		}
		pDC->MoveTo(start_point); //画出直线
		pDC->LineTo(point);
		end_point = point;
	}
	CView::OnMouseMove(nFlags, point);
}


void Clab2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (if_Press == 1 || Number == 0)
	{
		if_Press = 1; //说明已经点过了一个点
		start_point = point;
		end_point = start_point;
		Point[Number] = start_point;//把这个点存入数组里
		if (Number == 0) //表示第一次左击的点为第一个点，
			first = start_point;//把第一个点保存下来
		Number++;//多边形的点数加一
	}
	CView::OnLButtonDown(nFlags, point);
}



void Clab2View::OnSutherHodgman()
{
	// TODO: 在此添加命令处理程序代码
    {
        // TODO: 在此添加命令处理程序代码
        int temp_num; //这个临时的变量存储的就是多边形的点个数，也就是前面画多边形时的Number
        int new_num = 0; //这个变量是用来存储通过算法裁剪后的点的个数
        CPoint temp_point; //定义一个临时点，在后面是用来求交点用的
        int i;
        int j;
        temp_num = Number;

        //先说明下，画线的时候是从起始点到终止点，在后面的解释中我会把起始点说成头点，终止点说成尾点，也就是从头到尾，还有一点，如果这个点在可视区外，在后面就直接简称为外了
          /////////////
          //对矩形框上边处理
        for (i = 0; i < temp_num; i++)//temp_num 为多边形的边数，执行次数也就是边数
        {
            if (Point[i % temp_num].y > YT) //如果头点在上边之上，即头点在不可视区
            {    //这样要注意下Point数组选取元素是加的%号的含义，因为头号是i，尾号是i+1，可是到了最后一个点的时候，头号还是i，可是尾却不是i+1了，此时的头要和最初的第一个点相连，才能形成一个闭合的多边形，所以整除（点的个数），刚好能让最后一个的值变成0
            //本来%只是最后一个点才用得着，这里为了不乱掉，索性全加上，反正对前面的点也没什么影响。。。
                if (Point[(i + 1) % temp_num].y > YT) //尾点也在上边之上，即两点都在不可视区，均不进入新数组，这种情况就是（外到外），所以什么都不用写.
                {

                }
                else  //尾点在上边之下，即尾点在可视区，尾点和与上边的交点进入新数组（外到内）
                {

                    temp_point.x = Point[i % temp_num].x - (Point[i % temp_num].y - YT) * (Point[i % temp_num].x - Point[(i + 1) % temp_num].x)
                        / (Point[i % temp_num].y - Point[(i + 1) % temp_num].y);  //这一大串求交点的x坐标值是通过相似的原理来表示的，想深究的朋友可以画个实例图研究，接下来的很多求交点都是通过这个原理
                    temp_point.y = YT; //求交点的y坐标，因为交点在上边，所以上边的y值就是交点的y值
                    New_Point[new_num] = temp_point; //把这个临时的交点存到New_point数组里，同时点的个数加一，接下来同样每送到新数组一个点，new_num（新多边形的点个数)就加一
                    new_num += 1;
                    /*New_Point[new_num] = Point[(i + 1) % temp_num]; //这里要注意下，按理来说，画一条从外到内的线应该要把与边的交点和内点都要进入新数组里，但是内点却不在这段代码中加到新数组里面，而是到下一个循环，这个内点从尾点变成头点的时候加到数组里
                    new_num += 1;*/                                   //如果在这里加到数组里会导致这样一种情况，这个点作为尾点加到数组，到下一个循环，这个点变成了头点，难道又要再加一遍？这会导致同一个点在数组内存在了两次
                    //凡是都有例外，其中有一种情况是要在一段代码中添加两个点的，接下来会解释到

                }
            }
            else  //头点在上边之下，即在可视区
            {
                if (Point[(i + 1) % temp_num].y <= YT) //尾点也在可视区，头尾两点均入新数组（内到内）
                {
                    New_Point[new_num] = Point[i % temp_num];
                    new_num += 1;
                    /*New_Point[new_num] = Point[(i + 1) % temp_num];
                    new_num += 1;*/
                }
                else //尾点在不可视区，所以头点和交点进入新数组（内到外），这种情况就是上面说到的要添加两个点，大家可以拿笔画个草图看看，内到外，肯定先把内点加到新数组里，如果说把那个交点留着下一个循环来处理会导致什么样的情况，下一次肯定是从外到内
                {    //或者从外到外，不管哪种情况，能确定的是和这个交点已经没什么关系了，画图很容易看出来，所以如果不在这种情况下把交点加到新数组里，就会导致这个交点漏掉，即使在下面的循环也没法把这个交点加到数组了
                    New_Point[new_num] = Point[i % temp_num];
                    new_num += 1;
                    //计算交点坐标
                    temp_point.x = Point[i % temp_num].x + (YT - Point[i % temp_num].y) * (Point[(i + 1) % temp_num].x - Point[i % temp_num].x)
                        / (Point[(i + 1) % temp_num].y - Point[i % temp_num].y);
                    temp_point.y = YT;
                    New_Point[new_num] = temp_point;
                    new_num += 1;

                }
            }
        }
        ///////////////////////////

        //重新整理处理上边后的图形


        for (j = 0; j < new_num; j++)
        {
            Point[j] = New_Point[j]; //将新数组的点调回Point，此时的新数组已经保留了裁剪上边后的新的点，这时，把这个经过上边处理后的多边形调回Point,可以这么说，把这个新的多边形重新作为待处理的多边形，进行下边处理
        }
        Number = new_num; //此时的多边形点数为上边裁剪后的新多边形的点数
        new_num = 0; //归0，进行下边处理
        temp_num = Number;
        //对矩形框下边处理，接下来的对下边，左边，右边和上边处理类似，只是在一些细节方面做了一些改变，例如在求交点的相似处理上，有所改变，而且上下两边和左右两边的处理也稍有不同，不过大同小异，把上边处理弄懂后，下面的都差不多
        for (i = 0; i < temp_num; i++)//temp_num 为多边形的边数，执行次数也就是边数
        {
            if (Point[i % temp_num].y > YB) // 头点在下边上，即头点在可视区内
            {
                if (Point[(i + 1) % temp_num].y > YB) // 尾点也在可视区内，两点均在可视区内（内到内）
                {
                    New_Point[new_num] = Point[i % temp_num];
                    new_num += 1;
                    /*New_Point[new_num] = Point[(i + 1) % temp_num];
                    new_num += 1;*/
                }
                else  //尾点在可视区外 ，头点和交点进入新数组（内到外）
                {
                    New_Point[new_num] = Point[i % temp_num];
                    new_num += 1;
                    //求交点
                    temp_point.x = Point[i % temp_num].x - (Point[i % temp_num].y - YB) * (Point[i % temp_num].x - Point[(i + 1) % temp_num].x)
                        / (Point[i % temp_num].y - Point[(i + 1) % temp_num].y);
                    temp_point.y = YB;
                    New_Point[new_num] = temp_point;
                    new_num += 1;

                }
            }
            else //头在可视区外
            {
                if (Point[(i + 1) % temp_num].y >= YB) //尾在可视区内（外到内）
                {
                    //计算交点坐标
                    temp_point.x = Point[i % temp_num].x + (YB - Point[i % temp_num].y) * (Point[(i + 1) % temp_num].x - Point[i % temp_num].x)
                        / (Point[(i + 1) % temp_num].y - Point[i % temp_num].y);
                    temp_point.y = YB;
                    New_Point[new_num] = temp_point;
                    new_num += 1;
                    /*New_Point[new_num] = Point[(i + 1) % temp_num];
                    new_num += 1;*/
                }
                else //头尾两点均在可视区外（外到外）
                {


                }
            }
        }
        //重新整理处理下边后的图形
        for (j = 0; j < new_num; j++)
        {
            Point[j] = New_Point[j]; //将新数组的点调回Point
        }
        Number = new_num; //此时的点数为改变上边后的图形的点数
        new_num = 0; //归0，进行下边处理
        temp_num = Number;
        //对矩形框左边处理
        for (i = 0; i < temp_num; i++)//temp_num 为多边形的边数，执行次数也就是边数
        {
            if (Point[i % temp_num].x >= XL) // 头点在可视区内
            {
                if (Point[(i + 1) % temp_num].x >= XL) // 尾点也在可视区内，两点均在可视区内(内到内）
                {
                    New_Point[new_num] = Point[i % temp_num];
                    new_num += 1;
                    /*New_Point[new_num] = Point[(i + 1) % temp_num];
                    new_num += 1;*/
                }
                else  //尾点在可视区外 ，头点和交点进入新数组（内到外）
                {
                    New_Point[new_num] = Point[i % temp_num];
                    new_num += 1;
                    //求交点
                    temp_point.y = Point[i % temp_num].y - (Point[i % temp_num].x - XL) * (Point[i % temp_num].y - Point[(i + 1) % temp_num].y)
                        / (Point[i % temp_num].x - Point[(i + 1) % temp_num].x);
                    temp_point.x = XL;
                    New_Point[new_num] = temp_point;
                    new_num += 1;

                }
            }
            else //头在可视区外
            {
                if (Point[(i + 1) % temp_num].x >= XL) //尾在可视区内（外到内）
                {
                    //计算交点坐标
                    temp_point.y = Point[i % temp_num].y + (XL - Point[i % temp_num].x) * (Point[(i + 1) % temp_num].y - Point[i % temp_num].y)
                        / (Point[(i + 1) % temp_num].x - Point[i % temp_num].x);
                    temp_point.x = XL;
                    New_Point[new_num] = temp_point;
                    new_num += 1;
                    /*New_Point[new_num] = Point[(i + 1) % temp_num];
                    new_num += 1;*/
                }
                else //头尾两点均在可视区外（外到外）
                {


                }
            }
        }
        //重新整理处理左边后的图形
        for (j = 0; j < new_num; j++)
        {
            Point[j] = New_Point[j]; //将新数组的点调回Point
        }
        Number = new_num; //此时的点数为改变上边后的图形的点数
        new_num = 0; //归0，进行下边处理
        temp_num = Number;
        //对矩形框右边处理
        for (i = 0; i < temp_num; i++)//temp_num 为多边形的边数，执行次数也就是边数
        {
            if (Point[i % temp_num].x <= XR) // 头点在可视区内
            {
                if (Point[(i + 1) % temp_num].x <= XR) // 尾点也在可视区内，两点均在可视区内(内到内）
                {
                    New_Point[new_num] = Point[i % temp_num];
                    new_num += 1;
                    /*New_Point[new_num] = Point[(i + 1) % temp_num];
                    new_num += 1;*/
                }
                else  //尾点在可视区外 ，头点和交点进入新数组（内到外）
                {
                    New_Point[new_num] = Point[i % temp_num];
                    new_num += 1;
                    //求交点
                    temp_point.y = Point[i % temp_num].y + (XR - Point[i % temp_num].x) * (Point[(i + 1) % temp_num].y - Point[i % temp_num].y)
                        / (Point[(i + 1) % temp_num].x - Point[i % temp_num].x);
                    temp_point.x = XR;
                    New_Point[new_num] = temp_point;
                    new_num += 1;

                }
            }
            else //头在可视区外
            {
                if (Point[(i + 1) % temp_num].x <= XR) //尾在可视区内（外到内）
                {
                    //计算交点坐标
                    temp_point.y = Point[i % temp_num].y - (Point[i % temp_num].x - XR) * (Point[i % temp_num].y - Point[(i + 1) % temp_num].y)
                        / (Point[i % temp_num].x - Point[(i + 1) % temp_num].x);
                    temp_point.x = XR;
                    New_Point[new_num] = temp_point;
                    new_num += 1;
                    /*New_Point[new_num] = Point[(i + 1) % temp_num];
                    new_num += 1;*/
                }
                else //头尾两点均在可视区外（外到外）
                {


                }
            }
        }
        //这里是最后一步，经过上下左右四条边的处理，此时的多边形已经裁剪完成，并且储存在New_Point在，下面的工作就是用循环一条边一条边得画出来
        CDC* pDC = this->GetDC();
        CPen newpen(PS_SOLID, 3, RGB(0, 255, 255));
        CPen* old = pDC->SelectObject(&newpen);
        for (j = 0; j < new_num; j++)
        {
            pDC->MoveTo(New_Point[j % new_num].x, New_Point[j % new_num].y);
            pDC->LineTo(New_Point[(j + 1) % new_num].x, New_Point[(j + 1) % new_num].y);
        }
        pDC->MoveTo(New_Point[0].x, New_Point[0].y);
        pDC->LineTo(New_Point[1].x, New_Point[1].y);
        pDC->SelectObject(old);
        ReleaseDC(pDC);

    }
}
