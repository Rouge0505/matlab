#pragma once


// CReadOnlyEdit

class CReadOnlyEdit : public CEdit
{
	DECLARE_DYNAMIC(CReadOnlyEdit)

public:
	CReadOnlyEdit();
	virtual ~CReadOnlyEdit();
	BOOL CreateScale(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, float scale = 1.0);
	void setReadOnlyNum(int readOnlyCharNum);

protected:
	DECLARE_MESSAGE_MAP()
private:
	int m_readOnlyCharNum;	//前多少个字符受保护，默认0x7FFFFFFF
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


