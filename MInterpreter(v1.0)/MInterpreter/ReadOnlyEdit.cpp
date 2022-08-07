// ReadOnlyEdit.cpp : ʵ���ļ�
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



// CReadOnlyEdit ��Ϣ�������




BOOL CReadOnlyEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	//������ȷ���λ�ã���a|bc|d�������ֱܷ����ʼ��nStart�ͽ�����nEnd����nStart=1��nEnd=3�������λ��Ϊǰ���ַ���
	//Ȼ����ȷ������Ҫ���⴦��Ĳ�����(WM_KEYDOWN��Ϣ��)ɾ�����˸񣬼��У�ճ������������֮�������������������������д����
	//ע�⵽����ֻ���غ��˸񣨿�ʼ����λ����ͬ���ܸı俪ʼ��֮ǰ��ֵ��������ֻ��ı俪ʼ��֮���ֵ
	//������m_readOnlyCharNumΪnum��ʾ����ȡnum=2
	//�����غ��˸񣬹��λ�á�numʱ���أ���01���2������ɾ���ַ�1
	//����֮�����ͳһ����
	//nStart��num�������������01��ʼ��2�������غ��˸�Ͳ��У������ų����ⵥ������
	//nStart��num��nEnd��num����nStart=num����01������2����nStart=2��01��ʼ����2���Ϳ������������
	//nStart��num����ֹ����ֱ�ӷ��أ���0������12��������Ȼ��ı�ֻ���ַ�
	
	//�㷨����nStart��num?��Ļ����������ǵĻ���nEnd��num?�ǵĻ�ֱ�ӷ��أ���Ļ����ÿ�ʼ��
	if (pMsg->message == WM_KEYDOWN)
	{
		WPARAM wParam = 0;	//���Ϊ�غ��˸񣬸�ֵΪVK_BACK�����Ϊ����д��������ֵΪVK_DELETE������Ϊ0
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
				pMsg->wParam = VK_DELETE;	//���򽫿�ʼ����Ϊnum����������ǡΪnum����ɾ�������ַ�
			}
				
			break;
		default:
			if (GetKeyState(VK_CONTROL) & 0x8000)	//Ctrl����
			{
				switch (pMsg->wParam)
				{
				case 'X':	//����
				case 'V':	//ճ��
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
			//�㷨����nStart��num?��Ļ����������ǵĻ���nEnd��num?�ǵĻ�ֱ�ӷ��أ���Ļ����ÿ�ʼ��
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
			//�����غ��˸񣬹��λ�á�numʱ���أ���01���2������ɾ���ַ�1
			if (nStart <= m_readOnlyCharNum)
				return TRUE;
			break;
		default:
			break;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}
