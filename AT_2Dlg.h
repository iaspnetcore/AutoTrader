
// AT_2Dlg.h : 头文件
//

/*
	2018_10_18
	
*/


#pragma once
#include "struct_.h"
#include "Hexin.h"
#include "Dzh365.h"
#include "afxcmn.h"
#include "afxdtctl.h"
#include "DlgFlashBuy.h"
#include "afxwin.h"
#include <memory>
#include <map>

// CAT_2Dlg 对话框
class CAT_2Dlg : public CDialog
{
// 构造
public:
	CAT_2Dlg(CWnd* pParent = NULL);	// 标准构造函数
	~CAT_2Dlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AT_2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


public:

	static void UpdateBalance(LPVOID lp);
	static void StartHexinWarnBuy(LPVOID lp);
	static void StartHexinWarnSell(LPVOID lp);
	static void KeepHexinActivate(LPVOID lp);
	static void CallBack_FlushBuy(CString code, int amount);
	static void GetHighLimit(LPVOID lp);//从服务器获取涨停价格表


public:

	
	void InitialParam();	
	void InitialAutoTrader2();
	void UpdateBuySellCount();
	void UpdateBalance_once();
	void GetBalance(TradeBalance& balance);
	void Recort(int amount_, CString oper);
	void Recort(CString code , CString name ,  CString oper , CString amount , CString price);
	void DivString(CStringArray& array, CString& test);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();	
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	DECLARE_MESSAGE_MAP()	

private:


	
	CFile m_file;
	CHexin m_hexin;
	CDzh365 m_dzh365;
	SOCKET m_socketClient;	
	TradeBalance m_balance;
	std::map<CString, CString> m_highLimit;
	std::shared_ptr<CDlgFlashBuy> m_flashBuy;	

	bool m_warnStart = false;
	bool m_hexinWarnBuy = false;
	bool m_hexinWarnSell = false;
	bool m_hexinKeepActivate = false;
	bool m_hexinUpdateBalance = false;	

	int m_nBuyCount;
	int m_nSellCount;	
	int m_nBuyAmount;
	int m_nSellAmount;	
	int second_buyStop;
	int second_sellStop;	
	int second_buyStart;
	int second_sellStart;	

public:

	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnFlush();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);	

	CStatic m_labEnable;
	CStatic m_labAsset;
	CListCtrl m_listCtrl;
	CDateTimeCtrl m_time1;
	CDateTimeCtrl m_time2;
	CDateTimeCtrl m_time3;
	CDateTimeCtrl m_time4;
	
	
	
	
	
};
