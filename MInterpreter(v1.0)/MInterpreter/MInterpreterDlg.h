
// MInterpreterDlg.h : ͷ�ļ�
//

#pragma once
#include "MInterpreterBE.h"
#include "ReadOnlyEdit.h"

// CMInterpreterDlg �Ի���
class CMInterpreterDlg : public CDialogEx
{
// ����
public:
	CMInterpreterDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MINTERPRETER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CMInterpreterBE m_interpreter;			//matlab������
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void getExePath();							//��ȡexe����·��
	void initWorkspace();						//��ʼ��������
	//void initWorkPath();						//��ʼ������·��
	void setWorkPath(const TCHAR* workPath);	//���ù���·��
	void initDlgSize();							//��¼�Ի����ʼ��С
	LRESULT updateWorkspace(WPARAM wParam, LPARAM lParam);	//�ڹ����ռ���ʾ����
	LRESULT showCmdWindow(WPARAM wParam, LPARAM lParam);	//������ڴ�ӡ����
	LRESULT handleWindowCmd(WPARAM wParam, LPARAM lParam);	//��Ӧ��������
public:
	afx_msg void OnBnClickedButtonSelectPath();			//ѡ����·��
	BOOL listFilesInDirectory(const TCHAR* directory);	//����һ��·���µ������ļ������ļ��� . .. �����ļ���
	afx_msg void OnLbnDblclkListFiles();
	void setHScroll(CListBox* pList);					//����LIST BOX��ˮƽ������
	//void setFullScreen();
	void resize();										//�ؼ���Ӧ�Ի����С
	POINT m_oldSize;
	BOOL m_bKeyDown;	//��ʼΪFALSE�����°���ʱ��TRUE������ʱ��FALSE
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonRun();			//���У�����M�ļ�
	void interpretCmdLine();						//����ʽ�������������
	void appendString2History(const TCHAR* newContent);	//����ʷ���ں�����ַ���
private:
	TCHAR m_exePath[MAX_PATH];						//exe����·��
	TCHAR m_workPath[MAX_PATH];						//����·��
	CReadOnlyEdit m_history;						//��ʷ����
	CReadOnlyEdit m_cmdLine;						//������
	CFont m_font;									//����������ֻ���༭����
private:
	void initROEdits();								//��ʼ��ֻ���༭��
};
