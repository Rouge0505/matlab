#pragma once
//耦合关系：_INVALID_COLUMN_长度要与m_wordCategory一致，且LR的实际列不能达到该无效值
#include "Token.h"
#include "Action.h"
#include "Quadruple.h"
#include <vector>
#include <map>
using namespace std;
#define _IDN_MAX_LEN_		61						//标识符最大长度
#define _CH_LETTER_			'A'						//预分类字母
#define _CH_DIGIT_			'0'						//预分类数字
//#define _CH_SEMICOLON_		';'						//预分类分号
#define _CH_BLANK_			0						//预分类空白字符
#define _CH_COMMENT_		'%'						//注释

enum
{
	WM_UPDATE_WORKSPACE = WM_USER + 1000,		//更新工作空间
	WM_SHOW_CMDWND,								//在命令窗口打印变量值
	WM_WINDOW_CMD,								//发送clear或clc窗口命令
};

enum statusList
{
	//解析阶段
	_STATUS_PROGRESS_ = 0,						//正在进行解析
	_STATUS_PARSE_SUCCESS_,						//解析完成，同时也是计算进行中的标志
	_STATUS_UNEXPECTED_CHAR_,					//意外的字符
	//_STATUS_TOKEN_ERR_,							//错误的token，如读到qwer这一无意义的符号
	_STATUS_CONST_ERR_,							//数字错误，如1.2.3
	_STATUS_SYNTAX_ERR_,						//语法错误，如1+*2
	_STATUS_BREAK_SANS_LOOP_,					//break或continue不在循环里面
	//计算阶段
	_STATUS_CALCULATE_SUCCESS_,					//计算成功
	_STATUS_DIVIDED_BY_ZERO_,					//除以0
	_STATUS_TOKEN_NOT_FOUND_,					//标识符不存在
	_STATUS_INVALID_OPERAND_,					//操作数不是常量、用户变量、临时变量，意外的错误
	_STATUS_TEMPSTACK_ERR_,						//临时变量栈错误
	_STATUS_INVALID_OPERATION_,					//无效的四元式操作符
	_STATUS_UNEXPECTED_ERR_,					//意外的错误，兜底
};

enum LR_ColumnList			//LR表的列没有语法分析那么详细，如*/^都会当做*，以减小LR表的尺寸，要先预处理语法分析结果到该表
{
	_COLUMN_S_ = 0,			//S=statement
	_COLUMN_SEMCLN_,		//;
	_COLUMN_E_,				//E=expression
	_COLUMN_VAR_,			//var
	_COLUMN_IF_,			//if
	_COLUMN_WHILE_,			//while
	_COLUMN_SLP_,			//(
	_COLUMN_ADD_,			//+，包括+-~
	_COLUMN_CONST_,			//常量
	_COLUMN_MUL_,			//*，包括*/
	_COLUMN_POW_,			//^
	_COLUMN_GRT_,			//包括> >= < <= == ~=
	_COLUMN_OR_,			//|
	_COLUMN_AND_,			//&
	_COLUMN_ASN_,			//=
	_COLUMN_SRP_,			//)
	_COLUMN_P_,				//P=paragraph
	_COLUMN_END_,			//end
	_COLUMN_ELSE_,			//else
	_COLUMN_BREAK_,			//break
};
#define _INVALID_COLUMN_	0xFFFF		//无效列

class CMInterpreterBE
{
public:
	CMInterpreterBE();
	~CMInterpreterBE();
	//BOOL open(const TCHAR* path);
	BOOL open(const TCHAR* folderPath, const TCHAR* fileName);	//打开folderPath路径下的fileName文件，成功TURE失败FALSE
	void setStatement(const char* statement, const TCHAR* workPath);	//设置待解析的语句，传入工作路径以存储过程文件
	BYTE run();									//运行=解析+计算
	const TCHAR* getErrStr();					//获取报错信息
private:
	//解析阶段，包含词法分析、语法分析、语义分析、中间代码生成
	BYTE parse();
	//词法分析
	const char* getWord();						//进行词法分析，获取下一个单词
	BYTE getCharacter();						//读取一个字符，填进m_character，并归类。读到头了填0。返回字符本身
	BOOL getAValidCharacter();					//读取一个有意义的字符，成功返回TRUE，字符及种类存储在m_character和m_chCategory，读到文档结尾仍无有效字符返回FALSE
	char categoryCharacter(char ch);			//预分类标识符，如果为数字返回'0'，如果为字母（包含_）返回'A'，如果为空返回' '，否则返回本身
	void handleIdentifier();					//处理标识符
	void handleDigit();							//处理数字
	void handleAdherence();						//处理可能后继连词的字符，如>判断是否为>=
	//语法分析
	void categoryWord();						//分类语法分析出来的词，分类结果见LR_ColumnList
	void shift();								//移进，更新状态栈和语义栈，为简化LR表，移进也会有部分语义动作
	void reduce();								//归约
	void reduce1();								//按01号产生式进行归约 S->;
	void reduce2();								//按02号产生式进行归约 S->E;
	void reduce3();								//按03号产生式进行归约 S->var=E;
	void reduce4();								//按04号产生式进行归约 S->if E; P end
	void reduce5();								//按05号产生式进行归约 S->if E; P else P end
	void reduce6();								//按06号产生式进行归约 S->while E; P end
	void reduce7();								//按07号产生式进行归约 E->E+E
	void reduce8();								//按08号产生式进行归约 E->E*E
	void reduce9();								//按09号产生式进行归约 E->E^E
	void reduce10();							//按10号产生式进行归约 E->E>E
	void reduce11();							//按11号产生式进行归约 E->E|E
	void reduce12();							//按12号产生式进行归约 E->E&E
	void reduce13();							//按13号产生式进行归约 E->(E)
	void reduce14();							//按14号产生式进行归约 E->+E
	void reduce15();							//按15号产生式进行归约 E->var
	void reduce16();							//按16号产生式进行归约 E->const
	void reduce17();							//按17号产生式进行归约 P->S
	void reduce18();							//按18号产生式进行归约 P->PS
	void reduce19();							//按19号产生式进行归约 S->break
	void handleStatusStack();					//归约完统一处理状态栈，即上个状态识别ESP到达哪个状态
	void accept();								//接受
	void backpatch(const CToken& token, QUADNUM next);						//拉链回填，遍历token的四元式编号list<QUADNUM>，取出每一个QUADNUM，四元式栈对应每一条跳转语句，回填目标next
	void printQuad();							//测试语法分析，生成四元式写进记事本
	//计算阶段
	BYTE calculate();							//遍历四元式进行计算
	double getTokenValue(const CToken& token);	//获取token值，因为涉及到了变量列表，写进解释器类而非token类
	void assignResult(double value);			//给四元式结果赋值
	void handleBinaryOperation();				//处理二元运算
	void handleUnaryOperation();				//处理一元运算
	//重置，进行下一次解释
	void reset();

private:
	char* m_pBuffer;							//读文件后的缓存区
	long m_bufferLen;							//缓冲区有效长度，不包含结尾的0，单位字节
	long m_bufPos;								//读到哪个位置了
	BYTE m_status;								//解析状态
	char m_character;							//每次读进来一个字符
	char m_chCategory;							//预分类字符类别
	BYTE m_wordLen;								//本次获取时单词已读取长度
	char m_word[_IDN_MAX_LEN_];					//本次获取单词时用于存放的数组
	CToken m_lastToken;							//上次识别的token，初始种别为_TOKEN_NONE_，用于+-区分是正负号还是运算符
	vector<BYTE>* m_pStatusStack;				//状态栈
	vector<CToken>* m_pSemanticStack;			//语义栈
	WORD m_wordCategory;						//预分类词类
	WORD m_tempNum;								//临时变量数，每次acc后清零
	DWORD m_quadNum;							//四元式个数
	vector<CQuadruple>* m_pQuadStack;			//四元式栈
	const CAction* m_pAction;					//当前动作
	FILE* m_pLexFile;							//词法分析结果文件
	FILE* m_pQuadFile;							//四元式结果文件
	vector<QUADNUM>* m_pLoopStack;				//每读进一个循环标识符while for，将其语义栈下标记录下来
	static const char* m_reserved[];			//函数名，静态方便初始化，也方便多线程调用
	static const CAction m_LRTable[44][20];		//LR表，静态方便初始化，也方便多线程调用
	//计算阶段
	map<string, double>* m_pVarList;			//变量名-变量值
	vector<double>* m_pTempList;					//临时变量只有编号
	DWORD m_operation;					//处理四元式时，取出其内容
	const CToken* m_pData1;
	const CToken* m_pData2;
	const CToken* m_pResult;
	QUADNUM m_quadIndex;		//待处理四元式下标，从0开始
	static const TCHAR* m_errStrList[];		//报错信息字符串
	TCHAR m_errStr[128];			//错误信息
};

