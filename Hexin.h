/*
	项目中所用到的核心类
	用于对同花顺下单程序做交互处理
	配合使用的同花顺版本：同花顺同一版下单客户端 (5,18,92,119)

*/

#pragma once

#include "stdafx.h"
#include "struct_.h"

class CHexin
{
public:

	CHexin();
	~CHexin();

public:	

	static HWND FindWindowChildEx(HWND hwndParent, HWND hwndChild, LPCTSTR lpszClass, LPCTSTR lpszWindow, int count);
	static void SendVirtualKey(HWND hwnd, SHORT vk);
	void TradeWndRefresh();	

public:
	
	bool GetTradeBalance();	
	bool GetAllHwnd();

	HWND GetHexinHwnd();	
	HWND GetAfxMDIFrame42sHwnd();
	HWND GetPositionHwnd();
	HWND GetBuyStockHwnd();
	HWND GetSellStockHwnd();	
	HWND GetToolBarHolderHwnd();
	HWND GetToolBarBoxHwnd();	
	HWND GetToolBarMarketHwnd();

	bool KeepHexinActivate();
	TradeBalance& GetTradeBalance_();

	//同花顺预警功能
	CString GetStockCodeBuy();
	CString GetStockNameBuy();
	CString GetStockCodeSell();
	CString GetStockNameSell();

	int GetBidAmount();
	int GetAskAmount();
	int GetCanBidAmount();
	int GetCanAskAmount();
	double GetHighLimit();
	double GetLowLimit();
	
	int HexinWarnBuy(int amount);
	int HexinWarnSell(int amount);

	//闪电下单功能
	bool FlashBuyStock(CString strStock,CString strPrice,CString strAmount);
	bool FlashSellStock(CString strStock, CString strPrice, CString strAmount);
	bool WaitForDone(HWND hWndCtrl, enum WaitForClass nClass, char *szValue, int nInterval, int nTimeout);
	

private:

	TradeBalance m_balance;
	HWND hHexin = NULL;
	HWND hAfxMDIFrame42s = NULL;
	HWND hPosition = NULL;//持仓窗口
	HWND hBuyStock = NULL;//买股票窗口
	HWND hSellStock = NULL;//卖股票窗口
	HWND hToolBarBox = NULL;//顶部栏窗口
	HWND hToolBarMarket = NULL;//菜单栏市场框
	HWND hToolBarHolder = NULL;//菜单栏股东帐户框

	
};

