#include "stdafx.h"
#include "MInterpreterBE.h"

const char* CMInterpreterBE::m_reserved[] = { "while", "break", "continue", "end", "if", "else", "clear", "clc" };
const CAction CMInterpreterBE::m_LRTable[44][20]		//LR表，静态方便初始化，也方便多线程调用
=
{
	{ _S(1), _S(2), _S(3), _S(4), _S(5), _S(6), _S(7), _S(8), _S(9), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _S(40), _ERR_, _S(43) },		//0
	{ _ERR_, _ACC_, _ERR_, _ACC_, _ACC_, _ACC_, _ACC_, _ACC_, _ACC_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ACC_ },		//1
	{ _ERR_, _R(1), _ERR_, _R(1), _R(1), _R(1), _R(1), _R(1), _R(1), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _R(1), _R(1), _R(1), },		//2
	{ _ERR_, _S(10), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _S(11), _ERR_, _S(12), _S(13), _S(14), _S(15), _S(16),},		//3
	{ _ERR_, _R(15), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _R(15), _ERR_, _R(15), _R(15), _R(15), _R(15), _R(15), _S(17), _R(15), },		//4
	{ _ERR_, _ERR_, _S(18), _S(19), _ERR_, _ERR_, _S(7), _S(8), _S(9) },		//5
	{ _ERR_, _ERR_, _S(20), _S(19), _ERR_, _ERR_, _S(7), _S(8), _S(9) },		//6
	{ _ERR_, _ERR_, _S(21), _S(19), _ERR_, _ERR_, _S(7), _S(8), _S(9) },		//7
	{ _ERR_, _ERR_, _S(22), _S(19), _ERR_, _ERR_, _S(7), _S(8), _S(9) },		//8
	{ _ERR_, _R(16), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _R(16), _ERR_, _R(16), _R(16), _R(16), _R(16), _R(16), _ERR_, _R(16), },//9
	{ _ERR_, _R(2), _ERR_, _R(2), _R(2), _R(2), _R(2), _R(2), _R(2), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _R(2), _R(2), _R(2), },		//10
	{ _ERR_, _ERR_, _S(23), _S(19), _ERR_, _ERR_, _S(7), _S(8), _S(9) },		//11
	{ _ERR_, _ERR_, _S(24), _S(19), _ERR_, _ERR_, _S(7), _S(8), _S(9) },		//12
	{ _ERR_, _ERR_, _S(25), _S(19), _ERR_, _ERR_, _S(7), _S(8), _S(9) },		//13
	{ _ERR_, _ERR_, _S(26), _S(19), _ERR_, _ERR_, _S(7), _S(8), _S(9) },		//14
	{ _ERR_, _ERR_, _S(27), _S(19), _ERR_, _ERR_, _S(7), _S(8), _S(9) },		//15
	{ _ERR_, _ERR_, _S(28), _S(19), _ERR_, _ERR_, _S(7), _S(8), _S(9) },		//16
	{ _ERR_, _ERR_, _S(29), _S(19), _ERR_, _ERR_, _S(7), _S(8), _S(9) },		//17
	{ _ERR_, _S(30), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _S(11), _ERR_, _S(12), _S(13), _S(14), _S(15), _S(16), },		//18
	{ _ERR_, _R(15), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _R(15), _ERR_, _R(15), _R(15), _R(15), _R(15), _R(15), _ERR_, _R(15), },		//19
	{ _ERR_, _S(31), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _S(11), _ERR_, _S(12), _S(13), _S(14), _S(15), _S(16), },		//20
	{ _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _S(11), _ERR_, _S(12), _S(13), _S(14), _S(15), _S(16), _ERR_, _S(32) },		//21

	{ _ERR_, _R(14), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _R(14), _ERR_, _R(14), _S(13), _R(14), _R(14), _R(14), _ERR_, _R(14), },		//22
	{ _ERR_, _R(7), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _R(7), _ERR_, _S(12), _S(13), _R(7), _R(7), _R(7), _ERR_, _R(7), },		//23
	{ _ERR_, _R(8), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _R(8), _ERR_, _R(8), _S(13), _R(8), _R(8), _R(8), _ERR_, _R(8), },		//24
	{ _ERR_, _R(9), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _R(9), _ERR_, _R(9), _R(9), _R(9), _R(9), _R(9), _ERR_, _R(9), },//25
	{ _ERR_, _R(10), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _S(11), _ERR_, _S(12), _S(13), _R(10), _R(10), _R(10), _ERR_, _R(10), },//26
	{ _ERR_, _R(11), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _S(11), _ERR_, _S(12), _S(13), _S(14), _R(11), _S(16), _ERR_, _R(11), },//27
	{ _ERR_, _R(12), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _S(11), _ERR_, _S(12), _S(13), _S(14), _R(12), _R(12), _ERR_, _R(12), },//28

	{ _ERR_, _S(33), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _S(11), _ERR_, _S(12), _S(13), _S(14), _S(15), _S(16), },		//29
	{ _S(35), _S(2), _S(3), _S(4), _S(5), _S(6), _S(7), _S(8), _S(9), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _S(34), _ERR_, _ERR_, _S(43) },		//30
	{ _S(35), _S(2), _S(3), _S(4), _S(5), _S(6), _S(7), _S(8), _S(9), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _S(36), _ERR_, _ERR_, _S(43) },		//31
	{ _ERR_, _R(13), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _R(13), _ERR_, _R(13), _R(13), _R(13), _R(13), _R(13), _ERR_, _R(13), },		//32
	{ _ERR_, _R(3), _ERR_, _R(3), _R(3), _R(3), _R(3), _R(3), _R(3), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _R(3), _R(3), _R(3), },		//33
	{ _S(39), _S(2), _S(3), _S(4), _S(5), _S(6), _S(7), _S(8), _S(9), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _S(37), _S(38), _S(43) },		//34
	{ _ERR_, _R(17), _ERR_, _R(17), _R(17), _R(17), _R(17), _R(17), _R(17), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _R(17), _R(17), _R(17), },		//35
	{ _S(39), _S(2), _S(3), _S(4), _S(5), _S(6), _S(7), _S(8), _S(9), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _S(40), _ERR_, _S(43) },		//36
	{ _ERR_, _R(4), _ERR_, _R(4), _R(4), _R(4), _R(4), _R(4), _R(4), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _R(4), _R(4), _R(4), },		//37
	{ _S(35), _S(2), _S(3), _S(4), _S(5), _S(6), _S(7), _S(8), _S(9), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _S(41), _ERR_, _ERR_, _S(43) },		//38
	{ _ERR_, _R(18), _ERR_, _R(18), _R(18), _R(18), _R(18), _R(18), _R(18), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _R(18), _R(18), _R(18), },		//39
	{ _ERR_, _R(6), _ERR_, _R(6), _R(6), _R(6), _R(6), _R(6), _R(6), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _R(6), _R(6), _R(6), },		//40
	{ _S(39), _S(2), _S(3), _S(4), _S(5), _S(6), _S(7), _S(8), _S(9), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _S(42), _ERR_, _S(43) },		//41
	{ _ERR_, _R(5), _ERR_, _R(5), _R(5), _R(5), _R(5), _R(5), _R(5), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _R(5), _R(5), _R(5), },		//42
	{ _ERR_, _R(19), _ERR_, _R(19), _R(19), _R(19), _R(19), _R(19), _R(19), _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _ERR_, _R(19), _R(19), _R(19), },		//43
};

const TCHAR* CMInterpreterBE::m_errStrList[]		//报错信息字符串
=
{
	_T("正在解析"),
	_T("解析成功"),
	_T("意外的字符"),
	_T("错误的常量"),
	_T("语法错误"),
	_T("break或continue不在循环中"),
	_T("计算成功"),
	_T("除以0错误"),
	_T("标识符不存在"),
	_T("操作数不是常量或变量"),
	_T("临时变量栈错误"),
	_T("无效的四元式操作符"),
	_T("意外的错误"),
};

CMInterpreterBE::CMInterpreterBE()
{
	memset(this, 0, sizeof(*this));
	m_pSemanticStack = new vector<CToken>;
	m_pSemanticStack->reserve(64);
	m_pStatusStack = new vector<BYTE>;
	m_pStatusStack->reserve(64);
	m_pStatusStack->push_back(0);				//最初有个0状态
	m_pQuadStack = new vector<CQuadruple>;
	m_pQuadStack->reserve(128);
	m_wordCategory = _INVALID_COLUMN_;
	m_pLoopStack = new vector<QUADNUM>;
	m_pLoopStack->reserve(8);
	m_pVarList = new map<string, double>;
	m_pTempList = new vector<double>;
	m_pTempList->reserve(16);
}


CMInterpreterBE::~CMInterpreterBE()
{
	if (m_pBuffer)
		delete m_pBuffer;
	if (m_pSemanticStack)
		delete m_pSemanticStack;
	if (m_pStatusStack)
		delete m_pStatusStack;
	if (m_pQuadStack)
		delete m_pQuadStack;
	if (m_pLexFile)
		fclose(m_pLexFile);
	if (m_pQuadFile)
		fclose(m_pQuadFile);
	if (m_pLoopStack)
		delete m_pLoopStack;
	if (m_pVarList)
		delete m_pVarList;
	if (m_pTempList)
		delete m_pTempList;
}

//BOOL CMInterpreterBE::open(const TCHAR * path)
//{
//	if (m_pFile)
//		fclose(m_pFile);
//	m_pFile = _tfopen(path, _T("rb"));
//	if (!m_pFile)
//		return FALSE;
//
//	TCHAR folder[MAX_PATH] = { 0 };
//	_tcscpy(folder, path);
//	int i = _tcslen(folder);
//	for (; i >= 0; i--)
//	{
//		if (folder[i] == _T('\\') || folder[i] == _T('/'))
//			break;
//	}
//	_tcscpy(folder + i + 1, _T("Debug"));
//	CreateDirectory(folder, NULL);
//	TCHAR debugPath[MAX_PATH] = { 0 };
//	//_stprintf(debugPath, _T("%s/"))
//	m_pLexFile = fopen("lex.txt", "w");
//	m_pQuadFile = fopen("quad.txt", "w");
//
//	if (m_pFile)
//	{
//		//测试
//		char buffer[100] = { 0 };
//		fread(buffer, 100, 1, m_pFile);
//		fseek(m_pFile, 0, SEEK_END);
//		m_bufferLen = ftell(m_pFile);
//		fseek(m_pFile, 0, SEEK_SET);
//	}
//
//	return TRUE;
//}

BOOL CMInterpreterBE::open(const TCHAR * folderPath, const TCHAR * fileName)
{
	TCHAR filePath[MAX_PATH] = { 0 };			//M文件完整路径
	_stprintf(filePath, _T("%s\\%s"), folderPath, fileName);
	FILE* pFile = _tfopen(filePath, _T("rb"));
	if (!pFile)
		return FALSE;

	fseek(pFile, 0, SEEK_END);
	m_bufferLen = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	if (m_pBuffer)
		delete m_pBuffer;
	m_pBuffer = new char[m_bufferLen + 1];
	size_t sRet = fread(m_pBuffer, m_bufferLen, 1, pFile);
	fclose(pFile);
	if (!sRet && m_bufferLen)	//多判断文件长度不为0，避免空文件打开失败
		return FALSE;
	m_pBuffer[m_bufferLen] = 0;


	if (m_pLexFile)
	{
		fclose(m_pLexFile);
		m_pLexFile = 0;
	}
	if (m_pQuadFile)
	{
		fclose(m_pQuadFile);
		m_pQuadFile = 0;
	}
	TCHAR debugFolder[MAX_PATH] = { 0 };			//debug文件夹，存放词法分析结果和四元式
	_stprintf(debugFolder, _T("%s\\Debug"), folderPath);
	if (!CreateDirectory(debugFolder, NULL))				//成功返回非0
		if (GetLastError() != ERROR_ALREADY_EXISTS)		//创建失败了，还不是因为已经存在而失败，说明没这文件夹，直接返回
			return TRUE;								//还是能解析，待细化返回值

	TCHAR debufFileName[MAX_PATH] = { 0 };		//词法分析结果和四元式文件名
	TCHAR debugFilePath[MAX_PATH] = { 0 };		//词法分析结果和四元式文件完整路径
	_tcscpy(debufFileName, fileName);
	const TCHAR* pDot = _tcsrchr(debufFileName, _T('.'));	//找到.的位置
	if (pDot)
		pDot++;	
	
	_tcscpy((TCHAR*)pDot, _T("lex"));				//词法分析
	_stprintf(debugFilePath, _T("%s\\%s"), debugFolder, debufFileName);
	m_pLexFile = _tfopen(debugFilePath, _T("w"));

	_tcscpy((TCHAR*)pDot, _T("quad"));				//四元式
	_stprintf(debugFilePath, _T("%s\\%s"), debugFolder, debufFileName);
	m_pQuadFile = _tfopen(debugFilePath, _T("w"));

	return TRUE;
}

void CMInterpreterBE::setStatement(const char * statement, const TCHAR* workPath)
{
	if (m_pBuffer)
		delete m_pBuffer;
	m_bufferLen = strlen(statement);
	m_pBuffer = new char[m_bufferLen + 1];
	strcpy(m_pBuffer, statement);
	if (m_pLexFile)
	{
		fclose(m_pLexFile);
		m_pLexFile = 0;
	}
	if (m_pQuadFile)
	{
		fclose(m_pQuadFile);
		m_pQuadFile = 0;
	}
	TCHAR debugFolder[MAX_PATH] = { 0 };			//debug文件夹，存放词法分析结果和四元式
	_stprintf(debugFolder, _T("%s\\Debug"), workPath);
	if (!CreateDirectory(debugFolder, NULL))				//成功返回非0
		if (GetLastError() != ERROR_ALREADY_EXISTS)		//创建失败了，还不是因为已经存在而失败，说明没这文件夹，直接返回
			return;
	TCHAR debugFilePath[MAX_PATH] = { 0 };		//词法分析结果和四元式文件完整路径
	_stprintf(debugFilePath, _T("%s\\anonymous.lex"), debugFolder);
	m_pLexFile = _tfopen(debugFilePath, _T("w"));

	_stprintf(debugFilePath, _T("%s\\anonymous.quad"), debugFolder);
	m_pQuadFile = _tfopen(debugFilePath, _T("w"));
}

const char * CMInterpreterBE::getWord()
{
	m_wordLen = 0;
	memset(m_word, 0, sizeof(m_word));
	//m_lastToken.setType(_TOKEN_NONE_);
	m_lastToken.reset();
	if (!getAValidCharacter())							//读到末尾都没读到有意义字符
		return m_word;

	m_word[m_wordLen++] = m_character;					//有意义字符往数组里填
	switch (m_chCategory)
	{
	case _CH_LETTER_:									//字符为字母
		handleIdentifier();
		break;
	case _CH_DIGIT_:									//字符为数字
	case '.':											//数字也可能以小数点开头
		handleDigit();
		break;
	case '>':
		m_lastToken.setType(_TOKEN_GRT_);
		handleAdherence();
		break;
	case '<':
		m_lastToken.setType(_TOKEN_LES_);
		handleAdherence();
		break;
	case '=':
		m_lastToken.setType(_TOKEN_ASN_);
		handleAdherence();
		break;
	case '~':
		m_lastToken.setType(_TOKEN_NOT_);
		handleAdherence();
		break;
	case '+':
		m_lastToken.setType(_TOKEN_ADD_);
		m_chCategory = _CH_BLANK_;
		break;
	case '-':
		m_lastToken.setType(_TOKEN_SUB_);
		m_chCategory = _CH_BLANK_;
		break;
	case '*':
		m_lastToken.setType(_TOKEN_MUL_);
		m_chCategory = _CH_BLANK_;
		break;
	case '/':
		m_lastToken.setType(_TOKEN_DIV_);
		m_chCategory = _CH_BLANK_;
		break;
	case '^':
		m_lastToken.setType(_TOKEN_POW_);
		m_chCategory = _CH_BLANK_;
		break;
	case '(':
		m_lastToken.setType(_TOKEN_SLP_);
		m_chCategory = _CH_BLANK_;
		break;
	case ')':
		m_lastToken.setType(_TOKEN_SRP_);
		m_chCategory = _CH_BLANK_;
		break;
	case ';':
		m_lastToken.setType(_TOKEN_SEMCLN_);
		m_chCategory = _CH_BLANK_;
		break;
	case '\n':
		m_lastToken.setType(_TOKEN_NEWL_);
		m_chCategory = _CH_BLANK_;
		break;
	case '|':
		m_lastToken.setType(_TOKEN_OR_);
		if (getCharacter() == '|')
			m_chCategory = _CH_BLANK_;
		break;
	case '&':
		m_lastToken.setType(_TOKEN_AND_);
		if (getCharacter() == '&')
			m_chCategory = _CH_BLANK_;
		break;
	default:
		m_status = _STATUS_UNEXPECTED_CHAR_;
		_stprintf(m_errStr, _T("%s %c\r\n\r\n"), m_errStrList[m_status], m_character);
		break;
	}

	//m_chCategory = _CH_BLANK_;							//置无效，下次取词时要重新读
	//m_pSemanticStack->push_back(m_lastToken);
	return m_word;
}

BYTE CMInterpreterBE::run()
{
	reset();
	BYTE result = parse();
	if (result != _STATUS_PARSE_SUCCESS_)
		return result;
	return calculate();
}

BYTE CMInterpreterBE::parse()
{
	const char* temp = NULL;
	while (1)
	{
		//if (m_word[0] == 0)
		//if (m_lastToken.getType() == _TOKEN_NONE_)
		if (m_wordCategory == _INVALID_COLUMN_)
		{
			temp = getWord();
			categoryWord();
		}
		if (m_status != _STATUS_PROGRESS_)		//解析成功或者失败了
			break;
		size_t stackSize = m_pStatusStack->size();
		BYTE curState = (*m_pStatusStack)[stackSize - 1];
		//const CAction& action = m_LRTable[curState][m_wordCategory];
		m_pAction = *(m_LRTable + curState) + m_wordCategory;

		switch (m_pAction->getType())
		{
		case _ACTION_SFT_:
			shift();
			break;
		case _ACTION_RDC_:
			reduce();
			break;
		case _ACTION_ACC_:
			//m_status = _STATUS_SUCCESS_;
			accept();
			break;
		case _ACTION_ERR_:
			m_status = _STATUS_SYNTAX_ERR_;
			{
				string lastStr;
				string lastlastStr;
				m_lastToken.toString(lastStr);

				size_t semanticSize = m_pSemanticStack->size();
				if (semanticSize > 0)
				{
					CToken& lastlastToken = (*m_pSemanticStack)[semanticSize - 1];
					lastlastToken.toString(lastlastStr);
				}
				TCHAR tcLastStr[16] = { 0 };
				TCHAR tcLastLastStr[16] = { 0 };
				_tcscpy(tcLastStr, CA2T(lastStr.data()));
				_tcscpy(tcLastLastStr, CA2T(lastlastStr.data()));
				_stprintf(m_errStr, _T("%s %s%s\r\n\r\n"), m_errStrList[m_status], tcLastLastStr, tcLastStr);
			}
			return m_status;
		default:
			m_status = _STATUS_UNEXPECTED_ERR_;
			return m_status;
		}
	}
	
	
	return m_status;
}

void CMInterpreterBE::categoryWord()
{
	DWORD type = m_lastToken.getType();			//语法分析出的词是什么类型
	switch (type)
	{
	case _TOKEN_CONST_:
		m_wordCategory = _COLUMN_CONST_;
		break;
	case _TOKEN_VAR_:
		m_wordCategory = _COLUMN_VAR_;
		break;

	case _TOKEN_WHILE_:
		m_wordCategory = _COLUMN_WHILE_;
		break;
	case _TOKEN_BREAK_:
	case _TOKEN_CONTN_:
	case _TOKEN_CLEAR_:
	case _TOKEN_CLC_:
		m_wordCategory = _COLUMN_BREAK_;
		break;
	case _TOKEN_END_:
		m_wordCategory = _COLUMN_END_;
		break;
	case _TOKEN_IF_:
		m_wordCategory = _COLUMN_IF_;
		break;
	case _TOKEN_ELSE_:
		m_wordCategory = _COLUMN_ELSE_;
		break;
	case _TOKEN_ADD_:
	case _TOKEN_SUB_:
	case _TOKEN_NOT_:
		m_wordCategory = _COLUMN_ADD_;
		break;
	case _TOKEN_MUL_:
	case _TOKEN_DIV_:
		m_wordCategory = _COLUMN_MUL_;
		break;
	case _TOKEN_POW_:
		m_wordCategory = _COLUMN_POW_;
		break;
	case _TOKEN_SLP_:
		m_wordCategory = _COLUMN_SLP_;
		break;
	case _TOKEN_SRP_:
		m_wordCategory = _COLUMN_SRP_;
		break;
	case _TOKEN_GRT_:
	case _TOKEN_GRE_:
	case _TOKEN_LES_:
	case _TOKEN_LEE_:
	case _TOKEN_EQL_:
	case _TOKEN_NEQ_:
		m_wordCategory = _COLUMN_GRT_;
		break;
	case _TOKEN_ASN_:
		m_wordCategory = _COLUMN_ASN_;
		break;
	case _TOKEN_NEWL_:
	case _TOKEN_SEMCLN_:
	case _TOKEN_NONE_:	//读完文档了，骗解释器说还有个分号，方便归约，acc后判实际token为空结束
		m_wordCategory = _COLUMN_SEMCLN_;
		break;
	case _TOKEN_OR_:
		m_wordCategory = _COLUMN_OR_;
		break;
	case _TOKEN_AND_:
		m_wordCategory = _COLUMN_AND_;
		break;
	default:
		//m_status = _STATUS_TOKEN_ERR_;	//取词错误目前只有常量错误，报错在getWord完成
		break;
	}
}

void CMInterpreterBE::shift()
{
	//一些额外语义动作
	switch (m_wordCategory)
	{
	case _COLUMN_WHILE_:
		m_lastToken.setNum(m_pQuadStack->size());			//while记录下一条四元式的编号，即E第一条
		m_pLoopStack->push_back(m_pSemanticStack->size());	//循环栈记录下一个token的下标，即即将移入while的下标
		break;
	case _COLUMN_SEMCLN_:
	{
		DWORD semanticSize = m_pSemanticStack->size();
		if (semanticSize < 2)	//if E或while E至少得有两个token
			break;
		const CToken& E = (*m_pSemanticStack)[semanticSize - 1];
		const CToken& ctrl = (*m_pSemanticStack)[semanticSize - 2];		//控制token
		DWORD ctrlType = ctrl.getType();								//控制类型
		if (ctrlType != _TOKEN_IF_ && ctrlType != _TOKEN_WHILE_)
			break;
		//判断通过，生成jz四元式
		m_lastToken.setNum(m_pQuadStack->size());			//;记录下一条四元式的编号，即jz指令
		CQuadruple quad;						//出口等待回填，;记录的即为该四元式
		quad.setJumpZero(E);
		m_pQuadStack->push_back(quad);
	}
		break;
	case _COLUMN_ELSE_:	//else生成跳转到出口四元式，等待回填。记录下一条四元式编号，一来待回填，二来再下一条四元式即为;四元式的跳转目标即假出口
	{
		m_lastToken.setNum(m_pQuadStack->size());	//记录下一条四元式的编号，即jmp指令；再下一条指令即为假出口
		CQuadruple quad;
		quad.setOperation(_TOKEN_JMP_);
		m_pQuadStack->push_back(quad);
	}
		break;
	}
	//移进通用操作
	m_pStatusStack->push_back(m_pAction->getValue());	//更新状态栈
	m_pSemanticStack->push_back(m_lastToken);			//更新语义栈
	m_wordCategory = _INVALID_COLUMN_;						//下次循环重新取词

	//打印语法分析结果
	if (m_pLexFile)
	{
		string temp;
		m_lastToken.toString(temp);
		fprintf(m_pLexFile, "%s\n", temp.data());
		fflush(m_pLexFile);
	}
		
}

void CMInterpreterBE::reduce()
{
	switch (m_pAction->getValue())		//按几号产生式进行归约
	{
	case 1:
		reduce1();
		break;
	case 2:
		reduce2();
		break;
	case 3:
		reduce3();
		break;
	case 4:
		reduce4();
		break;
	case 5:
		reduce5();
		break;
	case 6:
		reduce6();
		break;
	case 7:
		reduce7();
		break;
	case 8:
		reduce8();
		break;
	case 9:
		reduce9();
		break;
	case 10:
		reduce10();
		break;
	case 11:
		reduce11();
		break;
	case 12:
		reduce12();
		break;
	case 13:
		reduce13();
		break;
	case 14:
		reduce14();
		break;
	case 15:
		reduce15();
		break;
	case 16:
		reduce16();
		break;
	case 17:
		reduce17();
		break;
	case 18:
		reduce18();
		break;
	case 19:
		reduce19();
		break;
	case 0:								//不会出现，r0一般被acc替代
	default:
		m_status = _STATUS_UNEXPECTED_ERR_;
		return;
	}
	handleStatusStack();				//语义栈处理完毕，处理状态栈
}

void CMInterpreterBE::reduce1()		//S->;
{
	size_t semanticSize = m_pSemanticStack->size();
	CToken& rToken = (*m_pSemanticStack)[semanticSize - 1];
	rToken.setType(_TOKEN_S_);		//;出栈S入栈简化成更改种别码
	m_pStatusStack->pop_back();		//状态栈弹栈
}

void CMInterpreterBE::reduce2()		//S->E; 1.E是变量 \n生成输出四元式，;不处理 2.E非变量，产生ans=E四元式，\n输出;不输出
{
	size_t semanticSize = m_pSemanticStack->size();
	CToken& semicolon = (*m_pSemanticStack)[semanticSize - 1];	//;或者\n
	CToken& E = (*m_pSemanticStack)[semanticSize - 2];	//E

	DWORD endCode = semicolon.getType();			//结尾，是\n还是;
	DWORD EType = E.getType();						//E是_TOKEN_E_VAR_还是其他
	if (E.getType() == _TOKEN_E_VAR_)
	{
		if (semicolon.getType() != _TOKEN_SEMCLN_)	//\n输出 ;不产生四元式
		{
			CQuadruple quad(E);	//输出
			m_pQuadStack->push_back(quad);
		}
	}
	else											//ans=E	
	{
		//四元式
		CQuadruple quad(_TOKEN_ASN_, E, CToken(_TOKEN_VAR_, "ans"));	//构造CToken用户变量
		if (semicolon.getType() != _TOKEN_SEMCLN_)	//\n输出
			quad.setAssignOutput();
		m_pQuadStack->push_back(quad);
	}
	

	E.setType(_TOKEN_S_);		//语义栈E直接替换
	E.clearPointers();				//清理掉无用指针，避免产生干扰。对S来说，这个指针表示nextlist，比如P->PS时，要将S的nextlist给P，如果这个指针实际指向了一个字符串，那必然会出错
	m_pSemanticStack->pop_back();	//;弹出
	for (int i = 0; i < 2; i++)		//状态栈弹栈
		m_pStatusStack->pop_back();
}

void CMInterpreterBE::reduce3()	//S->var=E;	如果结尾是;不输出，是\n则输出
{
	size_t semanticSize = m_pSemanticStack->size();
	CToken& semicolon = (*m_pSemanticStack)[semanticSize - 1];	//;或者\n
	CToken& E = (*m_pSemanticStack)[semanticSize - 2];
	CToken& var = (*m_pSemanticStack)[semanticSize - 4];
	CQuadruple quad(_TOKEN_ASN_, E, var);
	if (semicolon.getType() != _TOKEN_SEMCLN_)	//如果是不是;结尾则打印输出
		quad.setAssignOutput();
	m_pQuadStack->push_back(quad);
	for (int i = 0; i < 4; i++)
	{
		m_pSemanticStack->pop_back();		//语义栈弹栈
		m_pStatusStack->pop_back();			//状态栈弹栈
	}
	m_pSemanticStack->push_back(CToken(_TOKEN_S_));	//语义栈压栈	//构造CToken语义符号
}

void CMInterpreterBE::reduce4()	//S->if E; P end
{
	size_t semanticSize = m_pSemanticStack->size();
	CToken& P = (*m_pSemanticStack)[semanticSize - 2];
	CToken& semicolon = (*m_pSemanticStack)[semanticSize - 3];	//;或者\n
	CToken& IF = (*m_pSemanticStack)[semanticSize - 5];
	IF.reset();
	IF.setType(_TOKEN_S_);
	IF.copyNextList(P);
	IF.push_backQuadNum(semicolon.getNum());
	for (int i = 0; i < 4; i++)
		m_pSemanticStack->pop_back();		//语义栈弹栈
	for (int i = 0; i < 5; i++)
		m_pStatusStack->pop_back();			//状态栈弹栈
}

void CMInterpreterBE::reduce5()	//S->if E; P1 else P2 end
{
	size_t semanticSize = m_pSemanticStack->size();
	CToken& P2 = (*m_pSemanticStack)[semanticSize - 2];
	CToken& ELSE = (*m_pSemanticStack)[semanticSize - 3];	//else记录了下一条四元式(jmp到出口)下标
	CToken& P1 = (*m_pSemanticStack)[semanticSize - 4];
	CToken& semicolon = (*m_pSemanticStack)[semanticSize - 5];	//;或者\n
	CToken& IF = (*m_pSemanticStack)[semanticSize - 7];

	QUADNUM jzNum = semicolon.getNum();	//;记录了下一条，即jz四元式下标
	CQuadruple& jzQuad = m_pQuadStack->at(jzNum);
	jzQuad.setJmpDestination(ELSE.getNum() + 1);	//上述四元式要跳转到else的下下条四元式，即P2第一条

	IF.reset();
	IF.setType(_TOKEN_S_);
	IF.copyNextList(P1);
	IF.push_backQuadNum(ELSE.getNum());
	IF.mergetNextList(P2);					//以上都是严格升序，满足list::merge条件

	for (int i = 0; i < 6; i++)
		m_pSemanticStack->pop_back();		//语义栈弹栈
	for (int i = 0; i < 7; i++)
		m_pStatusStack->pop_back();			//状态栈弹栈
}

void CMInterpreterBE::reduce6()	//S->while E; P end
{
	size_t semanticSize = m_pSemanticStack->size();
	CToken& P = (*m_pSemanticStack)[semanticSize - 2];
	CToken& semicolon = (*m_pSemanticStack)[semanticSize - 3];
	CToken& WHILE = (*m_pSemanticStack)[semanticSize - 5];
	backpatch(P, WHILE.getNum());			//P执行完跳回去
	WHILE.setType(_TOKEN_S_);
	WHILE.push_backQuadNum(semicolon.getNum());
	CQuadruple quad;						//生成跳回while四元式
	quad.setOperation(_TOKEN_JMP_);
	quad.setJmpDestination(WHILE.getNum());
	m_pQuadStack->push_back(quad);
	for (int i = 0; i < 4; i++)
		m_pSemanticStack->pop_back();			//语义栈弹栈
	for (int i = 0; i < 5; i++)
		m_pStatusStack->pop_back();			//状态栈弹栈
	m_pLoopStack->pop_back();				//循环栈弹栈
}

void CMInterpreterBE::reduce7()	//E->E1+E2
{
	size_t semanticSize = m_pSemanticStack->size();
	CToken& E2 = (*m_pSemanticStack)[semanticSize - 1];	//E2
	CToken& op = (*m_pSemanticStack)[semanticSize - 2];	//operation
	CToken& E1 = (*m_pSemanticStack)[semanticSize - 3];	//E1
	int tempNum = CToken::getMaxTemp(E1, E2);	//获取两者临时变量较大者，或者都不是临时变量返回-1
	if (tempNum < 0)
		tempNum = m_tempNum++;
	CToken result(_TOKEN_E_TEMP_);		//构造CToken临时变量
	result.setNum(tempNum);
	CQuadruple quad(op.getType(), E1, E2, result);
	m_pQuadStack->push_back(quad);

	for (int i = 0; i < 3; i++)
	{
		m_pSemanticStack->pop_back();		//语义栈弹栈
		m_pStatusStack->pop_back();			//状态栈弹栈
	}
	m_pSemanticStack->push_back(result);	//语义栈压栈
}

void CMInterpreterBE::reduce8()	//E->E*E
{
	reduce7();
}

void CMInterpreterBE::reduce9()	//E->E^E
{
	reduce7();
}

void CMInterpreterBE::reduce10()	//E->E>E
{
	reduce7();
}

void CMInterpreterBE::reduce11()	//E->E|E
{
	reduce7();
}

void CMInterpreterBE::reduce12()	//E->E&E
{
	reduce7();
}

void CMInterpreterBE::reduce13()	//E->(E)
{
	size_t semanticSize = m_pSemanticStack->size();
	CToken& E = (*m_pSemanticStack)[semanticSize - 2];	//E
	CToken& SLP = (*m_pSemanticStack)[semanticSize - 3];	//(
	SLP = E;

	for (int i = 0; i < 2; i++)
		m_pSemanticStack->pop_back();		//语义栈弹栈
	for (int i = 0; i < 3; i++)
		m_pStatusStack->pop_back();			//状态栈弹栈
}

void CMInterpreterBE::reduce14()	//E->+E
{
	for (int i = 0; i < 2; i++)
		m_pStatusStack->pop_back();			//状态栈弹栈
	size_t semanticSize = m_pSemanticStack->size();
	CToken& E = (*m_pSemanticStack)[semanticSize - 1];	//E
	CToken& op = (*m_pSemanticStack)[semanticSize - 2];	//+
	DWORD operation = op.getType();
	if (operation == _TOKEN_ADD_)
	{
		op = E;
		m_pSemanticStack->pop_back();		//语义栈弹栈
		return;
	}
	else
	{
		if (operation == _TOKEN_SUB_)
			operation = _TOKEN_USUB_;
		CToken result(_TOKEN_E_TEMP_);		//构造CToken临时变量
		result.setNum(m_tempNum++);
		CQuadruple quad(operation, E, result);
		m_pQuadStack->push_back(quad);
		op = result;
		m_pSemanticStack->pop_back();		//语义栈弹栈
	}
}

void CMInterpreterBE::reduce15()	//E->var
{
	size_t semanticSize = m_pSemanticStack->size();
	CToken& rToken = (*m_pSemanticStack)[semanticSize - 1];	//E
	rToken.setType(_TOKEN_E_VAR_);	//语义栈直接替换
	m_pStatusStack->pop_back();			//状态栈弹栈
}

void CMInterpreterBE::reduce16()	//E->const
{
	size_t semanticSize = m_pSemanticStack->size();
	CToken& cst = (*m_pSemanticStack)[semanticSize - 1];
	cst.setType(_TOKEN_E_CONST_);	//语义栈直接替换
	m_pStatusStack->pop_back();			//状态栈弹栈
}

void CMInterpreterBE::reduce17()	//P->S
{
	size_t semanticSize = m_pSemanticStack->size();
	CToken& S = (*m_pSemanticStack)[semanticSize - 1];
	S.setType(_TOKEN_P_);	//语义栈直接替换
	S.setNum(m_pQuadStack->size());		//新归约的P记录下下一条四元式的下标
	m_pStatusStack->pop_back();			//状态栈弹栈
}

void CMInterpreterBE::reduce18()	//P->PS
{
	size_t semanticSize = m_pSemanticStack->size();
	CToken& S = (*m_pSemanticStack)[semanticSize - 1];
	CToken& P = (*m_pSemanticStack)[semanticSize - 2];
	backpatch(P, P.getNum());		//P的nextlist回填当时记录的P下一条四元式编号
	P.setNextList(S.getNextList());
	P.setNum(m_pQuadStack->size());		//新归约的P记录下下一条四元式的下标
	m_pSemanticStack->pop_back();
	for (int i = 0; i < 2; i++)
		m_pStatusStack->pop_back();			//状态栈弹栈
}

void CMInterpreterBE::reduce19()	//S->break
{
	size_t semanticSize = m_pSemanticStack->size();
	CToken& rBreak = (*m_pSemanticStack)[semanticSize - 1];	//break或continue或clear或clc token

	switch (rBreak.getType())
	{
	case _TOKEN_BREAK_:
	{
		DWORD loopSize = m_pLoopStack->size();
		if (loopSize == 0)						//压根没有循环
		{
			m_status = _STATUS_BREAK_SANS_LOOP_;
			_stprintf(m_errStr, _T("break不在循环中\r\n\r\n"));
			return;
		}
		QUADNUM lastLoopNum = (*m_pLoopStack)[loopSize - 1];	//上一个loop标识符在语义栈中的下标
		CToken& rWhile = (*m_pSemanticStack)[lastLoopNum];		//while token
		rWhile.push_backQuadNum(m_pQuadStack->size());		//while token记录下一条四元式的下标
		CQuadruple quad;
		quad.setOperation(_TOKEN_JMP_);						//break无条件跳转，目标待回填
		m_pQuadStack->push_back(quad);
	}
		break;
	case _TOKEN_CONTN_:
	{
		DWORD loopSize = m_pLoopStack->size();
		if (loopSize == 0)						//压根没有循环
		{
			m_status = _STATUS_BREAK_SANS_LOOP_;
			_stprintf(m_errStr, _T("continue不在循环中\r\n\r\n"));
			return;
		}
		QUADNUM lastLoopNum = (*m_pLoopStack)[loopSize - 1];	//上一个loop标识符在语义栈中的下标
		CToken& rWhile = (*m_pSemanticStack)[lastLoopNum];		//while token
		CQuadruple quad;
		quad.setOperation(_TOKEN_JMP_);				//continue无条件跳转
		quad.setJmpDestination(rWhile.getNum());		//到E的第一条四元式
		m_pQuadStack->push_back(quad);
	}
		break;
	case _TOKEN_CLEAR_:
	case _TOKEN_CLC_:
	{
		CQuadruple quad;
		quad.setOperation(rBreak.getType());
		m_pQuadStack->push_back(quad);
	}
		break;
	default:
		m_status = _STATUS_UNEXPECTED_ERR_;
		break;
	}
	m_pStatusStack->pop_back();
	rBreak.setType(_TOKEN_S_);
}

void CMInterpreterBE::handleStatusStack()
{
	if (m_status != _STATUS_PROGRESS_)			//归约中出错
		return;
	size_t semanticSize = m_pSemanticStack->size();
	const CToken& rToken = (*m_pSemanticStack)[semanticSize - 1];	//语义栈顶token，只能是ESP之一
	BYTE nonTerminal = 0;		//语义栈顶非终结符类型
	switch (rToken.getType())
	{
	case _TOKEN_E_CONST_:
	case _TOKEN_E_VAR_:
	case _TOKEN_E_TEMP_:
		nonTerminal = _COLUMN_E_;
		break;
	case _TOKEN_S_:
		nonTerminal = _COLUMN_S_;
		break;
	case _TOKEN_P_:
		nonTerminal = _COLUMN_P_;
		break;
	default:
		m_status = _STATUS_UNEXPECTED_ERR_;
		return;
	}
	size_t statusSize = m_pStatusStack->size();
	BYTE curState = (*m_pStatusStack)[statusSize - 1];	//栈顶状态
	m_pAction = *(m_LRTable + curState) + nonTerminal;
	if (m_pAction->getType() != _ACTION_SFT_)
	{
		m_status = _STATUS_UNEXPECTED_ERR_;
		return;
	}
	m_pStatusStack->push_back(m_pAction->getValue());
}

void CMInterpreterBE::accept()
{
	size_t semanticSize = m_pSemanticStack->size();
	CToken& S = (*m_pSemanticStack)[semanticSize - 1];
	size_t quadNum = m_pQuadStack->size();
	backpatch(S, quadNum);			//拉链回填
	m_pSemanticStack->pop_back();	//语义栈应该只剩一个S，弹出S
	m_pStatusStack->pop_back();		//状态栈应该有0和1，弹出1
	if (m_lastToken.getType() == _TOKEN_NONE_)	//acc后发现最新读进来的token都没了，只是归类到了分号，那么整个解析就结束了
	{
		m_status = _STATUS_PARSE_SUCCESS_;
		CQuadruple quad;
		quad.setOperation(_TOKEN_FIN_);		//结束四元式
		m_pQuadStack->push_back(quad);
		printQuad();	//打印四元式
		if (m_pLexFile)
		{
			fclose(m_pLexFile);	//词法分析可以关了
			m_pLexFile = 0;
		}
		delete m_pBuffer;
		m_pBuffer = 0;
	}
		
	else		//否则继续，能归约一定拿着下一个符合文法的词，不需任何额外处理，继续解析
		;
	m_tempNum = 0;
}

void CMInterpreterBE::printQuad()
{
	if (!m_pQuadFile)
		return;
	string temp;
	const CQuadruple* pQuad = m_pQuadStack->data();
	DWORD num = m_pQuadStack->size();
	for (int i = 0; i < num; i++)
	{
		pQuad->toString(temp);
		fprintf(m_pQuadFile, "%d:%s\n", i, temp.data());
		fflush(m_pQuadFile);
		pQuad++;
	}
	fclose(m_pQuadFile);
	m_pQuadFile = 0;
}

BYTE CMInterpreterBE::calculate()
{
	m_quadIndex = 0;
	while (1)
	{
		const CQuadruple& quad = m_pQuadStack->at(m_quadIndex);
		m_operation = quad.getAll(&m_pData1, &m_pData2, &m_pResult);
		switch (m_operation)
		{
			//二元运算
		case _TOKEN_ADD_:	//+
		case _TOKEN_SUB_:	//-
		case _TOKEN_MUL_:	//*
		case _TOKEN_DIV_:	// /
		case _TOKEN_POW_:	//^
		case _TOKEN_GRT_:	//>
		case _TOKEN_GRE_:	//>=
		case _TOKEN_LES_:	//<
		case _TOKEN_LEE_:	//<=
		case _TOKEN_EQL_:	//==
		case _TOKEN_NEQ_:	//~=
		case _TOKEN_OR_:	//|
		case _TOKEN_AND_:	//&
			handleBinaryOperation();
			break;
			//一元运算
		case _TOKEN_ASN_:	//=
		case _TOKEN_NOT_:	//~
		case _TOKEN_USUB_:	//一元减法
		case _TOKEN_JZ_:	//表达式为0跳转
			handleUnaryOperation();
			break;
		case _TOKEN_JMP_:			//无条件跳转
			m_quadIndex = m_pResult->getNum();
			break;
		case _TOKEN_OUTPUT_:
		{
			map<string, double>::const_iterator iter = m_pVarList->find(m_pData1->getName());
			if (iter == m_pVarList->end())
			{
				m_status = _STATUS_TOKEN_NOT_FOUND_;
				_stprintf(m_errStr, _T("%s %s\r\n\r\n"), m_errStrList[m_status], CA2T(m_pData1->getName()));
				return m_status;
			}

			::SendMessage(AfxGetMainWnd()->m_hWnd, WM_SHOW_CMDWND, (WPARAM)m_pData1->getName(), (LPARAM)&iter->second);
		}
			m_quadIndex++;
			break;
		case _TOKEN_FIN_:
			m_status = _STATUS_CALCULATE_SUCCESS_;
			//for (map<string, double>::const_iterator iter = m_pVarList->begin(); iter != m_pVarList->end(); iter++)
			//	::SendMessage(AfxGetMainWnd()->m_hWnd, WM_UPDATE_WORKSPACE, (WPARAM)iter->first.data(), (LPARAM)&iter->second);
			return m_status;
		case _TOKEN_CLEAR_:
			::SendMessage(AfxGetMainWnd()->m_hWnd, WM_WINDOW_CMD, _TOKEN_CLEAR_, 0);
			m_pVarList->clear();
			m_quadIndex++;
			break;
		case _TOKEN_CLC_:
			::SendMessage(AfxGetMainWnd()->m_hWnd, WM_WINDOW_CMD, _TOKEN_CLC_, 0);
			m_quadIndex++;
			break;
		default:
			m_status = _STATUS_INVALID_OPERATION_;
			return m_status;
		}
		if (m_status != _STATUS_PARSE_SUCCESS_)
			return m_status;
	}
}

double CMInterpreterBE::getTokenValue(const CToken & token)
{
	switch (token.getType())
	{
	case _TOKEN_CONST_:
	case _TOKEN_E_CONST_:
		return atof(token.getName());
	case _TOKEN_VAR_:
	case _TOKEN_E_VAR_:
	{
		map<string, double>::const_iterator iter = m_pVarList->find(token.getName());
		if (iter == m_pVarList->end())
		{
			m_status = _STATUS_TOKEN_NOT_FOUND_;
			_stprintf(m_errStr, _T("%s %s\r\n\r\n"), m_errStrList[m_status], CA2T(token.getName()));
			return 0.0;
		}
		return iter->second;
	}
	case _TOKEN_E_TEMP_:
	{
		DWORD index = token.getNum();	
		if (index >= m_pTempList->size())	//下标应该在0到size闭区间内
		{
			m_status = _STATUS_INVALID_OPERAND_;
			return 0.0;
		}
		return m_pTempList->at(index);
	}
		break;
	default:
		m_status = _STATUS_INVALID_OPERAND_;
	}
	return 0.0;
}

void CMInterpreterBE::assignResult(double value)
{
	switch (m_pResult->getType())
	{
	case _TOKEN_VAR_:
	case _TOKEN_E_VAR_:
		(*m_pVarList)[m_pResult->getName()] = value;
		break;
	case _TOKEN_E_TEMP_:
	{
		size_t tempSize = m_pTempList->size();
		WORD tempNum = m_pResult->getNum();
		if (tempNum > tempSize)						//每次最多增加一个临时变量
		{
			m_status = _STATUS_TEMPSTACK_ERR_;
			return;
		}
		else if (tempNum == tempSize)				//增加一个临时变量
			m_pTempList->push_back(value);
		else
			m_pTempList->at(tempNum) = value;		//无需增加临时变量
	}
	break;
	}
}

void CMInterpreterBE::handleBinaryOperation()
{
	double dOperand1 = getTokenValue(*m_pData1);		//操作数1数值
	if (m_status != _STATUS_PARSE_SUCCESS_)
		return;
	double dOperand2 = getTokenValue(*m_pData2);		//操作数2数值
	if (m_status != _STATUS_PARSE_SUCCESS_)
		return;
	double dResult = 0;									//结果数值
	switch (m_operation)
	{
		//二元运算
	case _TOKEN_ADD_:	//+
		dResult = dOperand1 + dOperand2;
		break;
	case _TOKEN_SUB_:	//-
		dResult = dOperand1 - dOperand2;
		break;
	case _TOKEN_MUL_:	//*
		dResult = dOperand1 * dOperand2;
		break;
	case _TOKEN_DIV_:	// /
		if (dOperand2 == 0.0)
		{
			m_status = _STATUS_DIVIDED_BY_ZERO_;
			_stprintf(m_errStr, _T("%s %f / %f\r\n\r\n"), m_errStrList[m_status], dOperand1, dOperand2);
			return;
		}
		dResult = dOperand1 / dOperand2;
		break;
	case _TOKEN_POW_:	//^
		dResult = pow(dOperand1, dOperand2);
		break;
	case _TOKEN_GRT_:	//>
		dResult = dOperand1 > dOperand2;
		break;
	case _TOKEN_GRE_:	//>=
		dResult = dOperand1 >= dOperand2;
		break;
	case _TOKEN_LES_:	//<
		dResult = dOperand1 < dOperand2;
		break;
	case _TOKEN_LEE_:	//<=
		dResult = dOperand1 <= dOperand2;
		break;
	case _TOKEN_EQL_:	//==
		dResult = dOperand1 == dOperand2;
		break;
	case _TOKEN_NEQ_:	//~=
		dResult = dOperand1 != dOperand2;
		break;
	case _TOKEN_OR_:	//|
		dResult = dOperand1 || dOperand2;
		break;
	case _TOKEN_AND_:	//&
		dResult = dOperand1 && dOperand2;
		break;
	}
	assignResult(dResult);
	m_quadIndex++;
}

void CMInterpreterBE::handleUnaryOperation()
{
	double dOperand1 = getTokenValue(*m_pData1);		//操作数1数值
	if (m_status != _STATUS_PARSE_SUCCESS_)
		return;
	double dResult = 0;									//结果数值
	switch (m_operation)
	{
	case _TOKEN_ASN_:	//=
		dResult = dOperand1;
		::SendMessage(AfxGetMainWnd()->m_hWnd, WM_UPDATE_WORKSPACE, (WPARAM)m_pResult->getName(), (LPARAM)&dResult);
		if (m_pResult->getNum())
			::SendMessage(AfxGetMainWnd()->m_hWnd, WM_SHOW_CMDWND, (WPARAM)m_pResult->getName(), (LPARAM)&dResult);
		break;
	case _TOKEN_NOT_:	//~
		dResult = !dOperand1;
		break;
	case _TOKEN_USUB_:	//一元减法
		dResult = -dOperand1;
		break;
	case _TOKEN_JZ_:	//表达式为0跳转
		if (dOperand1 == 0)
			m_quadIndex = m_pResult->getNum();
		else
			m_quadIndex++;
		return;
	}
	assignResult(dResult);
	m_quadIndex++;
}

void CMInterpreterBE::reset()
{
	m_bufPos = 0;
	m_quadIndex = 0;
	m_status = _STATUS_PROGRESS_;
	m_wordCategory = _INVALID_COLUMN_;
	m_pQuadStack->clear();
	m_quadIndex = 0;
	m_chCategory = _CH_BLANK_;
	m_pStatusStack->clear();
	m_pStatusStack->push_back(0);
	m_pSemanticStack->clear();
	//if (m_pLexFile)
	//{
	//	fclose(m_pLexFile);	//词法分析文件
	//	m_pLexFile = 0;
	//}
	//if (m_pQuadFile)
	//{
	//	fclose(m_pQuadFile);	//四元式文件
	//	m_pQuadFile = 0;
	//}
}

const TCHAR * CMInterpreterBE::getErrStr()
{
	
	switch (m_status)					//由于要填报错字符等，前面已经填写过了
	{
	case _STATUS_UNEXPECTED_CHAR_:
	case _STATUS_CONST_ERR_:
	case _STATUS_SYNTAX_ERR_:
	case _STATUS_BREAK_SANS_LOOP_:
	case _STATUS_DIVIDED_BY_ZERO_:
	case _STATUS_TOKEN_NOT_FOUND_:
		return m_errStr;
	}
	_stprintf(m_errStr, _T("%s\r\n\r\n"), m_errStrList[m_status]);
	return m_errStr;
}

BYTE CMInterpreterBE::getCharacter()
{
	if (m_bufPos >= m_bufferLen)
	{
		m_character = 0;
		m_chCategory = _CH_BLANK_;
		return m_character;
	}
	m_character = m_pBuffer[m_bufPos++];
	m_chCategory = categoryCharacter(m_character);
	return m_character;
}

BOOL CMInterpreterBE::getAValidCharacter()
{
	while (m_chCategory == _CH_BLANK_)					//循环读，直到读到有意义字符或者读完文档
	{
		if (!getCharacter())								//读到文档结尾了
			return FALSE;
		if (m_chCategory == _CH_COMMENT_)				//跳过注释
		{
			while (m_character != '\n')
			{
				if (!getCharacter())								//读到缓冲区字符串结尾了	
					return FALSE;
			}
		}
		//出去时是'\n'，换行在我的文法中用于归约句子，得要
	}
	return TRUE;
}

char CMInterpreterBE::categoryCharacter(char ch)
{
	//数字
	if (ch >= '0' && ch <= '9')
		return _CH_DIGIT_;
	//字符
	if (ch >= 'A' && ch <= 'Z')
		return _CH_LETTER_;
	if (ch >= 'a' && ch <= 'z')
		return _CH_LETTER_;
	////;和换行 表示一句的结束
	//if (ch == ';' || ch == '\n')
	//	return _CH_SEMICOLON_;
	//无意义字符
	if (ch == ' ' || ch == '\t' || ch == '\r')
		return _CH_BLANK_;
	return ch;
}

void CMInterpreterBE::handleIdentifier()
{
	while (1)											//循环读，直到读到有意义字符或者读完文档
	{
		if (!getCharacter())								//读到缓冲区字符串结尾了
			break;
		if (m_chCategory == _CH_LETTER_ || m_chCategory == _CH_DIGIT_)
			m_word[m_wordLen++] = m_character;
		else
			break;
	}
	int resevedNum = sizeof(m_reserved) / sizeof(char*);	//保留字个数
	int i = 0;
	for (; i < resevedNum; i++)
	{
		if (strcmp(m_word, m_reserved[i]) == 0)
			break;
	}
	if (i == resevedNum)	//没找到，是普通变量
	{
		m_lastToken.setType(_TOKEN_VAR_);
		m_lastToken.setName(m_word);
	}
	else
		m_lastToken.setType(_TOKEN_WHILE_ + i);
}

void CMInterpreterBE::handleDigit()
{
	BOOL bDot = FALSE;
	if (m_word[0] == '.')
		bDot = TRUE;		//已有小数点，后边再有小数点就会报错
	while (1)
	{
		if (!getCharacter())							//读到缓冲区字符串结尾了
			break;
		if (m_chCategory == _CH_DIGIT_)				//数字
			m_word[m_wordLen++] = m_character;
		else if (m_chCategory == '.')				//小数点
		{
			m_word[m_wordLen++] = m_character;
			if (bDot)
			{
				m_status = _STATUS_CONST_ERR_;
				_stprintf(m_errStr, _T("%s %s\r\n\r\n"), m_errStrList[m_status], CA2T(m_word));
				return;
			}
			bDot = TRUE;
		}
		else										//否则标识符识别完毕
			break;
	}
	if (bDot && m_wordLen == 1)						//只读了个. 报错
	{
		m_lastToken.setType(_TOKEN_NONE_);
		m_status = _STATUS_UNEXPECTED_CHAR_;
		_stprintf(m_errStr, _T("%s %c\r\n\r\n"), m_errStrList[m_status], '.');
		return;
	}
	m_lastToken.setType(_TOKEN_CONST_);
	m_lastToken.setName(m_word);
}

void CMInterpreterBE::handleAdherence()
{
	if (!getCharacter())								//读到缓冲区字符串结尾了
		return;
	if (m_character == '=')
	{
		m_word[m_wordLen++] = m_character;			//有意义字符往数组里填
		m_lastToken.updateAdherence();				//更新种别码
		m_chCategory = _CH_BLANK_;					//字符已使用，下次重新读
	}
}

void CMInterpreterBE::backpatch(const CToken & token, QUADNUM next)
{
	switch (token.getType())
	{
	case _TOKEN_S_:				//回填只涉及S语句和P段落
	case _TOKEN_P_:
		{
			const list<QUADNUM>* pNextList = token.getNextList();
			if (!pNextList)	//nextlist为空
				return;
			for (list<QUADNUM>::const_iterator iter = pNextList->begin(); iter != pNextList->end(); iter++)
			{
				CQuadruple& rQuad = (*m_pQuadStack)[*iter];		//取出四元式
				rQuad.setJmpDestination(next);
			}
		}
		break;
	default:
		return;
	}
}
