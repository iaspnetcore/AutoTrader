// DlgFlashBuy.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgFlashBuy.h"
#include "afxdialogex.h"
#include <dbghelp.h>
#pragma comment(lib , "Dbghelp.lib")
#include "resource.h"

// CDlgFlashBuy 对话框

CDlgFlashBuy* pDlg;

IMPLEMENT_DYNAMIC(CDlgFlashBuy, CDialogEx)

CDlgFlashBuy::CDlgFlashBuy(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_QUCIKINS, pParent)
{

}

CDlgFlashBuy::~CDlgFlashBuy()
{
}

void CDlgFlashBuy::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_CHK1, m_chk1_1);
	DDX_Control(pDX, IDC_CHK2, m_chk1_2);
	DDX_Control(pDX, IDC_CHK3, m_chk1_3);
	DDX_Control(pDX, IDC_CHK4, m_chk1_4);	
	DDX_Control(pDX, IDC_BTN_BUY, m_btnBuy);
	DDX_Control(pDX, IDC_LAB_CODE, m_labCode);
}


BEGIN_MESSAGE_MAP(CDlgFlashBuy, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()	
	ON_BN_CLICKED(IDC_CHK1, OnBnClickedChk1)
	ON_BN_CLICKED(IDC_CHK2, OnBnClickedChk2)
	ON_BN_CLICKED(IDC_CHK3, OnBnClickedChk3)
	ON_BN_CLICKED(IDC_CHK4, OnBnClickedChk4)
	ON_BN_CLICKED(IDC_BTN_BUY, OnBnClickedBtnBuy)
	ON_STN_CLICKED(IDC_LAB_CODE, OnStnClickedLabCode)
END_MESSAGE_MAP()


// CDlgFlashBuy 消息处理程序


BOOL CDlgFlashBuy::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	pDlg = this;

	m_font_1.CreateFont(
		24,                        // nHeight  
		0,                         // nWidth  
		0,                         // nEscapement  
		0,                         // nOrientation  
		FW_BOLD,                    // nWeight  
		FALSE,                     // bItalic  
		FALSE,                      // bUnderline  
		0,                         // cStrikeOut  
		ANSI_CHARSET,              // nCharSet  
		OUT_DEFAULT_PRECIS,        // nOutPrecision  
		CLIP_DEFAULT_PRECIS,       // nClipPrecision  
		DEFAULT_QUALITY,           // nQuality  
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily  
		_T("宋体"));

	m_font_2.CreateFont(
		14,                        // nHeight  
		0,                         // nWidth  
		0,                         // nEscapement  
		0,                         // nOrientation  
		FW_NORMAL,                    // nWeight  
		FALSE,                     // bItalic  
		FALSE,                      // bUnderline  
		0,                         // cStrikeOut  
		ANSI_CHARSET,              // nCharSet  
		OUT_DEFAULT_PRECIS,        // nOutPrecision  
		CLIP_DEFAULT_PRECIS,       // nClipPrecision  
		DEFAULT_QUALITY,           // nQuality  
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily  
		_T("宋体"));
	
	GetDlgItem(IDC_CHK1)->SetFont(&m_font_2);
	GetDlgItem(IDC_CHK2)->SetFont(&m_font_2);
	GetDlgItem(IDC_CHK3)->SetFont(&m_font_2);
	GetDlgItem(IDC_CHK4)->SetFont(&m_font_2);
	GetDlgItem(IDC_BTN_BUY)->SetFont(&m_font_2);
	GetDlgItem(IDC_LAB_CODE)->SetFont(&m_font_1);	
	
	m_btnBuy.SetBkColor(RGB(64, 69, 76));
	m_btnBuy.SetTextColor(RGB(255, 255, 255));
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadCapture, NULL, 0, &threadID);
	CloseHandle(hThread);
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


HBRUSH CDlgFlashBuy::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	int nID[] = { IDC_BTN_BUY,IDC_CHK1,IDC_CHK2,IDC_CHK3,IDC_CHK4 };

	for (int i = 0; i < sizeof(nID) / sizeof(int); ++i){
		if (pWnd == GetDlgItem(nID[i])){
			pDC->SetTextColor(RGB(255, 255, 255));			
			pDC->SetBkMode(TRANSPARENT);
			return (HBRUSH)GetStockObject(NULL_BRUSH);
		}
	}

	if (pWnd == GetDlgItem(IDC_LAB_CODE)){
		pDC->SetTextColor(RGB(255, 255, 0));	
		pDC->SetBkColor(RGB(64, 69, 76));
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CDlgFlashBuy::OnStnClickedLabCode()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgFlashBuy::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	//CDialogEx::OnLButtonDown(nFlags, point);
}


void CDlgFlashBuy::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(64, 69, 76));

}


void CDlgFlashBuy::OnBnClickedChk1()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton*)GetDlgItem(IDC_CHK2))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHK3))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHK4))->SetCheck(FALSE);
	m_amount = 1;
}


void CDlgFlashBuy::OnBnClickedChk2()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton*)GetDlgItem(IDC_CHK1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHK3))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHK4))->SetCheck(FALSE);
	m_amount = 2;
}


void CDlgFlashBuy::OnBnClickedChk3()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton*)GetDlgItem(IDC_CHK1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHK2))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHK4))->SetCheck(FALSE);
	m_amount = 3;
}


void CDlgFlashBuy::OnBnClickedChk4()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton*)GetDlgItem(IDC_CHK1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHK2))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHK3))->SetCheck(FALSE);
	m_amount = 4;
}


void CDlgFlashBuy::SetCallBack(CallBack callback)
{
	m_callback = callback;
}

void CDlgFlashBuy::OnBnClickedBtnBuy()
{
	// TODO: 在此添加控件通知处理程序代码
	m_callback(m_code, m_amount);
}

CString CDlgFlashBuy::GetCodeFromAddr()
{
	char code[7];
	ReadProcessMemory(pDlg->m_dzhHandle, (LPCVOID)(pDlg->m_CodeAddr), code, 6, NULL);
	code[6] = '\0';	
	return CString(code);
}

BOOL CDlgFlashBuy::MyEnumerateLoadedModulesProc64(PTSTR ModuleName,
	DWORD64 ModuleBase,
	ULONG ModuleSize,
	PVOID UserContext)
{	
	pDlg->m_CodeAddr = ModuleBase + 0x104C2C2;
	return false;
}

void CDlgFlashBuy::ThreadCapture(LPVOID lp)
{
	while (true){
		Sleep(100);
		if (pDlg->m_CodeAddr == NULL) {
			HWND  hwnd = nullptr;
			pDlg->GetDZHHwnd(&hwnd);
			pDlg->GetBaseAddr(hwnd);
		}
		CString strCode = pDlg->GetCodeFromAddr();
		if(pDlg->m_CodeAddr == NULL) strCode = _T("000000");
		pDlg->m_code = strCode;
		((CStatic*)pDlg->GetDlgItem(IDC_LAB_CODE))->SetWindowText(strCode);		
	}
}

HWND CDlgFlashBuy::GetDZHHwnd(HWND* hwnd)
{
	HWND DesktopHwnd = ::GetDesktopWindow();
	EnumChildWindows(DesktopHwnd, EnumChildProc, LPARAM(hwnd));
	return *hwnd;
}

BOOL CDlgFlashBuy::EnumChildProc(HWND hwndChild, LPARAM lParam)
{
	CWnd* wnd = CWnd::FromHandle(hwndChild);
	CString strText;
	wnd->GetWindowText(strText);
	if (strText.Left(6).Compare(_T("大智慧 - ")) == 0) {
		HWND* hwnd = (HWND*)lParam;
		*hwnd = hwndChild;		
		return false;
	}
	return true;
}

DWORD CDlgFlashBuy::GetBaseAddr(HWND hwnd)
{
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);//进程id
	if (!pid) return 0;
	m_dzhHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	if (m_dzhHandle == NULL) {
		MessageBox(_T("未找到大智慧进程"), _T("warning!"), MB_OK);
		return 0;
	}
	EnumerateLoadedModules64(m_dzhHandle,
		(PENUMLOADED_MODULES_CALLBACK64)MyEnumerateLoadedModulesProc64,
		(PVOID)NULL);
	return 0;
}