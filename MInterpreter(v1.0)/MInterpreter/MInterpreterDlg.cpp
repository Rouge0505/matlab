
// MInterpreterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MInterpreter.h"
#include "MInterpreterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMInterpreterDlg 对话框



CMInterpreterDlg::CMInterpreterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MINTERPRETER_DIALOG, pParent)
	, m_bKeyDown(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMInterpreterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMInterpreterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_UPDATE_WORKSPACE, updateWorkspace)
	ON_MESSAGE(WM_SHOW_CMDWND, showCmdWindow)
	ON_MESSAGE(WM_WINDOW_CMD, handleWindowCmd)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_PATH, &CMInterpreterDlg::OnBnClickedButtonSelectPath)
	ON_LBN_DBLCLK(IDC_LIST_FILES, &CMInterpreterDlg::OnLbnDblclkListFiles)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CMInterpreterDlg::OnBnClickedButtonRun)
END_MESSAGE_MAP()


// CMInterpreterDlg 消息处理程序

BOOL CMInterpreterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	getExePath();
	initWorkspace();
	//initWorkPath();
	//setWorkPath(_T("C:\\Windows\\System32"));
	setWorkPath(m_exePath);
	
	//setWorkPath(_T("E:\\VS2015 Projects\\MInterpreter\\MInterpreter"));
	initDlgSize();
	initROEdits();
	

	


	//GetDlgItem(IDC_EDIT_CMDLN)->SetWindowText(_T(">>"));
	ShowWindow(SW_MAXIMIZE);
	//setFullScreen();

	//m_interpreter.open(_T("E:\\VS2015 Projects\\MInterpreter\\MInterpreter\\test.m"));
	//statusList result = (statusList)m_interpreter.run();
	//if (result != _STATUS_CALCULATE_SUCCESS_)
	//{
	//	MessageBox(_T("解释失败！"));
	//}
	//CString str(_T("123"));
	//TCHAR str1[] = _T("123");
	//int res = str.Compare(str1);

	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMInterpreterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMInterpreterDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMInterpreterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMInterpreterDlg::getExePath()
{
	GetModuleFileName(NULL, m_exePath, MAX_PATH);
	TCHAR* ptr = _tcsrchr(m_exePath, '\\');
	if (!ptr)
		ptr = _tcsrchr(m_exePath, '/');
	*ptr = 0;
}

void CMInterpreterDlg::initWorkspace()
{
	CListCtrl* pWorkspace = (CListCtrl*)GetDlgItem(IDC_LIST_WORKSPACE);
	if (!pWorkspace)
		return;
	pWorkspace->InsertColumn(0, _T("名称"));
	pWorkspace->InsertColumn(1, _T("值"));
	RECT rect;
	pWorkspace->GetWindowRect(&rect);
	int windowWidth = rect.right - rect.left;
	pWorkspace->SetColumnWidth(0, windowWidth / 2);
	pWorkspace->SetColumnWidth(1, windowWidth / 2);
	pWorkspace->SetExtendedStyle(pWorkspace->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	pWorkspace->SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	//pWorkspace->InsertItem(0, _T("ans"));
	//pWorkspace->SetItemText(0, 1, _T("0"));
	//CString valueStr;
	//valueStr.Format(_T("%f"), 1.0);
	//pWorkspace->SetItemText(0, 1, valueStr);
	//pWorkspace->DeleteAllItems();
}

//void CMInterpreterDlg::initWorkPath()
//{
//	TCHAR workPath[MAX_PATH] = _T("C:\\Windows\\System32");
//	GetDlgItem(IDC_EDIT_PATH)->SetWindowText(workPath);
//	listFilesInDirectory(workPath);
//}

void CMInterpreterDlg::setWorkPath(const TCHAR * workPath)
{
	_tcscpy(m_workPath, workPath);
	GetDlgItem(IDC_EDIT_PATH)->SetWindowText(workPath);
	listFilesInDirectory(workPath);
}

void CMInterpreterDlg::initDlgSize()
{
	CRect rect;
	GetClientRect(&rect);			//仅为客户区大小
	m_oldSize.x = rect.right - rect.left;
	m_oldSize.y = rect.bottom - rect.top;
}

LRESULT CMInterpreterDlg::updateWorkspace(WPARAM wParam, LPARAM lParam)
{
	CListCtrl* pWorkspace = (CListCtrl*)GetDlgItem(IDC_LIST_WORKSPACE);
	if (!pWorkspace)
		return 1;

	const char* chName = (const char*)wParam;
	double value = *(double*)lParam;
	CString valueStr;
	valueStr.Format(_T("%f"), value);
	TCHAR tcName[128] = { 0 };
	_tcscpy(tcName, CA2T(chName));
	TCHAR temp[128] = { 0 };

	int count = pWorkspace->GetItemCount();
	int i = 0;
	for (i = 0; i < count; i++)
	{
		CString str = pWorkspace->GetItemText(i, 0);
		if (str.Compare(tcName) == 0)
		{
			pWorkspace->SetItemText(i, 1, valueStr);
			break;
		}
	}
	if (i == count)
	{
		pWorkspace->InsertItem(i, tcName);
		pWorkspace->SetItemText(i, 1, valueStr);
	}
	//pWorkspace->get


	return LRESULT();
}

LRESULT CMInterpreterDlg::showCmdWindow(WPARAM wParam, LPARAM lParam)
{
	CString strOld;
	m_history.GetWindowText(strOld);
	CString str2add;
	//str2add.Format(_T("%s =\n\n     %f"), CA2T((char*)wParam), *(double*)lParam);
	//上面不可以

	TCHAR name[32] = { 0 };
	_tcscpy(name, CA2T((char*)wParam));
	double value = *(double*)lParam;

	str2add.Format(_T("%s =\r\n\r\n      %lf\r\n\r\n"), name, value);
	appendString2History(str2add);
	return LRESULT();
}

LRESULT CMInterpreterDlg::handleWindowCmd(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case _TOKEN_CLEAR_:
	{
		CListCtrl* pWorkspace = (CListCtrl*)GetDlgItem(IDC_LIST_WORKSPACE);
		if (pWorkspace)
			pWorkspace->DeleteAllItems();
	}
		break;
	case _TOKEN_CLC_:
	{
		m_history.SetWindowText(_T(""));
	}
		break;
	}
	return LRESULT();
}



void CMInterpreterDlg::OnBnClickedButtonSelectPath()
{
	// TODO: 在此添加控件通知处理程序代码
	BROWSEINFO sInfo = { 0 };
	sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_EDITBOX;
	LPITEMIDLIST lpidlBrowse = SHBrowseForFolder(&sInfo);
	TCHAR path[MAX_PATH] = { 0 };
	if (lpidlBrowse)
	{
		if (SHGetPathFromIDList(lpidlBrowse, path))
			setWorkPath(path);
			
		CoTaskMemFree(lpidlBrowse);
	}
}

BOOL CMInterpreterDlg::listFilesInDirectory(const TCHAR * directory)
{
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_FILES);
	if (!pList)
		return FALSE;
	pList->ResetContent();
	WIN32_FIND_DATA findData = { 0 };
	TCHAR szFind[MAX_PATH] = { 0 };
	_stprintf(szFind, _T("%s\\*.*"), directory);
	BOOL bRet = TRUE;
	HANDLE hFind = ::FindFirstFile(szFind, &findData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		DWORD err = GetLastError();
		return FALSE;
	}

	while (bRet)
	{
		pList->AddString(findData.cFileName);
		bRet = ::FindNextFile(hFind, &findData);
	}
	::FindClose(hFind);
	setHScroll(pList);
	return TRUE;
}


void CMInterpreterDlg::OnLbnDblclkListFiles()	//读取文件
{
	// TODO: 在此添加控件通知处理程序代码
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_FILES);
	if (!pList)
		return;
	int sel = pList->GetCurSel();
	CString str;
	pList->GetText(sel, str);
	TCHAR workPath[MAX_PATH] = { 0 };
	GetDlgItem(IDC_EDIT_PATH)->GetWindowText(workPath, MAX_PATH);
	if (str.GetAt(0) == _T('.'))
	{
		if (str.GetAt(1) == _T('.'))
		{
			//const TCHAR* pTc = _tcsrchr(str, _T('\\'));
			for (int i = _tcslen(workPath); i >= 0; i--)
				if (workPath[i] == _T('\\') || workPath[i] == _T('/'))
				{
					workPath[i] = 0;
					setWorkPath(workPath);
					return;
				}
		}
		return;
	}
	
	TCHAR filePath[MAX_PATH] = { 0 };
	_stprintf(filePath, _T("%s\\%s"), workPath, str);
	if (GetFileAttributes(filePath) & FILE_ATTRIBUTE_DIRECTORY)
	{
		setWorkPath(filePath);
		return;
	}
	FILE* pFile = _tfopen(filePath, _T("rb"));
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
		long len = ftell(pFile);
		char* chContent = new char[len + 1];
		chContent[len] = 0;
		fseek(pFile, 0, SEEK_SET);
		size_t sRet = fread(chContent, len, 1, pFile);
		if (sRet)
		{
			GetDlgItem(IDC_EDIT_EDITOR)->SetWindowText(CA2T(chContent));
		}
		delete chContent;
		fclose(pFile);
	}
}

void CMInterpreterDlg::setHScroll(CListBox* pList)
{
	CDC* dc = GetDC();
	SIZE s;
	int i;
	TCHAR str[100];
	HDC hdc = dc->m_hDC; // 获得相关设备句柄
	for (i = 0; i < pList->GetCount(); i++)
	{
		pList->GetText(i, str);
		GetTextExtentPoint32(hdc, str, lstrlen(str), &s); // 获取字符串的像素大小
														  // 如果新的字符串宽度大于先前的水平滚动条宽度，则重新设置滚动条宽度
														  // IDC_LISTBOX为m_listBox的资源ID
		if (s.cx >(LONG)SendDlgItemMessage(IDC_LIST_FILES, LB_GETHORIZONTALEXTENT, 0, 0))
			SendDlgItemMessage(IDC_LIST_FILES, LB_SETHORIZONTALEXTENT, (WPARAM)s.cx, 0);
	}
	ReleaseDC(dc);
}

void CMInterpreterDlg::resize()
{
	POINT dlgSize;		//当前对话框大小
	CRect dlgRect;		//当前对话框坐标
	GetClientRect(&dlgRect);
	dlgSize.x = dlgRect.right - dlgRect.left;
	dlgSize.y = dlgRect.bottom - dlgRect.top;

	float xTimes = (float)dlgSize.x / m_oldSize.x;
	float yTimes = (float)dlgSize.y / m_oldSize.y;

	int ctrlID = 0;		//控件ID
	CRect ctrlRect;
	HWND hWnd = ::GetWindow(m_hWnd, GW_CHILD);	//列出所有控件
	while (hWnd)
	{
		ctrlID = ::GetDlgCtrlID(hWnd);			//获取控件ID
		CWnd* pWnd = GetDlgItem(ctrlID);
		pWnd->GetWindowRect(&ctrlRect);			//获取控件在屏幕中的位置
		ScreenToClient(&ctrlRect);				//转化为在对话框客户区（除去标题边框等）坐标系下的位置
		CPoint oldTL = ctrlRect.TopLeft();
		CPoint oldBR = ctrlRect.BottomRight();
		CPoint TL;
		CPoint BR;
		TL.SetPoint(oldTL.x * xTimes, oldTL.y * yTimes);
		BR.SetPoint(oldBR.x * xTimes, oldBR.y * yTimes);
		ctrlRect.SetRect(TL, BR);
		pWnd->MoveWindow(ctrlRect, TRUE);
		hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
	}
	m_oldSize = dlgSize;

}

//void CMInterpreterDlg::setFullScreen()
//{
//	CRect fullScreenRect = { 0 };
//	fullScreenRect.right = GetSystemMetrics(SM_CXSCREEN);
//	fullScreenRect.bottom = GetSystemMetrics(SM_CYSCREEN);
//
//	WINDOWPLACEMENT wndPl = { 0 };
//	wndPl.length = sizeof(WINDOWPLACEMENT);
//	wndPl.showCmd = SW_SHOWNORMAL;
//	wndPl.rcNormalPosition = fullScreenRect;
//
//	SetWindowPlacement(&wndPl);
//}


void CMInterpreterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
		resize();
}


BOOL CMInterpreterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			if (m_bKeyDown)	//保证只响应一次KEYDOWN
				return TRUE;
			m_bKeyDown = TRUE;
			switch (GetFocus()->GetDlgCtrlID())
			{
			case IDC_EDIT_PATH:								//设置工作路径
			{
				TCHAR workPath[MAX_PATH] = { 0 };
				GetDlgItem(IDC_EDIT_PATH)->GetWindowText(workPath, MAX_PATH);
				_tcscpy(m_workPath, workPath);
				listFilesInDirectory(workPath);
			}
				break;
			case IDC_ROEDIT_CMDLN:							//解释一行命令
				interpretCmdLine();
				break;
			}
			return TRUE;
		case VK_F5:
			OnBnClickedButtonRun();
			break;
		}

		

		break;
	case WM_KEYUP:
		m_bKeyDown = FALSE;
		break;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMInterpreterDlg::OnBnClickedButtonRun()
{
	// TODO: 在此添加控件通知处理程序代码
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_FILES);
	if (!pList)
		return;

	int sel = pList->GetCurSel();
	if (sel < 0)
	{
		MessageBox(_T("请选择文件"));
		return;
	}
	CString fileName;							//文件名
	pList->GetText(sel, fileName);

	TCHAR workPath[MAX_PATH] = { 0 };			//工作路径
	GetDlgItem(IDC_EDIT_PATH)->GetWindowText(workPath, MAX_PATH);

	


	CString str2add;
	str2add.Format(_T(">>%s\r\n\r\n"), fileName);
	appendString2History(str2add);


	

	if (!m_interpreter.open(workPath, fileName))
	{
		appendString2History(_T("打开文件失败！"));
		return;
	}
	statusList result = (statusList)m_interpreter.run();
	if (result != _STATUS_CALCULATE_SUCCESS_)
		appendString2History(m_interpreter.getErrStr());
}

void CMInterpreterDlg::interpretCmdLine()
{
	CString strCmdLine;										//命令行语句
	m_cmdLine.GetWindowText(strCmdLine);
	m_cmdLine.SetWindowText(_T(">>"));
	m_cmdLine.SetSel(2, 2);				//设置光标在最后


	CString strHistory;
	m_history.GetWindowText(strHistory);
	strHistory.Append(strCmdLine);
	strHistory.Append(_T("\r\n\r\n"));
	m_history.SetWindowText(strHistory);
	int textLen = strHistory.GetLength();				//设置光标在最后
	m_history.SetSel(textLen, textLen, 0);
		//pCmdWnd->SetFocus();

	m_interpreter.setStatement(CT2A(strCmdLine) + 2, m_workPath);
	statusList status = (statusList)m_interpreter.run();
	if (status != _STATUS_CALCULATE_SUCCESS_)
		appendString2History(m_interpreter.getErrStr());
}

void CMInterpreterDlg::appendString2History(const TCHAR * newContent)
{
	CString strOld;
	m_history.GetWindowText(strOld);
	strOld.Append(newContent);
	m_history.SetWindowText(strOld);
	int textLen = strOld.GetLength();				//设置光标在最后
	m_history.SetSel(textLen, textLen, 0);
	//pCmdWnd->SetFocus();
}

void CMInterpreterDlg::initROEdits()
{
	LOGFONT lf = { 0 };
	CFont* pFont = GetDlgItem(IDC_EDIT_PATH)->GetFont();
	pFont->GetLogFont(&lf);
	m_font.CreateFontIndirect(&lf);
	
	//命令行
	m_cmdLine.CreateScale(WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_WANTRETURN | ES_AUTOHSCROLL | WS_BORDER | WS_TABSTOP,
		CRect(168, 315, 168 + 380, 315 + 14), this, IDC_ROEDIT_CMDLN, 1.75);
	m_cmdLine.SetWindowText(_T(">>"));
	m_cmdLine.setReadOnlyNum(2);
	m_cmdLine.SetFont(&m_font);


	//历史窗口
	m_history.CreateScale(WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_WANTRETURN | ES_AUTOHSCROLL | WS_BORDER | WS_TABSTOP | WS_VSCROLL,
		CRect(168, 211, 168 + 380, 211 + 103), this, IDC_ROEDIT_HISTORY, 1.75);
	m_history.SetFont(&m_font);
}


