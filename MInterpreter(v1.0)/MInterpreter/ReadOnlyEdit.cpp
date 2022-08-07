// ReadOnlyEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "ReadOnlyEdit.h"


// CReadOnlyEdit

IMPLEMENT_DYNAMIC(CReadOnlyEdit, CEdit)

CReadOnlyEdit::CReadOnlyEdit()
{
	m_readOnlyCharNum = 0x7FFFFFFF;
}

CReadOnlyEdit::~CReadOnlyEdit()
{
}

BOOL CReadOnlyEdit::CreateScale(DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID, float scale)
{
	CRect rectScale;
	rectScale.bottom = rect.bottom * scale;
	rectScale.top = rect.top * scale;
	rectScale.left = rect.left * scale;
	rectScale.right = rect.right * scale;
	return Create(dwStyle, rectScale, pParentWnd, nID);
}

void CReadOnlyEdit::setReadOnlyNum(int readOnlyCharNum)
{
	m_readOnlyCharNum = readOnlyCharNum;
}


BEGIN_MESSAGE_MAP(CReadOnlyEdit, CEdit)
END_MESSAGE_MAP()



// CReadOnlyEdit 消息处理程序




BOOL CReadOnlyEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//首先明确光标位置，如a|bc|d，两竖杠分别代表开始符nStart和结束符nEnd，则nStart=1和nEnd=3，即光标位置为前面字符数
	//然后明确所有需要特殊处理的操作：(WM_KEYDOWN消息下)删除，退格，剪切，粘贴，上下左右之外的其他按键，即撤销外所有写操作
	//注意到其中只有重合退格（开始结束位置相同）能改变开始符之前的值，其他均只会改变开始符之后的值
	//下面简记m_readOnlyCharNum为num，示例均取num=2
	//对于重合退格，光标位置≤num时返回，如01光标2，否则将删掉字符1
	//除此之外可以统一操作
	//nStart≥num，随意操作，如01开始符2，但是重合退格就不行，所以排除在外单独处理
	//nStart＜num，nEnd≥num，令nStart=num，如01结束符2，令nStart=2，01开始结束2，就可以任意操作了
	//nStart＜num，禁止操作直接返回，如0结束符12，操作必然会改变只读字符
	
	//算法：判nStart＜num?否的话不操作，是的话判nEnd＜num?是的话直接返回，否的话设置开始符
	if (pMsg->message == WM_KEYDOWN)
	{
		WPARAM wParam = 0;	//如果为重合退格，该值为VK_BACK；如果为其他写操作，该值为VK_DELETE；否则为0
		int nStart = 0;
		int nEnd = 0;
		GetSel(nStart, nEnd);
		switch (pMsg->wParam)
		{
		case VK_DELETE:
			wParam = VK_DELETE;
			break;
		case VK_BACK:
			if (nStart == nEnd)
				wParam = VK_BACK;
			else
			{
				wParam = VK_DELETE;
				pMsg->wParam = VK_DELETE;	//否则将开始符置为num，结束符又恰为num，会删除保护字符
			}
				
			break;
		default:
			if (GetKeyState(VK_CONTROL) & 0x8000)	//Ctrl按下
			{
				switch (pMsg->wParam)
				{
				case 'X':	//剪切
				case 'V':	//粘贴
					wParam = VK_DELETE;
					break;
				}
			}
			else
			{
				switch (pMsg->wParam)
				{
				case VK_UP:
				case VK_DOWN:
				case VK_LEFT:
				case VK_RIGHT:
					break;
				default:
					wParam = VK_DELETE;
					break;
				}
			}
			break;
		}

		switch (wParam)
		{
		case VK_DELETE:
			//算法：判nStart＜num?否的话不操作，是的话判nEnd＜num?是的话直接返回，否的话设置开始符
			if (nStart < m_readOnlyCharNum)
			{
				if (nEnd < m_readOnlyCharNum)
					return TRUE;
				SetSel(m_readOnlyCharNum, nEnd);
				//SetSel((DWORD)2, (DWORD)3);
				//GetSel(nStart, nEnd);
			}
			break;
		case VK_BACK:
			//对于重合退格，光标位置≤num时返回，如01光标2，否则将删掉字符1
			if (nStart <= m_readOnlyCharNum)
				return TRUE;
			break;
		default:
			break;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}
