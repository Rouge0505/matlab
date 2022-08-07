#include "stdafx.h"
#include "Token.h"

//#define new DEBUG_NEW

char* const CToken::m_tokenStrList[] =
{ "none", "const", "var",
"while", "break", "continue", "end", "if", "else", "clear", "clc",
"+", "-", "*", "/", "^", "(", ")",
">", ">=", "<", "<=", "=", "==", "~", "~=",
"\\n", ";", "|", "&",
"output", "-", "jz", "jmp", "fin",
"const", "var", "_temp"};

CToken::CToken()
{
	memset(this, 0, sizeof(*this));
}

CToken::CToken(DWORD type, char * value)
{
	memset(this, 0, sizeof(*this));		//set时会判指针非空
	m_type = type;	
	setName(value);
}

CToken::CToken(const CToken & another)
{
	memset(this, 0, sizeof(*this));
	*this = another;
}

CToken::~CToken()
{
	clearPointers();
}

void CToken::setType(DWORD type)
{
	m_type = type;
}

void CToken::setNum(DWORD num)
{
	m_num = num;
}

void CToken::setAssignOutput(BOOL bOutput)
{
	m_num = bOutput;
}

void CToken::setJmpDestination(QUADNUM dest)
{
	m_num = dest;
}

void CToken::setName(char* name)
{
	if (!name)
		return;

	if (m_pName)
	{
		delete m_pName;
		m_pName = NULL;
	}

	int len = strlen(name);
	m_pName = new char[len + 1];
	strcpy(m_pName, name);
}

void CToken::setNextList(const list<QUADNUM>* pNextlist)
{
	if (!pNextlist)
		return;
	if (m_pNextList)
	{
		delete m_pNextList;
		m_pNextList = NULL;
	}
	m_pNextList = new list<QUADNUM>(*pNextlist);
}

void CToken::copyNextList(const CToken & another)
{
	if (!another.m_pNextList)
		return;
	if (m_pNextList)
	{
		delete m_pNextList;
		m_pNextList = NULL;
	}
	m_pNextList = new list<QUADNUM>(*another.m_pNextList);
}

void CToken::mergetNextList(const CToken & another)
{
	if (!another.m_pNextList)
		return;
	if (!m_pNextList)
		return;
	m_pNextList->merge(*another.m_pNextList);
}

void CToken::copyParamList(const CToken & another)
{
	if (!another.m_pParamList)
		return;
	if (m_pParamList)
	{
		delete m_pParamList;
		m_pParamList = NULL;
	}
	m_pParamList = new list<CToken>(*another.m_pParamList);
}

DWORD CToken::getType() const
{
	return m_type;
}

DWORD CToken::getNum() const
{
	return m_num;
}

QUADNUM CToken::getJmpDestination() const
{
	return m_num;
}

BOOL CToken::assignNeedsOutput() const
{
	return m_num;
}

const list<QUADNUM>* CToken::getNextList() const
{
	return m_pNextList;
}

const char * CToken::getName() const
{
	return m_pName;
}

CToken & CToken::operator=(const CToken & another)
{
	// TODO: 在此处插入 return 语句
	clearPointers();
	m_type = another.m_type;
	m_num = another.m_num;
	setName(another.m_pName);
	copyNextList(another);
	copyParamList(another);
	return *this;
}

void CToken::updateAdherence()
{
	m_type++;
}

void CToken::toString(string & output) const
{
	if (m_type >= _TOKEN_S_)	//不管词法分析还是四元式都不会出现的token
	{
		output = "error!!!";
		return;
	}
	output = m_tokenStrList[m_type];
	switch (m_type)
	{
	case _TOKEN_E_CONST_:
	case _TOKEN_CONST_:
	case _TOKEN_E_VAR_:
	case _TOKEN_VAR_:
		output += " ";
		output += m_pName;
		break;
	case _TOKEN_E_TEMP_:
		output = "_temp";
		output += to_string(m_num);
		break;
	case _TOKEN_JMP_:
	case _TOKEN_JZ_:
		output = to_string(m_num);
		break;
	}
}

int CToken::getMaxTemp(const CToken & token1, const CToken & token2)
{
	int tempNum = -1;
	if (token1.getType() == _TOKEN_E_TEMP_)
		tempNum = token1.getNum();
	if (token2.getType() == _TOKEN_E_TEMP_)
	{
		int tempNum2 = token2.getNum();
		if (tempNum2 > tempNum)
			tempNum = tempNum2;
	}
	return tempNum;
}

void CToken::push_backQuadNum(QUADNUM num)
{
	if (!m_pNextList)
		m_pNextList = new list<QUADNUM>;
	if (m_pNextList)
		m_pNextList->push_back(num);
}

void CToken::reset()
{
	m_type = _TOKEN_NONE_;
	m_num = 0;
	clearPointers();
}

void CToken::clearPointers()
{
	if (m_pName)
	{
		delete m_pName;
		m_pName = NULL;
	}
	if (m_pNextList)
	{
		delete m_pNextList;
		m_pNextList = NULL;
	}
	if (m_pParamList)
	{
		delete m_pParamList;
		m_pParamList = NULL;
	}
}
