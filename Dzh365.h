/*
	获取大智慧365中正在浏览的股票代码
	2018_10_16
*/

#pragma once
class CDzh365
{
public:
	CDzh365();
	~CDzh365();

public:

	static BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam);
	static BOOL CALLBACK MyEnumerateLoadedModulesProc64(PTSTR ModuleName,
														DWORD64 ModuleBase,
														ULONG ModuleSize,
														PVOID UserContext);
public:

	bool GetDZHHwnd();
	HWND GetDZHHwnd(HWND* hwnd);
	CString GetStockCode();
	DWORD GetBaseAddr();

private:

	HWND m_hwnd = NULL;
	DWORD64 m_codeAddr = NULL;
	HANDLE m_dzhHandle = NULL;
};

