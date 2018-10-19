#pragma once


/*
	项目中所用到的结构
	2018_10_15
*/

//资金股票结构体
typedef struct TRADEBALANCE{
	double dFrozen;
	double dMarket;
	double dEnable;
	double dAsset;
	double dDayProfit;
	double dTotalProfit;
	double dCash;
	double dRemain;
}TradeBalance;


static void RestBalance(TradeBalance& balance) {
	balance.dAsset = 0;
	balance.dCash = 0;
	balance.dDayProfit = 0;
	balance.dEnable = 0;
	balance.dFrozen = 0;
	balance.dMarket = 0;
	balance.dRemain = 0;
	balance.dTotalProfit = 0;
}

enum WaitForClass {
	WFC_Windows_Enabled,
	WFC_Windows_Disabled,
	WFC_Windows_Visible,
	WFC_Windows_Hidden,
	WFC_TextBox_IsEmpty,
	WFC_TextBox_NotEmpty,
	WFC_ComboBox_LeftMatch,
};


