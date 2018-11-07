#include "stdafx.h"
#include "Hexin.h"
#include <thread>
#include <mutex>

CHexin::CHexin()
{

}


CHexin::~CHexin()
{

}

HWND CHexin::FindWindowChildEx(HWND hwndParent, HWND hwndChild, LPCTSTR lpszClass, LPCTSTR lpszWindow, int count)
{
	while (count--) hwndChild = ::FindWindowEx(hwndParent, hwndChild, lpszClass, lpszWindow);
	return 	hwndChild;
}

void CHexin::SendVirtualKey(HWND hwnd, SHORT vk)
{
	WPARAM wParam = vk & 0xFF;
	LPARAM lParam = 1;
	lParam += MapVirtualKey(wParam, MAPVK_VK_TO_VSC) << 16;
	::PostMessage(hwnd, WM_KEYDOWN, wParam, lParam);
	lParam += 1 << 30; lParam += 1 << 31;
	::PostMessage(hwnd, WM_KEYUP, wParam, lParam);
}

bool CHexin::GetTradeBalance()
{
	wchar_t szText[50];
	bool ret = false;
	
	HWND hwnd_Position = NULL;
	HWND hwnd_Static = NULL;
	while (true) {
		hwnd_Position = ::FindWindowEx(hAfxMDIFrame42s, hwnd_Position, _T("#32770"), NULL);
		if (!hwnd_Position) return ret;
		hwnd_Static = ::FindWindowEx(hwnd_Position, NULL, _T("Static"), _T("查询资金股票"));
		if (!hwnd_Static) continue;
		else break;
	}
	HWND hwnd_text = NULL;
	
	USES_CONVERSION;
	hwnd_text = FindWindowChildEx(hwnd_Position, NULL, _T("Static"), NULL, 5);
	::SendMessage(hwnd_text, WM_GETTEXT, MAX_PATH, LPARAM(szText));
	sscanf_s(W2A(szText), ("%lf"), &m_balance.dRemain);
	hwnd_text = FindWindowChildEx(hwnd_Position, hwnd_text, _T("Static"), NULL, 1);
	::SendMessage(hwnd_text, WM_GETTEXT, MAX_PATH, LPARAM(szText));
	sscanf_s(W2A(szText), ("%lf"), &m_balance.dFrozen);
	hwnd_text = FindWindowChildEx(hwnd_Position, hwnd_text, _T("Static"), NULL, 1);
	::SendMessage(hwnd_text, WM_GETTEXT, MAX_PATH, LPARAM(szText));
	sscanf_s(W2A(szText), ("%lf"), &m_balance.dEnable);
	hwnd_text = FindWindowChildEx(hwnd_Position, hwnd_text, _T("Static"), NULL, 4);
	::SendMessage(hwnd_text, WM_GETTEXT, MAX_PATH, LPARAM(szText));
	sscanf_s(W2A(szText), ("%lf"), &m_balance.dCash);
	hwnd_text = FindWindowChildEx(hwnd_Position, hwnd_text, _T("Static"), NULL, 1);
	::SendMessage(hwnd_text, WM_GETTEXT, MAX_PATH, LPARAM(szText));
	sscanf_s(W2A(szText), ("%lf"), &m_balance.dMarket);
	hwnd_text = FindWindowChildEx(hwnd_Position, hwnd_text, _T("Static"), NULL, 1);
	::SendMessage(hwnd_text, WM_GETTEXT, MAX_PATH, LPARAM(szText));
	sscanf_s(W2A(szText), ("%lf"), &m_balance.dAsset);
	hwnd_text = FindWindowChildEx(hwnd_Position, hwnd_text, _T("Static"), NULL, 27);
	::SendMessage(hwnd_text, WM_GETTEXT, MAX_PATH, LPARAM(szText));
	sscanf_s(W2A(szText), ("%lf"), &m_balance.dDayProfit);
	hwnd_text = FindWindowChildEx(hwnd_Position, hwnd_text, _T("Static"), NULL, 2);
	::SendMessage(hwnd_text, WM_GETTEXT, MAX_PATH, LPARAM(szText));
	sscanf_s(W2A(szText), ("%lf"), &m_balance.dTotalProfit);

	return true;
}

TradeBalance& CHexin::GetTradeBalance_()
{
	return m_balance;
}

bool CHexin::FlashBuyStock(CString strStock, CString strPrice, CString strAmount)
{

	USES_CONVERSION;

	bool ret = false;
	HWND buyHwnd = NULL;
	if (!hBuyStock) 
		buyHwnd = GetBuyStockHwnd();	
	buyHwnd = hBuyStock;

	int nSelMktIndex;
	nSelMktIndex = ::SendMessage(hToolBarMarket, CB_GETCURSEL, 0, 0);
	if (strStock.Left(1).Compare( _T("6")) ==0 && nSelMktIndex != 0) {
		::SendMessage(hToolBarMarket, CB_SETCURSEL, 0, 0);		
		::SendMessage(hToolBarBox, WM_COMMAND, MAKEWPARAM(GetWindowLong(hToolBarMarket, GWL_ID), CBN_SELCHANGE), (LPARAM)hToolBarMarket);
		WaitForDone(hToolBarHolder, WFC_ComboBox_LeftMatch, "A", 10, 500);
	}
	else if ( (strStock.Left(1).Compare(_T("3"))==0 || strStock.Left(1).Compare(_T("0")) == 0) && nSelMktIndex != 1) {
		::SendMessage(hToolBarMarket, CB_SETCURSEL, 1, 0);		
		::SendMessage(hToolBarBox, WM_COMMAND, MAKEWPARAM(GetWindowLong(hToolBarMarket, GWL_ID), CBN_SELCHANGE), (LPARAM)hToolBarMarket);
		WaitForDone(hToolBarHolder, WFC_ComboBox_LeftMatch, "0", 10, 500);
	}

	HWND codeHwnd = NULL;
	codeHwnd = FindWindowChildEx(buyHwnd, NULL, _T("Edit"), NULL, 1);
	if (!codeHwnd)  return ret;	
	::SendMessage(codeHwnd, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)strStock);
	WaitForDone(codeHwnd, WFC_TextBox_NotEmpty, T2A(strStock.GetBuffer()), 10, 500);

	HWND priceHwnd = NULL;
	priceHwnd = FindWindowChildEx(buyHwnd, NULL, _T("Edit"), NULL, 2);
	if (!priceHwnd)  return ret;	
	::SendMessage(priceHwnd, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)strPrice);
	WaitForDone(codeHwnd, WFC_TextBox_NotEmpty, T2A(strStock.GetBuffer()), 10, 500);
	
	HWND amountHwnd = NULL;
	amountHwnd = FindWindowChildEx(buyHwnd, NULL, _T("Edit"), NULL, 3);
	if (!amountHwnd)  return ret;
	::SendMessage(amountHwnd, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)strAmount);
	WaitForDone(codeHwnd, WFC_TextBox_NotEmpty, T2A(strStock.GetBuffer()), 10, 500);

	//同花顺版本不同 可能是 买入或者买入B
	HWND confirmHwnd  = NULL;
	confirmHwnd  = ::FindWindowEx(buyHwnd, NULL, NULL, _T("买入"));
	if (!confirmHwnd) confirmHwnd = ::FindWindowEx(buyHwnd, NULL, NULL, _T("买入[B]"));
	if(!confirmHwnd) return ret;	
	
	::SendMessage(confirmHwnd, BM_CLICK, 0, 0);
	return true;
}

bool CHexin::FlashSellStock(CString strStock, CString strPrice, CString strAmount)
{
	bool ret = false;
	HWND sellHwnd;
	if (!hBuyStock) sellHwnd = GetBuyStockHwnd();
	if (!sellHwnd) return ret;

	HWND codeHwnd = NULL;
	codeHwnd = FindWindowChildEx(sellHwnd, NULL, _T("Edit"), NULL, 1);
	if (!codeHwnd)  return ret;
	::SendMessage(codeHwnd, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)strStock);	

	HWND priceHwnd = NULL;
	priceHwnd = FindWindowChildEx(sellHwnd, NULL, _T("Edit"), NULL, 2);
	if (!priceHwnd)  return ret;
	::SendMessage(priceHwnd, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)strPrice);

	HWND amountHwnd = NULL;
	amountHwnd = FindWindowChildEx(sellHwnd, amountHwnd, _T("Edit"), NULL, 3);
	if (!amountHwnd)  return ret;
	::SendMessage(amountHwnd, WM_SETTEXT, 10, (LPARAM)(LPCTSTR)strAmount);

	HWND confirmHwnd = ::FindWindowEx(sellHwnd, NULL, NULL, _T("卖出"));
	if (confirmHwnd == NULL) confirmHwnd = ::FindWindowEx(sellHwnd, NULL, NULL, _T("卖出[S]"));
	if (!confirmHwnd) return ret;

	::SendMessage(confirmHwnd, BM_CLICK, 0, 0);
	return true;
}

HWND CHexin::GetBuyStockHwnd()
{
	HWND ret = NULL;
	HWND child_hwnd = NULL;	
	HWND buyStockBtn_hwnd = NULL;

	GetHexinHwnd();
	GetAfxMDIFrame42sHwnd();

	if (!hAfxMDIFrame42s) return ret;

	while (true) {

		child_hwnd = ::FindWindowEx(hAfxMDIFrame42s, child_hwnd, _T("#32770"), NULL);
		if (!child_hwnd) break;
		
		buyStockBtn_hwnd = ::FindWindowEx(child_hwnd, NULL, _T("Static"), _T("买入价格"));
		if (buyStockBtn_hwnd) {
			HWND sellStockBtn_hwnd = ::FindWindowEx(child_hwnd, NULL, _T("Static"), _T("卖出价格"));
			if (sellStockBtn_hwnd) continue;
			else { hBuyStock = child_hwnd; break;}
		}	
	}

	if (!hBuyStock) return ret;
	else return hBuyStock;
}

HWND CHexin::GetSellStockHwnd()
{
	HWND ret = NULL;
	HWND child_hwnd = NULL;	
	HWND sellStockBtn_hwnd = NULL;

	GetHexinHwnd();
	GetAfxMDIFrame42sHwnd();

	if (!hAfxMDIFrame42s) return ret;

	while (true) {

		child_hwnd = ::FindWindowEx(hAfxMDIFrame42s, child_hwnd, _T("#32770"), NULL);
		if (!child_hwnd) break;

		sellStockBtn_hwnd = ::FindWindowEx(child_hwnd, NULL, _T("Static"), _T("卖出价格"));
		if (sellStockBtn_hwnd) {
			HWND buyStockBtn_hwnd = ::FindWindowEx(child_hwnd, NULL, _T("Static"), _T("买入价格"));
			if (buyStockBtn_hwnd) continue;
			else { hSellStock = child_hwnd; break; }
		}
	}

	if (!hSellStock) return ret;
	else return hSellStock;
}

bool CHexin::KeepHexinActivate()
{
	bool ret = false;
	if (!hHexin) return ret;

	SendVirtualKey(hHexin, VK_F1);	
	Sleep(100);
	SendVirtualKey(hHexin, VK_F2);
	Sleep(100);
	SendVirtualKey(hHexin, VK_F4);	
	Sleep(100);
	SendVirtualKey(hHexin, VK_F1);
	Sleep(100);

	return true;
}


int CHexin::GetBidAmount()
{
	USES_CONVERSION;
	int ret = 0;
	wchar_t szText[20];
	HWND hwnd_amount = NULL;		
	hwnd_amount = FindWindowChildEx(hBuyStock, NULL, _T("Edit"), NULL, 3);
	::SendMessage(hwnd_amount, WM_GETTEXT, 10, LPARAM(szText));
	ret = atoi( T2A( CString(szText).GetBuffer()) );
	return ret;
}

int CHexin::GetAskAmount()
{
	USES_CONVERSION;
	int ret = 0;
	wchar_t szText[20];
	HWND hwnd_amount = NULL;	
	hwnd_amount = FindWindowChildEx(hSellStock, NULL, _T("Edit"), NULL, 3);
	::SendMessage(hwnd_amount, WM_GETTEXT, 10, LPARAM(szText));
	ret = atoi(T2A(CString(szText).GetBuffer()));
	return ret;
}

int CHexin::HexinWarnBuy(int amount)
{
	
	bool ret = false;

	CString strPrice;
	strPrice.Format(_T("%f"), GetHighLimit());
	
	CString strAmount;
	strAmount.Format(_T("%d"), amount);

	HWND priceHwnd = NULL;	
	priceHwnd = FindWindowChildEx(hBuyStock, NULL, _T("Edit"), NULL, 2);
	if (!priceHwnd)  return ret;
	::SendMessage(priceHwnd, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)strPrice);

	HWND amountHwnd = NULL;
	amountHwnd = FindWindowChildEx(hBuyStock, NULL, _T("Edit"), NULL, 3);
	if (!amountHwnd)  return ret;
	::SendMessage(amountHwnd, WM_SETTEXT, 10, (LPARAM)(LPCTSTR)strAmount);

	//同花顺版本不同 可能是 买入或者买入B
	HWND confirmHwnd = NULL;
	confirmHwnd = ::FindWindowEx(hBuyStock, NULL, NULL, _T("买入"));
	if (!confirmHwnd) confirmHwnd = ::FindWindowEx(hBuyStock, NULL, NULL, _T("买入[B]"));
	if (!confirmHwnd) return ret;

	::SendMessage(confirmHwnd, BM_CLICK, 0, 0);
	return true;

}

int CHexin::HexinWarnSell(int amount)
{
	bool ret = false;

	CString strPrice;
	strPrice.Format(_T("%f"), GetLowLimit());

	CString strAmount;
	strAmount.Format(_T("%d"), amount);

	HWND priceHwnd = NULL;	
	priceHwnd = FindWindowChildEx(hSellStock, NULL, _T("Edit"), NULL, 2);
	if (!priceHwnd)  return ret;
	::SendMessage(priceHwnd, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)strPrice);

	HWND amountHwnd = NULL;
	amountHwnd = FindWindowChildEx(hSellStock, amountHwnd, _T("Edit"), NULL, 3);
	if (!amountHwnd)  return ret;
	::SendMessage(amountHwnd, WM_SETTEXT, 10, (LPARAM)(LPCTSTR)strAmount);

	HWND confirmHwnd = ::FindWindowEx(hSellStock, NULL, NULL, _T("卖出"));
	if (confirmHwnd == NULL) confirmHwnd = ::FindWindowEx(hSellStock, NULL, NULL, _T("卖出[S]"));
	if (!confirmHwnd) return ret;

	::SendMessage(confirmHwnd, BM_CLICK, 0, 0);
	return true;
}

double CHexin::GetHighLimit()
{
	USES_CONVERSION;
	double ret = 0;
	wchar_t szText[20];
	HWND hwnd_grid = ::FindWindowEx(hBuyStock, NULL, _T("#32770"), NULL);
	HWND hwnd_highlimit = FindWindowChildEx(hwnd_grid, NULL, NULL, NULL, 12);
	::SendMessage(hwnd_highlimit, WM_GETTEXT, 10, LPARAM(szText));
	return atof(T2A(CString(szText)));

}

double CHexin::GetLowLimit()
{
	USES_CONVERSION;
	double ret = 0;
	wchar_t szText[20];

	HWND hwnd_grid = ::FindWindowEx(hSellStock, NULL, _T("#32770"), NULL);
	HWND hwnd_lowlimit = FindWindowChildEx(hwnd_grid, NULL, NULL, NULL, 25);
	::SendMessage(hwnd_lowlimit, WM_GETTEXT, 10, LPARAM(szText));
	return atof(T2A(CString(szText)));

}

int CHexin::GetCanBidAmount()
{
	USES_CONVERSION;
	wchar_t szText[20];
	HWND hwnd_canBuy = FindWindowChildEx(hBuyStock, NULL, _T("Static"), NULL, 2);
	::SendMessage(hwnd_canBuy, WM_GETTEXT, 10, LPARAM(szText));
	return atoi( T2A(CString(szText).GetBuffer()));
}

int CHexin::GetCanAskAmount()
{
	USES_CONVERSION;
	wchar_t szText[20];
	HWND hwnd_canSell = FindWindowChildEx(hSellStock, NULL, _T("Static"), NULL, 2);
	::SendMessage(hwnd_canSell, WM_GETTEXT, 10, LPARAM(szText));
	return atoi(T2A(CString(szText).GetBuffer()));
}


CString CHexin::GetStockCodeBuy()
{
	USES_CONVERSION;
	wchar_t szText[20];
	HWND hwnd_code = ::FindWindowEx(hBuyStock, NULL, _T("Edit"), NULL);
	::SendMessage(hwnd_code, WM_GETTEXT, 10, LPARAM(szText));
	return CString(szText);
}

CString CHexin::GetStockNameBuy()
{
	USES_CONVERSION;
	wchar_t szText[20];
	HWND hwnd_name = ::FindWindowEx(hBuyStock, NULL, _T("Static"), NULL);
	::SendMessage(hwnd_name, WM_GETTEXT, 10, LPARAM(szText));
	return CString(szText);
}

CString CHexin::GetStockCodeSell()
{
	USES_CONVERSION;
	wchar_t szText[20];
	HWND hwnd_code = ::FindWindowEx(hSellStock, NULL, _T("Edit"), NULL);
	::SendMessage(hwnd_code, WM_GETTEXT, 10, LPARAM(szText));
	return CString(szText);
}

CString CHexin::GetStockNameSell()
{
	USES_CONVERSION;
	wchar_t szText[20];
	HWND hwnd_name = ::FindWindowEx(hSellStock, NULL, _T("Static"), NULL);
	::SendMessage(hwnd_name, WM_GETTEXT, 10, LPARAM(szText));
	return CString(szText);
}

bool CHexin::WaitForDone(HWND hWndCtrl, enum WaitForClass nClass, char *szValue, int nInterval, int nTimeout)
{
	clock_t t1, t2;
	t1 = clock() / (CLOCKS_PER_SEC / 1000);

	char szTemp[MAX_PATH];
	while (true) {
		t2 = clock() / (CLOCKS_PER_SEC / 1000);
		if (t2 - t1 > nTimeout) break;

		Sleep(nInterval);

		if (nClass == WFC_Windows_Enabled) {
			if (::IsWindowEnabled(hWndCtrl) == TRUE) return true;
		}
		else if (nClass == WFC_Windows_Disabled) {
			if (::IsWindowEnabled(hWndCtrl) == FALSE) return true;
		}
		else if (nClass == WFC_Windows_Visible) {
			if (::IsWindowVisible(hWndCtrl) == TRUE) return true;
		}
		else if (nClass == WFC_Windows_Hidden) {
			if (::IsWindowVisible(hWndCtrl) == FALSE) return true;
		}
		else if (nClass == WFC_TextBox_IsEmpty) {
			::SendMessage(hWndCtrl, WM_GETTEXT, MAX_PATH, LPARAM(szTemp));
			if (strlen(szTemp) == 0) return true;
		}
		else if (nClass == WFC_TextBox_NotEmpty) {
			::SendMessage(hWndCtrl, WM_GETTEXT, MAX_PATH, LPARAM(szTemp));
			if (strlen(szTemp) > 0) return true;
		}
		else if (nClass == WFC_ComboBox_LeftMatch) {
			int nIndex = ::SendMessage(hWndCtrl, CB_GETCURSEL, 0, 0);
			::SendMessage(hWndCtrl, CB_GETLBTEXT, nIndex, LPARAM(szTemp));
			if (strncmp(szTemp, szValue, strlen(szValue)) == 0) return true;
		}
		else
			break;
	}

	return false;
}

HWND CHexin::GetHexinHwnd()
{
	hHexin = ::FindWindowEx(NULL, NULL, NULL, _T("网上股票交易系统5.0"));
	if (hHexin) return hHexin;
	else return NULL;
}

HWND CHexin::GetToolBarHolderHwnd()
{
	HWND hToolBar = ::FindWindowEx(hHexin, NULL, _T("ToolbarWindow32"), NULL);
	if (!hToolBar) return hToolBar;
	HWND hToolBarBox = ::FindWindowEx(hToolBar, NULL, _T("#32770"), NULL);
	if (!hToolBarBox) return hToolBarBox;
	hToolBarMarket = ::FindWindowEx(hToolBarBox, 0, _T("ComboBox"), NULL);
	if (!hToolBarMarket) return hToolBarMarket;
	hToolBarHolder = ::FindWindowEx(hToolBarBox, hToolBarMarket, _T("ComboBox"), NULL);
	if (!hToolBarHolder) return hToolBarHolder;
	return hToolBarHolder;
}

HWND CHexin::GetToolBarMarketHwnd()
{
	HWND hToolBar = ::FindWindowEx(hHexin, NULL, _T("ToolbarWindow32"), NULL);
	if (!hToolBar) return hToolBar;
	HWND hToolBarBox = ::FindWindowEx(hToolBar, NULL, _T("#32770"), NULL);
	if (!hToolBarBox) return hToolBarBox;
	hToolBarMarket = ::FindWindowEx(hToolBarBox, 0, _T("ComboBox"), NULL);
	if (!hToolBarMarket) return hToolBarMarket;
	return hToolBarMarket;
}

HWND CHexin::GetAfxMDIFrame42sHwnd()
{
	hAfxMDIFrame42s = ::FindWindowEx(hHexin, NULL, _T("AfxMDIFrame42s"), NULL);
	if (hAfxMDIFrame42s) return hAfxMDIFrame42s;
	else return NULL;
}

HWND CHexin::GetPositionHwnd()
{
	wchar_t szText[50];
	HWND hwnd_Position = NULL;
	HWND hwnd_Static = NULL;
	while (true) {
		hwnd_Position = ::FindWindowEx(hAfxMDIFrame42s, hwnd_Position, _T("#32770"), NULL);
		if (!hwnd_Position) return NULL;
		hwnd_Static = ::FindWindowEx(hwnd_Position, NULL, _T("Static"), _T("查询资金股票"));
		if (!hwnd_Static) continue;
		else return hwnd_Position;
	}
	return NULL;
}

HWND CHexin::GetToolBarBoxHwnd()
{
	HWND hToolBar = ::FindWindowEx(hHexin, NULL, _T("ToolbarWindow32"), NULL);
	if (!hToolBar) return hToolBar;
	hToolBarBox = ::FindWindowEx(hToolBar, NULL, _T("#32770"), NULL);
	if (!hToolBarBox) return hToolBarBox;
	return hToolBarBox;
}

bool CHexin::GetAllHwnd()
{
	bool ret = false;
	if (!(hHexin=GetHexinHwnd())) return ret;
	if (!(hAfxMDIFrame42s = GetAfxMDIFrame42sHwnd())) return ret;
	if (!(hPosition = GetPositionHwnd())) return ret;
	if (!(hBuyStock = GetBuyStockHwnd())) return ret;
	if (!(hSellStock = GetSellStockHwnd())) return ret;
	if (!(hToolBarBox = GetToolBarBoxHwnd())) return ret;
	if (!(hToolBarMarket = GetToolBarHolderHwnd())) return ret;
	if (!(hToolBarHolder = GetToolBarMarketHwnd())) return ret;

	return true;
}

void CHexin::TradeWndRefresh()
{
	
	if (hHexin && !::IsWindow(hHexin) ||
		hPosition && !::IsWindow(hPosition) ||
		hBuyStock && !::IsWindow(hBuyStock) ||
		hSellStock && !::IsWindow(hSellStock))
	{
		KeepHexinActivate();
		GetAllHwnd();
	}
				

	return;
}

