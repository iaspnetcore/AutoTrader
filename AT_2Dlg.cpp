
// AT_2Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AT_2.h"
#include "AT_2Dlg.h"
#include "afxdialogex.h"
#include <afxsock.h>
#include "AppVersion.h"

#include <thread>
#include <mutex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::mutex mtx;
//服务器端口
const int nPort = 9000;
const char* szAddress = "139.196.38.147";

CAT_2Dlg* pThis = nullptr;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAT_2Dlg 对话框



CAT_2Dlg::CAT_2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_AT_2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CAT_2Dlg::~CAT_2Dlg()
{

}


void CAT_2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_listCtrl);
	DDX_Control(pDX, IDC_DATE_BUY1, m_time1);
	DDX_Control(pDX, IDC_DATE_BUY2, m_time2);
	DDX_Control(pDX, IDC_DATE_SELL1, m_time3);
	DDX_Control(pDX, IDC_DATE_SELL2, m_time4);
	DDX_Control(pDX, IDC_LAB_ENABLE, m_labEnable);
	DDX_Control(pDX, IDC_LAB_ALL, m_labAsset);
}

BEGIN_MESSAGE_MAP(CAT_2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CAT_2Dlg::OnBnClickedBtnStart)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CAT_2Dlg::OnLvnItemchangedList)
	ON_BN_CLICKED(IDC_BTN_FLUSH, &CAT_2Dlg::OnBnClickedBtnFlush)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAT_2Dlg 消息处理程序

BOOL CAT_2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码		


	//版本显示
	TCHAR szVersion[MAX_PATH];
	GetAppVersion(szVersion, MAX_PATH);
	TCHAR szCaption[MAX_PATH];
	GetWindowText(szCaption, MAX_PATH);
	_stprintf_s(szCaption, _T("%s %s"), szCaption, szVersion);	
	SetWindowText(szCaption);

	pThis = this;
	InitialAutoTrader2();

	//从服务器获取涨停价格表
	//发送端网络配置

	if (!AfxSocketInit()){
		AfxMessageBox(_T("加载套接字失败"));
		return 0;
	}

	m_socketClient = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == m_socketClient){
		AfxMessageBox(_T("套接字创建失败"));
		return 0;
	}

	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPort);
	addr.sin_addr.S_un.S_addr = inet_addr(szAddress);
	if (connect(m_socketClient, (sockaddr*)&addr, sizeof(sockaddr))) {
		MessageBox(_T("尝试与涨停价获取服务器的连接失败!"));
		return  0;
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAT_2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAT_2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAT_2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAT_2Dlg::GetBalance(TradeBalance& balance)
{
	m_hexin.GetTradeBalance();
	balance = m_hexin.GetTradeBalance_();
}

void CAT_2Dlg::UpdateBalance(LPVOID lp)
{
	CAT_2Dlg* pDlg = (CAT_2Dlg*)lp;
	while (pDlg->m_hexinUpdateBalance){	
		Sleep(100);
		TradeBalance balance;
		RestBalance(balance);
		pDlg->GetBalance(balance);
		CString strTemp;
		strTemp.Format(_T("%0.f"), balance.dEnable);
		pDlg->SetDlgItemTextW(IDC_LAB_ENABLE, strTemp);
		strTemp.Format(_T("%0.f"), balance.dAsset);
		pDlg->SetDlgItemTextW(IDC_LAB_ALL, strTemp);
		Sleep(10000);
	}
}

void CAT_2Dlg::UpdateBalance_once()
{	
	if (m_hexinUpdateBalance) {			
		GetBalance(m_balance);
		CString strTemp;
		strTemp.Format(_T("%0.f"), m_balance.dEnable);
		SetDlgItemTextW(IDC_LAB_ENABLE, strTemp);
		strTemp.Format(_T("%0.f"), m_balance.dAsset);
		SetDlgItemTextW(IDC_LAB_ALL, strTemp);
	}
}

void CAT_2Dlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码

	
	if (!m_warnStart) {

		m_warnStart = true;
		InitialParam();

		SetDlgItemText(IDC_BTN_START, _T("预警停止"));

		if (!m_hexin.GetHexinHwnd()) {
			MessageBox(_T("未找到同花顺交易程序"));
			return;
		}

		//定时激活同花顺核心		
		std::thread thread_keepActivate(KeepHexinActivate, (LPVOID)this);
		thread_keepActivate.detach();		

		//定时更新持仓		
		std::thread thread_updateBalance(UpdateBalance, (LPVOID)this);
		thread_updateBalance.detach();

		//开启同花顺预警买入线程		
		std::thread thread_WarnBuy(StartHexinWarnBuy, (LPVOID)this);
		thread_WarnBuy.detach();
		

		//开启同花顺预警卖出线程		
		std::thread thread_WarnSell(StartHexinWarnSell, (LPVOID)this);
		thread_WarnSell.detach();
		
	}else {

		SetDlgItemText(IDC_BTN_START, _T("预警开始"));
		GetDlgItem(IDC_EDT_BUY_AMOUNT)->EnableWindow(true);
		GetDlgItem(IDC_EDT_BUY_COUNT)->EnableWindow(true);
		GetDlgItem(IDC_DATE_BUY1)->EnableWindow(true);
		GetDlgItem(IDC_DATE_BUY2)->EnableWindow(true);
		GetDlgItem(IDC_EDT_SELL_AMOUNT)->EnableWindow(true);
		GetDlgItem(IDC_EDT_SELL_COUNT)->EnableWindow(true);
		GetDlgItem(IDC_DATE_SELL1)->EnableWindow(true);
		GetDlgItem(IDC_DATE_SELL2)->EnableWindow(true);

		m_hexinKeepActivate = false;
		m_hexinWarnBuy = false;
		m_hexinWarnSell = false;
		m_hexinUpdateBalance = false;
		m_warnStart = false;
	}

	
}

void CAT_2Dlg::StartHexinWarnBuy(LPVOID lp)
{
	CAT_2Dlg* pDlg = (CAT_2Dlg*)lp;
	while (pDlg->m_hexinWarnBuy) {

		Sleep(1000);
		CTime currentTime = CTime::GetCurrentTime();
		int nSecond = currentTime.GetHour() * 3600 + currentTime.GetMinute() * 60 + currentTime.GetSecond();
		if (nSecond< pDlg->second_buyStart || nSecond> pDlg->second_buyStop) {
			AfxMessageBox(_T("当前时间超过同花顺预警买入设定时间 停止预警买入线程")); return;}
		if (!pDlg->m_nBuyCount) {
			AfxMessageBox(_T("买入次数已为0 停止预警买入线程")); return;}

		int amount = pDlg->m_hexin.GetBidAmount();
		if (!amount) continue;

		//同花顺发出预警买入
		if(amount == 68) {
			mtx.lock();
			if (pDlg->m_hexin.HexinWarnBuy(pDlg->m_nBuyAmount)) {				
				pDlg->m_nBuyCount--;
				pDlg->UpdateBuySellCount();
				//将操作结果写到表格 并记录到文件中
				pDlg->Recort(pDlg->m_nBuyAmount,_T("买入"));
				pDlg->UpdateBalance_once();

			}
			mtx.unlock();
		}else if(amount% 100 == 68){			
			int nAmount = pDlg->m_hexin.GetCanBidAmount();
			nAmount = nAmount / (amount / 100);
			nAmount = nAmount / 100 * 100 ;
			mtx.lock();
			if (pDlg->m_hexin.HexinWarnBuy(nAmount)) {
				
				pDlg->m_nBuyCount--;
				pDlg->UpdateBuySellCount();
				//将操作结果写到表格 并记录到文件中
				pDlg->Recort(nAmount, _T("买入"));
				pDlg->UpdateBalance_once();
			}
			mtx.unlock();
		}
	}
}

void CAT_2Dlg::StartHexinWarnSell(LPVOID lp)
{
	CAT_2Dlg* pDlg = (CAT_2Dlg*)lp;
	while (pDlg->m_hexinWarnSell) {

		Sleep(1000);
		CTime currentTime = CTime::GetCurrentTime();
		int nSecond = currentTime.GetHour() * 3600 + currentTime.GetMinute() * 60 + currentTime.GetSecond();
		if (nSecond< pDlg->second_sellStart || nSecond> pDlg->second_sellStop) {
			AfxMessageBox(_T("当前时间超过同花顺预警卖出设定时间 停止预警卖出线程")); return;}
		if (!pDlg->m_nSellCount) {
			AfxMessageBox(_T("卖出次数已为0 停止预警卖出线程")); return;}

		int amount = pDlg->m_hexin.GetAskAmount();
		if (!amount) continue;

		//同花顺发出预警卖出
		if (amount == 88) {
			mtx.lock();
			if (pDlg->m_hexin.HexinWarnSell(pDlg->m_nSellAmount)) {
				pDlg->m_nSellCount--;
				pDlg->UpdateBuySellCount();
				//将操作结果写到表格 并记录到文件中
				pDlg->Recort(pDlg->m_nBuyAmount, _T("卖出"));
				pDlg->UpdateBalance_once();
			}
			mtx.unlock();
		}else if (amount % 100 == 88) {
			mtx.lock();
			int nAmount = pDlg->m_hexin.GetCanAskAmount();
			nAmount = nAmount / (amount / 100);
			nAmount = nAmount / 100 * 100;
			if (pDlg->m_hexin.HexinWarnSell(nAmount)) {
				pDlg->m_nSellCount--;
				pDlg->UpdateBuySellCount();
				//将操作结果写到表格 并记录到文件中
				pDlg->Recort(nAmount, _T("卖出"));
				pDlg->UpdateBalance_once();
			}
			mtx.unlock();
		}
	}
}

void CAT_2Dlg::KeepHexinActivate(LPVOID lp)
{
	CAT_2Dlg* pDlg = (CAT_2Dlg*)lp;
	while (pDlg->m_hexinKeepActivate){
		mtx.lock();
		pDlg->m_hexin.KeepHexinActivate();
		mtx.unlock();
		Sleep(60000);
	}
}

void CAT_2Dlg::InitialAutoTrader2()
{
	SetDlgItemText(IDC_EDT_BUY_TAIL, _T("68"));
	SetDlgItemText(IDC_EDT_SELL_TAIL, _T("88"));
	GetDlgItem(IDC_EDT_BUY_TAIL)->EnableWindow(false);
	GetDlgItem(IDC_EDT_SELL_TAIL)->EnableWindow(false);

	SetDlgItemText(IDC_EDT_BUY_COUNT, _T("3"));
	SetDlgItemText(IDC_EDT_SELL_COUNT, _T("3"));

	CTime currentTime = CTime::GetCurrentTime();
	CTime time1(currentTime.GetYear(), currentTime.GetMonth(), currentTime.GetDay(), 9, 15, 0);
	CTime time2(currentTime.GetYear(), currentTime.GetMonth(), currentTime.GetDay(), 15, 0, 0);

	CTime time3(currentTime.GetYear(), currentTime.GetMonth(), currentTime.GetDay(), 0, 0, 0);
	CTime time4(currentTime.GetYear(), currentTime.GetMonth(), currentTime.GetDay(), 23, 59, 59);

	m_time1.SetTime(&time1);
	m_time2.SetTime(&time2);
	m_time3.SetTime(&time3);
	m_time4.SetTime(&time4);

	//设置成交列表
	m_listCtrl.InsertColumn(0, _T("均价"), LVCFMT_CENTER, 65);//插入列
	m_listCtrl.InsertColumn(0, _T("数量"), LVCFMT_CENTER, 65);//插入列
	m_listCtrl.InsertColumn(0, _T("操作"), LVCFMT_CENTER, 65);//插入列
	m_listCtrl.InsertColumn(0, _T("名称"), LVCFMT_CENTER, 80);//插入列
	m_listCtrl.InsertColumn(0, _T("代码"), LVCFMT_CENTER, 80);//插入列
	m_listCtrl.InsertColumn(0, _T("时间"), LVCFMT_CENTER, 85);//插入列

	LVCOLUMN col;
	col.mask = LVCF_FMT;
	col.fmt = LVCFMT_CENTER;
	m_listCtrl.SetColumn(0, &col);
	m_listCtrl.SetColumn(1, &col);
	m_listCtrl.SetColumn(2, &col);
	m_listCtrl.SetColumn(3, &col);
	m_listCtrl.SetColumn(4, &col);
	m_listCtrl.SetColumn(5, &col);

	//文件操作
	if (!m_file.Open(_T("TradeRecord.txt"), CFile::modeReadWrite)){
		m_file.Open(_T("TradeRecord.txt"), CFile::modeCreate | CFile::modeReadWrite);
		m_file.SeekToBegin();
		const int UNICODE_TXT_FLG = 0xFEFF;
		m_file.Write(&UNICODE_TXT_FLG, 2);
	}

}

void CAT_2Dlg::InitialParam()
{
	CTime time1, time2;
	m_time1.GetTime(time1);
	m_time2.GetTime(time2);
	second_buyStart = time1.GetHour() * 3600 + time1.GetMinute() * 60 + time1.GetSecond();
	second_buyStop = time2.GetHour() * 3600 + time2.GetMinute() * 60 + time2.GetSecond();

	CTime time3, time4;
	m_time3.GetTime(time3);
	m_time4.GetTime(time4);
	second_sellStart = time3.GetHour() * 3600 + time3.GetMinute() * 60 + time3.GetSecond();
	second_sellStop = time4.GetHour() * 3600 + time4.GetMinute() * 60 + time4.GetSecond();

	USES_CONVERSION;
	CString strBuyCount, strBuyAmount;
	GetDlgItemText(IDC_EDT_BUY_AMOUNT, strBuyAmount);
	GetDlgItemText(IDC_EDT_BUY_COUNT, strBuyCount);
	m_nBuyCount = atoi(T2A(strBuyCount));
	m_nBuyAmount = atoi(T2A(strBuyAmount));


	CString strSellCount, strSellAmount;
	GetDlgItemText(IDC_EDT_BUY_AMOUNT, strSellAmount);
	GetDlgItemText(IDC_EDT_BUY_COUNT, strSellCount);
	m_nSellCount = atoi(T2A(strSellCount));
	m_nSellAmount = atoi(T2A(strSellAmount));

	GetDlgItem(IDC_EDT_BUY_AMOUNT)->EnableWindow(false);
	GetDlgItem(IDC_EDT_BUY_COUNT)->EnableWindow(false);
	GetDlgItem(IDC_DATE_BUY1)->EnableWindow(false);
	GetDlgItem(IDC_DATE_BUY2)->EnableWindow(false);
	GetDlgItem(IDC_EDT_SELL_AMOUNT)->EnableWindow(false);
	GetDlgItem(IDC_EDT_SELL_COUNT)->EnableWindow(false);
	GetDlgItem(IDC_DATE_SELL1)->EnableWindow(false);
	GetDlgItem(IDC_DATE_SELL2)->EnableWindow(false);

	m_hexin.GetAllHwnd();	
	m_hexin.KeepHexinActivate();
	m_hexin.GetTradeBalance();	
	m_balance = m_hexin.GetTradeBalance_();

	m_hexinKeepActivate = true;
	m_hexinWarnBuy = true;
	m_hexinWarnSell = true;
	m_hexinUpdateBalance = true;
	Sleep(100);

}

void CAT_2Dlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CAT_2Dlg::UpdateBuySellCount()
{
	CString strBuyCount, strSellCount;
	strBuyCount.Format(_T("%d"), m_nBuyCount);
	strSellCount.Format(_T("%d"), m_nSellCount);
	SetDlgItemText(IDC_EDT_BUY_COUNT, strBuyCount);
	SetDlgItemText(IDC_EDT_SELL_COUNT, strSellCount);
	Invalidate();
}

void CAT_2Dlg::Recort(int amount_ , CString oper)
{
	CTime time = CTime::GetCurrentTime();
	CString strTime = time.Format("%H:%M:%S");
	CString code, name, amount, price;
	if (oper.Compare(_T("买入")) == 0) {
		code = m_hexin.GetStockCodeBuy();
		name = m_hexin.GetStockNameBuy();
		price.Format(_T("%.2f"), m_hexin.GetHighLimit());
	}else {
		code = m_hexin.GetStockCodeSell();
		name = m_hexin.GetStockNameSell();
		price.Format(_T("%.2f"), m_hexin.GetLowLimit());
	}

	amount.Format(_T("%d"), amount_);
	
	int nRow = m_listCtrl.InsertItem(0, strTime);
	m_listCtrl.SetItemText(nRow, 1, code);
	m_listCtrl.SetItemText(nRow, 2, name);
	m_listCtrl.SetItemText(nRow, 3, oper);
	m_listCtrl.SetItemText(nRow, 4, amount);
	m_listCtrl.SetItemText(nRow, 5, price);

	strTime = time.Format(_T("%Y-%m-%d   %H:%M:%S"));
	CString strWrite = strTime + _T("   ") + code + _T("   ") + name + _T("   ") + oper + _T("   ") + amount + _T("   ") + price + _T("\r\n");
	m_file.SeekToEnd();
	m_file.Write(strWrite.GetBuffer(), strWrite.GetLength()*2);
}

void CAT_2Dlg::OnBnClickedBtnFlush()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_flashBuy == nullptr){	
		m_flashBuy = std::make_shared<CDlgFlashBuy>();
		m_flashBuy->Create(IDD_DLG_QUCIKINS, GetDesktopWindow());
		m_flashBuy->SetCallBack(CallBack_FlushBuy);
		m_flashBuy->ShowWindow(TRUE);
		std::thread t(GetHighLimit, pThis);
		t.detach();		
	}else if(m_flashBuy->IsWindowVisible()){
		m_flashBuy->ShowWindow(false);
	}else {
		m_flashBuy->ShowWindow(true);
	}
}


void CAT_2Dlg::CallBack_FlushBuy(CString code, int amount)
{
	//获取涨停价
	USES_CONVERSION;

	CString price = _T("");
	auto iter = pThis->m_highLimit.find(code);
	if (iter != pThis->m_highLimit.end())
		price = CString(iter->second);

	//获取买入数量	*0.99确保有足够的钱
	int nAmount = ((int)(pThis->m_balance.dEnable * 0.99 / (atof(T2A(price))*amount)) / 100 ) * 100;
	CString strAmount;
	strAmount.Format(_T("%d"), nAmount);

	mtx.lock();
	pThis->m_hexin.FlashBuyStock(code, price, strAmount);
	mtx.unlock();
	pThis->UpdateBalance_once();
	pThis->Recort(code,_T(""),_T("买入"), strAmount, price);
}

void CAT_2Dlg::Recort(CString code_, CString name_, CString oper_, CString amount_, CString price_)
{
	CTime time = CTime::GetCurrentTime();
	CString strTime = time.Format("%H:%M:%S");

	CString code, name,oper, amount, price;
	code = code_;
	name = name_;
	oper = _T("买入");
	amount = amount_;
	price = price_;
	int nRow = m_listCtrl.InsertItem(0, strTime);
	m_listCtrl.SetItemText(nRow, 1, code);
	m_listCtrl.SetItemText(nRow, 2, name);
	m_listCtrl.SetItemText(nRow, 3, oper);
	m_listCtrl.SetItemText(nRow, 4, amount);
	m_listCtrl.SetItemText(nRow, 5, price);

	strTime = time.Format(_T("%Y-%m-%d   %H:%M:%S"));
	CString strWrite = strTime + _T("   ") + code + _T("   ") + name + _T("   ") + oper + _T("   ") + amount + _T("   ") + price + _T("\r\n");
	m_file.SeekToEnd();
	m_file.Write(strWrite.GetBuffer(), strWrite.GetLength() * 2);
}

HBRUSH CAT_2Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	int STATIC_FONT[] = { IDC_LAB_ENABLE ,IDC_LAB_ALL };
	for (int i = 0; i < sizeof(STATIC_FONT) / sizeof(int); ++i)	
		if (pWnd->GetDlgCtrlID() == STATIC_FONT[i])
			pDC->SetTextColor(RGB(255, 0, 0));	

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CAT_2Dlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	m_file.Flush();
	m_file.Close();

	m_hexinKeepActivate = true;
	m_hexinWarnBuy = true;
	m_hexinWarnSell = true;
	m_hexinUpdateBalance = true;

	CDialog::OnClose();
}


void CAT_2Dlg::GetHighLimit(LPVOID lp) {


	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPort);
	addr.sin_addr.S_un.S_addr = inet_addr(szAddress);
	
	char sendData[100] = "GetHighLimitTable";
	char* recvData = new char[100000];
	memset(recvData, 0, 100000);
	
	int len = 0;
	bool bFinish = false;
	sendto(pThis->m_socketClient, sendData, strlen(sendData), 0, (SOCKADDR*)&addr, len);
	CString strRecvData = _T("");
	int ret = 0;	
	Sleep(5000);

	while (!bFinish) {
		ret = recvfrom(pThis->m_socketClient, recvData, 100000, 0, (SOCKADDR*)&addr, &len);
		CString temp = CString(recvData);
		strRecvData += CString(recvData);
		if (temp.Right(3).Compare(_T("end")) == 0) bFinish = true;
	}	
	
	CString code = strRecvData.Left(strRecvData.Find(_T("|")));
	CString price = strRecvData.Right(strRecvData.GetLength() - strRecvData.Find(_T("|")) - 1);

	CStringArray acode;
	CStringArray aprice;
	pThis->DivString(acode, code);
	pThis->DivString(aprice, price);
	
	for (int i = 0; i < acode.GetCount(); ++i){
		if (i == acode.GetCount() - 1)
			pThis->m_highLimit.insert(std::pair<CString, CString>(acode.GetAt(i).Left(6), aprice.GetAt(i).Left(aprice.GetAt(i).GetLength()-3)));;
		pThis->m_highLimit.insert(std::pair<CString, CString>(acode.GetAt(i).Left(6), aprice.GetAt(i)));
	}		

	if (!pThis->m_highLimit.size())
		AfxMessageBox(_T("未能从服务器获取涨停价格表"));
	delete[] recvData;

}


void CAT_2Dlg::DivString(CStringArray& array , CString& test)
{	
	while (TRUE){
		int index = test.Find(_T(","));
		if (index == -1){
			array.Add(test);
			return ;
		}
		CString test1 = test.Left(index);
		array.Add(test1);
		test = test.Right(test.GetLength() - index - 1);
	}
}
