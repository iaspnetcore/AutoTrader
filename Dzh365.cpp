#include "stdafx.h"
#include "Dzh365.h"
#include <dbghelp.h>
#pragma comment(lib , "Dbghelp.lib")

CDzh365::CDzh365()
{

}


CDzh365::~CDzh365()
{

}

HWND CDzh365::GetDZHHwnd(HWND* hwnd)
{
	HWND DesktopHwnd = ::GetDesktopWindow();
	EnumChildWindows(DesktopHwnd, EnumChildProc, LPARAM(hwnd));
	return *hwnd;
}

BOOL CDzh365::EnumChildProc(HWND hwndChild, LPARAM lParam)
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

bool CDzh365::GetDZHHwnd()
{
	GetDZHHwnd(&m_hwnd);
	if (!m_hwnd) return false;
	else return true;
}

CString CDzh365::GetStockCode()
{
	char code[7];
	ReadProcessMemory(m_dzhHandle, (LPCVOID)m_codeAddr, code, 6, NULL);
	code[6] = '\0';
	CString temp = CString(code);
	return temp;
}

BOOL CDzh365::MyEnumerateLoadedModulesProc64(PTSTR ModuleName,
	DWORD64 ModuleBase,
	ULONG ModuleSize,
	PVOID UserContext)
{
	DWORD64 addr;
	addr = ModuleBase + 0x104C2C2;
	*(DWORD64*)UserContext = addr;	
	return false;
}

DWORD CDzh365::GetBaseAddr()
{
	DWORD pid;
	GetWindowThreadProcessId(m_hwnd, &pid);//进程id
	if (!pid) return 0;
	
	m_dzhHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	if (m_dzhHandle == NULL) {
		AfxMessageBox(_T("未找到大智慧进程"), 0, 0);
		return 0;
	}
	EnumerateLoadedModules64(m_dzhHandle,
		(PENUMLOADED_MODULES_CALLBACK64)MyEnumerateLoadedModulesProc64,
		(PVOID)&m_codeAddr);
}