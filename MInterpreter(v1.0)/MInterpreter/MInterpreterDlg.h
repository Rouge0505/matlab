
// MInterpreterDlg.h : 头文件
//

#pragma once
#include "MInterpreterBE.h"
#include "ReadOnlyEdit.h"

// CMInterpreterDlg 对话框
class CMInterpreterDlg : public CDialogEx
{
// 构造
public:
	CMInterpreterDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MINTERPRETER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CMInterpreterBE m_interpreter;			//matlab解释器
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void getExePath();							//获取exe所在路径
	void initWorkspace();						//初始化工作区
	//void initWorkPath();						//初始化工作路径
	void setWorkPath(const TCHAR* workPath);	//设置工作路径
	void initDlgSize();							//记录对话框初始大小
	LRESULT updateWorkspace(WPARAM wParam, LPARAM lParam);	//在工作空间显示数据
	LRESULT showCmdWindow(WPARAM wParam, LPARAM lParam);	//在命令窗口打印数据
	LRESULT handleWindowCmd(WPARAM wParam, LPARAM lParam);	//响应窗口命令
public:
	afx_msg void OnBnClickedButtonSelectPath();			//选择工作路径
	BOOL listFilesInDirectory(const TCHAR* directory);	//查找一个路径下的所有文件（含文件夹 . .. 隐藏文件）
	afx_msg void OnLbnDblclkListFiles();
	void setHScroll(CListBox* pList);					//设置LIST BOX的水平滚动条
	//void setFullScreen();
	void resize();										//控件适应对话框大小
	POINT m_oldSize;
	BOOL m_bKeyDown;	//初始为FALSE，按下按键时置TRUE，弹起时置FALSE
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonRun();			//运行，解释M文件
	void interpretCmdLine();						//交互式解释命令行语句
	void appendString2History(const TCHAR* newContent);	//在历史窗口后添加字符串
private:
	TCHAR m_exePath[MAX_PATH];						//exe所在路径
	TCHAR m_workPath[MAX_PATH];						//工作路径
	CReadOnlyEdit m_history;						//历史窗口
	CReadOnlyEdit m_cmdLine;						//命令行
	CFont m_font;									//给上面两个只读编辑框用
private:
	void initROEdits();								//初始化只读编辑框
};
