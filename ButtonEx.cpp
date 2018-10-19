// ButtonST.cpp : implementation file
//
#include "stdafx.h"
#include "ButtonEx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define BS_TYPEMASK SS_TYPEMASK
/////////////////////////////////////////////////////////////////////////////
// CButtonST
CButtonEx::CButtonEx()
{   
	m_strURL = "";
	m_nShOffset = 30;
    m_IsPressed = FALSE;
	m_pBitMapBK = NULL;
	m_IsLButtonDown = false;
	m_TextColor = RGB(0, 0, 0);
    m_BkColor = RGB(216,233,216);   	
}
CButtonEx::~CButtonEx()
{
    if(m_pBitMapBK!=NULL)
    {
        delete m_pBitMapBK;
        m_pBitMapBK = NULL;
    }
}
BEGIN_MESSAGE_MAP(CButtonEx, CButton)
    //{{AFX_MSG_MAP(CButtonST)
    ON_WM_MOUSEMOVE()
    ON_WM_NCACTIVATE()
    ON_WM_NCMOUSEMOVE()
    ON_WM_KILLFOCUS()    
    ON_WM_LBUTTONDOWN()
    //}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_SETCURSOR()
//	ON_WM_PAINT()
ON_WM_CREATE()
ON_WM_DRAWITEM()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CButtonST message handlers
//设置背景颜色
void CButtonEx::SetBkColor(COLORREF BkColor)
{
    m_BkColor = BkColor;
    this->Invalidate();
}
//设置鼠标在按钮上的偏移颜色
void CButtonEx::SetShOffset(int nShOffset)
{
    m_nShOffset = nShOffset;
    this->Invalidate();
}
//设置字体颜色
void CButtonEx::SetTextColor(COLORREF TextColor)
{
    m_TextColor = TextColor;
    this->Invalidate();
}
//设置网页链接
void CButtonEx::SetURL(CString strURL)
{
    m_strURL = strURL;
}
//设置背景图片
void CButtonEx::SetBkPicture(CBitmap *pBitMap)
{
    if(m_pBitMapBK==pBitMap)
        return;
    if(m_pBitMapBK!=NULL)
    {
        delete m_pBitMapBK;
        m_pBitMapBK = pBitMap;
    }
    m_pBitMapBK = pBitMap;
    this->Invalidate();
}
//背景颜色偏移
COLORREF CButtonEx::OffsetColor(COLORREF color,int nOffset)
{
    BYTE    byRed = 0;
    BYTE    byGreen = 0;
    BYTE    byBlue = 0;
    short   shOffsetR = nOffset;
    short   shOffsetG = nOffset;
    short   shOffsetB = nOffset;
    if (nOffset < -255 || nOffset > 255)
    {
        nOffset = 30;
    }
    
    // Get RGB components of specified color
    byRed = GetRValue(color);
    byGreen = GetGValue(color);
    byBlue = GetBValue(color);
    
    // Calculate max. allowed real offset
    if (nOffset > 0)
    {
        if (byRed + nOffset > 255)      shOffsetR = 255 - byRed;
        if (byGreen + nOffset > 255)    shOffsetG = 255 - byGreen;          
        if (byBlue + nOffset > 255)     shOffsetB = 255 - byBlue;
        nOffset = min(min(shOffsetR, shOffsetG), shOffsetB);
    }
    else
    {
        if (byRed + nOffset < 0)        shOffsetR = -byRed;
        if (byGreen + nOffset < 0)      shOffsetG = -byGreen;
        if (byBlue + nOffset < 0)       shOffsetB = -byBlue;
        
        nOffset = max(max(shOffsetR, shOffsetG), shOffsetB);
    }
    int r,g,b;
    r = byRed + nOffset;
    g =byGreen + nOffset;
    b =byBlue + nOffset;
    return RGB(r,g,b);
}
void CButtonEx::DrawItem1(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    // TODO: Add your code to draw the specified item   
    
    
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rect = lpDrawItemStruct->rcItem;
    CRect offRect = rect;
    int ndist = 2;
    offRect.left+= ndist;
    offRect.right -=ndist;
    offRect.top += ndist;
    offRect.bottom -=ndist;
    
    
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(m_TextColor);
    
    //绘制背景
    if(m_pBitMapBK!=NULL)
    {
        CDC* pNewDC = new CDC;
        pNewDC->CreateCompatibleDC(pDC);
        pNewDC->SelectObject(m_pBitMapBK);
        BITMAP   bmp;
        m_pBitMapBK->GetBitmap(&bmp);
        if(m_IsPressed)
        {
            pDC->StretchBlt(0,0,rect.Width(),rect.Height(),pNewDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
        }
        else
        {
            CBrush brush(RGB(220,220,220));//RGB(m_nShOffset,m_nShOffset,m_nShOffset));
            pDC->FillRect(&rect,&brush);
            pDC->StretchBlt(0,0,rect.Width(),rect.Height(),pNewDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCAND);
        }
        
        delete pNewDC;
        pNewDC = NULL;
    }
    else
    {
        if(m_IsPressed)
        {
            CBrush brush(OffsetColor(m_BkColor,m_nShOffset));
            pDC->FillRect(rect,&brush);             
            CBrush brush1(RGB(128,128,128));
            pDC->FrameRect(rect,&brush1);
            pDC->FrameRect(offRect,&brush1);            
        }
		if (m_IsLButtonDown)
		{
			CBrush brush(OffsetColor(m_BkColor, m_nShOffset));
			pDC->FillRect(rect, &brush);
			CBrush brush1(RGB(200, 200, 200));
			pDC->FrameRect(rect, &brush1);
			pDC->FrameRect(offRect, &brush1);

		}
        else
        {
            CBrush brush(m_BkColor);
            pDC->FillRect(rect,&brush);
            CBrush brush1(RGB(128,128,128));
            pDC->FrameRect(offRect,&brush1);
        }
    }
    
    
    CString sTitle;
    GetWindowText(sTitle);
    offRect.OffsetRect(0,rect.Height()/2-8);
	//offRect.OffsetRect(0, 0);
    ////  pDC->DrawText(sTitle, sTitle.GetLength(),offRect, DT_WORDBREAK | DT_CENTER);
    pDC->DrawText(sTitle, sTitle.GetLength(),offRect, DT_WORDBREAK|DT_CENTER);
}
void CButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    // TODO: Add your code to draw the specified item  
    DrawItem1(lpDrawItemStruct);
    return;
    
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rect = lpDrawItemStruct->rcItem;
    CRect offRect = rect;
    int ndist = 2;
    offRect.left+= ndist;
    offRect.right -=ndist;
    offRect.top += ndist;
    offRect.bottom -=ndist;
    CDC* pResetDC=new CDC();
    pResetDC->CreateCompatibleDC(pDC);
    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
    pResetDC->SelectObject(&bitmap);    
    pResetDC->SetBkMode(TRANSPARENT);
    pResetDC->SetTextColor(m_TextColor);
    
    //绘制背景
    if(m_pBitMapBK!=NULL)
    {
        CDC* pNewDC = new CDC;
        pNewDC->CreateCompatibleDC(pResetDC);
        pNewDC->SelectObject(m_pBitMapBK);
        BITMAP   bmp;
        m_pBitMapBK->GetBitmap(&bmp);
        if(m_IsPressed)
        {
            pResetDC->StretchBlt(0,0,rect.Width(),rect.Height(),pNewDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
        }
        else
        {
            CBrush brush(RGB(220,220,220));//RGB(m_nShOffset,m_nShOffset,m_nShOffset));
            pResetDC->FillRect(&rect,&brush);
            pResetDC->StretchBlt(0,0,rect.Width(),rect.Height(),pNewDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCAND);
        }
        
        delete pNewDC;
        pNewDC = NULL;
    }
    else
    {
        if(m_IsPressed)
        {
            CBrush brush(OffsetColor(m_BkColor,m_nShOffset));
            pResetDC->FillRect(rect,&brush);                
            CBrush brush1(RGB(234, 242, 250));
            pResetDC->FrameRect(rect,&brush1);
            pResetDC->FrameRect(offRect,&brush1);
            
        }
		if (m_IsLButtonDown)
		{
			CBrush brush(OffsetColor(m_BkColor, m_nShOffset));
			pResetDC->FillRect(rect, &brush);
			CBrush brush1(RGB(200, 200, 200));
			pResetDC->FrameRect(rect, &brush1);
			pResetDC->FrameRect(offRect, &brush1);

		}
        else
        {
            CBrush brush(m_BkColor);
            pResetDC->FillRect(rect,&brush);
            CBrush brush1(RGB(128,128,128));
            pResetDC->FrameRect(offRect,&brush1);
        }
    }
    
    
    //CString sTitle;
    //GetWindowText(sTitle);
    //offRect.OffsetRect(0,rect.Height()/2 - 18);
    //pDC->DrawText(sTitle, sTitle.GetLength(),offRect, DT_WORDBREAK | DT_CENTER);
    ////pResetDC->DrawText(sTitle, sTitle.GetLength(),offRect, DT_CENTER);
    //pDC->StretchBlt(0,0,rect.Width(),rect.Height(),pResetDC,0,0,rect.Width(),rect.Height(),SRCCOPY);

    delete pResetDC;
}
void CButtonEx::OnMouseMove(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    TRACKMOUSEEVENT     csTME;
    csTME.cbSize = sizeof(csTME);
    csTME.dwFlags = TME_LEAVE;
    csTME.hwndTrack = m_hWnd;
    ::_TrackMouseEvent(&csTME); 
    if(m_IsPressed==FALSE)
    {
        m_IsPressed = TRUE;     
        this->Invalidate();
    }
    
    CButton::OnMouseMove(nFlags, point);
}
BOOL CButtonEx::OnNcActivate(BOOL bActive) 
{
    // TODO: Add your message handler code here and/or call default
    return CButton::OnNcActivate(bActive);
}
void CButtonEx::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    
    
    CButton::OnNcMouseMove(nHitTest, point);
}
void CButtonEx::OnKillFocus(CWnd* pNewWnd) 
{
    CButton::OnKillFocus(pNewWnd);  
    // TODO: Add your message handler code here
}
LRESULT CButtonEx::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
    m_IsPressed = FALSE;
    this->Invalidate();
    return 0;
} // End of OnMouseLeave
BOOL CButtonEx::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
    // TODO: Add your specialized code here and/or call the base class  
    dwStyle |= BS_OWNERDRAW;
    return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
void CButtonEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default

	m_IsLButtonDown = 1;
	Invalidate();
    CButton::OnLButtonDown(nFlags, point);
}
BOOL CButtonEx::PreCreateWindow(CREATESTRUCT& cs) 
{
    // TODO: Add your specialized code here and/or call the base class
    
    return CButton::PreCreateWindow(cs);
}
void CButtonEx::PreSubclassWindow() 
{
    // TODO: Add your specialized code here and/or call the base class
    ModifyStyle(BS_TYPEMASK, BS_OWNERDRAW, SWP_FRAMECHANGED);   
    CButton::PreSubclassWindow();
}

HBRUSH CButtonEx::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CButton::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

	return hbr;
}


void CButtonEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_IsLButtonDown = 0;
	Invalidate();
	CButton::OnLButtonUp(nFlags, point);
}


BOOL CButtonEx::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CButton::OnSetCursor(pWnd, nHitTest, message);
}


//void CButtonEx::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//					   // TODO: 在此处添加消息处理程序代码
//					   // 不为绘图消息调用 CButton::OnPaint()
//
//
//}


int CButtonEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}


void CButtonEx::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//UINT uStyle = DFCS_BUTTONPUSH;
	//ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);
	//if (lpDrawItemStruct->itemState & ODS_SELECTED)
	//{
	//	uStyle = DFCS_PUSHED;
	//}
	//::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem,
	//	DFC_BUTTON, uStyle);

	//CString strText;
	//GetWindowText(strText);
	//COLORREF crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, RGB(255, 0, 0));
	//::DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(),
	//	&lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	//::SetTextColor(lpDrawItemStruct->hDC, crOldColor);
	CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
