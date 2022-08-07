#pragma once
//��Ϲ�ϵ��_INVALID_COLUMN_����Ҫ��m_wordCategoryһ�£���LR��ʵ���в��ܴﵽ����Чֵ
#include "Token.h"
#include "Action.h"
#include "Quadruple.h"
#include <vector>
#include <map>
using namespace std;
#define _IDN_MAX_LEN_		61						//��ʶ����󳤶�
#define _CH_LETTER_			'A'						//Ԥ������ĸ
#define _CH_DIGIT_			'0'						//Ԥ��������
//#define _CH_SEMICOLON_		';'						//Ԥ����ֺ�
#define _CH_BLANK_			0						//Ԥ����հ��ַ�
#define _CH_COMMENT_		'%'						//ע��

enum
{
	WM_UPDATE_WORKSPACE = WM_USER + 1000,		//���¹����ռ�
	WM_SHOW_CMDWND,								//������ڴ�ӡ����ֵ
	WM_WINDOW_CMD,								//����clear��clc��������
};

enum statusList
{
	//�����׶�
	_STATUS_PROGRESS_ = 0,						//���ڽ��н���
	_STATUS_PARSE_SUCCESS_,						//������ɣ�ͬʱҲ�Ǽ�������еı�־
	_STATUS_UNEXPECTED_CHAR_,					//������ַ�
	//_STATUS_TOKEN_ERR_,							//�����token�������qwer��һ������ķ���
	_STATUS_CONST_ERR_,							//���ִ�����1.2.3
	_STATUS_SYNTAX_ERR_,						//�﷨������1+*2
	_STATUS_BREAK_SANS_LOOP_,					//break��continue����ѭ������
	//����׶�
	_STATUS_CALCULATE_SUCCESS_,					//����ɹ�
	_STATUS_DIVIDED_BY_ZERO_,					//����0
	_STATUS_TOKEN_NOT_FOUND_,					//��ʶ��������
	_STATUS_INVALID_OPERAND_,					//���������ǳ������û���������ʱ����������Ĵ���
	_STATUS_TEMPSTACK_ERR_,						//��ʱ����ջ����
	_STATUS_INVALID_OPERATION_,					//��Ч����Ԫʽ������
	_STATUS_UNEXPECTED_ERR_,					//����Ĵ��󣬶���
};

enum LR_ColumnList			//LR�����û���﷨������ô��ϸ����*/^���ᵱ��*���Լ�СLR��ĳߴ磬Ҫ��Ԥ�����﷨����������ñ�
{
	_COLUMN_S_ = 0,			//S=statement
	_COLUMN_SEMCLN_,		//;
	_COLUMN_E_,				//E=expression
	_COLUMN_VAR_,			//var
	_COLUMN_IF_,			//if
	_COLUMN_WHILE_,			//while
	_COLUMN_SLP_,			//(
	_COLUMN_ADD_,			//+������+-~
	_COLUMN_CONST_,			//����
	_COLUMN_MUL_,			//*������*/
	_COLUMN_POW_,			//^
	_COLUMN_GRT_,			//����> >= < <= == ~=
	_COLUMN_OR_,			//|
	_COLUMN_AND_,			//&
	_COLUMN_ASN_,			//=
	_COLUMN_SRP_,			//)
	_COLUMN_P_,				//P=paragraph
	_COLUMN_END_,			//end
	_COLUMN_ELSE_,			//else
	_COLUMN_BREAK_,			//break
};
#define _INVALID_COLUMN_	0xFFFF		//��Ч��

class CMInterpreterBE
{
public:
	CMInterpreterBE();
	~CMInterpreterBE();
	//BOOL open(const TCHAR* path);
	BOOL open(const TCHAR* folderPath, const TCHAR* fileName);	//��folderPath·���µ�fileName�ļ����ɹ�TUREʧ��FALSE
	void setStatement(const char* statement, const TCHAR* workPath);	//���ô���������䣬���빤��·���Դ洢�����ļ�
	BYTE run();									//����=����+����
	const TCHAR* getErrStr();					//��ȡ������Ϣ
private:
	//�����׶Σ������ʷ��������﷨����������������м��������
	BYTE parse();
	//�ʷ�����
	const char* getWord();						//���дʷ���������ȡ��һ������
	BYTE getCharacter();						//��ȡһ���ַ������m_character�������ࡣ����ͷ����0�������ַ�����
	BOOL getAValidCharacter();					//��ȡһ����������ַ����ɹ�����TRUE���ַ�������洢��m_character��m_chCategory�������ĵ���β������Ч�ַ�����FALSE
	char categoryCharacter(char ch);			//Ԥ�����ʶ�������Ϊ���ַ���'0'�����Ϊ��ĸ������_������'A'�����Ϊ�շ���' '�����򷵻ر���
	void handleIdentifier();					//�����ʶ��
	void handleDigit();							//��������
	void handleAdherence();						//������ܺ�����ʵ��ַ�����>�ж��Ƿ�Ϊ>=
	//�﷨����
	void categoryWord();						//�����﷨���������Ĵʣ���������LR_ColumnList
	void shift();								//�ƽ�������״̬ջ������ջ��Ϊ��LR���ƽ�Ҳ���в������嶯��
	void reduce();								//��Լ
	void reduce1();								//��01�Ų���ʽ���й�Լ S->;
	void reduce2();								//��02�Ų���ʽ���й�Լ S->E;
	void reduce3();								//��03�Ų���ʽ���й�Լ S->var=E;
	void reduce4();								//��04�Ų���ʽ���й�Լ S->if E; P end
	void reduce5();								//��05�Ų���ʽ���й�Լ S->if E; P else P end
	void reduce6();								//��06�Ų���ʽ���й�Լ S->while E; P end
	void reduce7();								//��07�Ų���ʽ���й�Լ E->E+E
	void reduce8();								//��08�Ų���ʽ���й�Լ E->E*E
	void reduce9();								//��09�Ų���ʽ���й�Լ E->E^E
	void reduce10();							//��10�Ų���ʽ���й�Լ E->E>E
	void reduce11();							//��11�Ų���ʽ���й�Լ E->E|E
	void reduce12();							//��12�Ų���ʽ���й�Լ E->E&E
	void reduce13();							//��13�Ų���ʽ���й�Լ E->(E)
	void reduce14();							//��14�Ų���ʽ���й�Լ E->+E
	void reduce15();							//��15�Ų���ʽ���й�Լ E->var
	void reduce16();							//��16�Ų���ʽ���й�Լ E->const
	void reduce17();							//��17�Ų���ʽ���й�Լ P->S
	void reduce18();							//��18�Ų���ʽ���й�Լ P->PS
	void reduce19();							//��19�Ų���ʽ���й�Լ S->break
	void handleStatusStack();					//��Լ��ͳһ����״̬ջ�����ϸ�״̬ʶ��ESP�����ĸ�״̬
	void accept();								//����
	void backpatch(const CToken& token, QUADNUM next);						//�����������token����Ԫʽ���list<QUADNUM>��ȡ��ÿһ��QUADNUM����Ԫʽջ��Ӧÿһ����ת��䣬����Ŀ��next
	void printQuad();							//�����﷨������������Ԫʽд�����±�
	//����׶�
	BYTE calculate();							//������Ԫʽ���м���
	double getTokenValue(const CToken& token);	//��ȡtokenֵ����Ϊ�漰���˱����б�д�������������token��
	void assignResult(double value);			//����Ԫʽ�����ֵ
	void handleBinaryOperation();				//�����Ԫ����
	void handleUnaryOperation();				//����һԪ����
	//���ã�������һ�ν���
	void reset();

private:
	char* m_pBuffer;							//���ļ���Ļ�����
	long m_bufferLen;							//��������Ч���ȣ���������β��0����λ�ֽ�
	long m_bufPos;								//�����ĸ�λ����
	BYTE m_status;								//����״̬
	char m_character;							//ÿ�ζ�����һ���ַ�
	char m_chCategory;							//Ԥ�����ַ����
	BYTE m_wordLen;								//���λ�ȡʱ�����Ѷ�ȡ����
	char m_word[_IDN_MAX_LEN_];					//���λ�ȡ����ʱ���ڴ�ŵ�����
	CToken m_lastToken;							//�ϴ�ʶ���token����ʼ�ֱ�Ϊ_TOKEN_NONE_������+-�����������Ż��������
	vector<BYTE>* m_pStatusStack;				//״̬ջ
	vector<CToken>* m_pSemanticStack;			//����ջ
	WORD m_wordCategory;						//Ԥ�������
	WORD m_tempNum;								//��ʱ��������ÿ��acc������
	DWORD m_quadNum;							//��Ԫʽ����
	vector<CQuadruple>* m_pQuadStack;			//��Ԫʽջ
	const CAction* m_pAction;					//��ǰ����
	FILE* m_pLexFile;							//�ʷ���������ļ�
	FILE* m_pQuadFile;							//��Ԫʽ����ļ�
	vector<QUADNUM>* m_pLoopStack;				//ÿ����һ��ѭ����ʶ��while for����������ջ�±��¼����
	static const char* m_reserved[];			//����������̬�����ʼ����Ҳ������̵߳���
	static const CAction m_LRTable[44][20];		//LR����̬�����ʼ����Ҳ������̵߳���
	//����׶�
	map<string, double>* m_pVarList;			//������-����ֵ
	vector<double>* m_pTempList;					//��ʱ����ֻ�б��
	DWORD m_operation;					//������Ԫʽʱ��ȡ��������
	const CToken* m_pData1;
	const CToken* m_pData2;
	const CToken* m_pResult;
	QUADNUM m_quadIndex;		//��������Ԫʽ�±꣬��0��ʼ
	static const TCHAR* m_errStrList[];		//������Ϣ�ַ���
	TCHAR m_errStr[128];			//������Ϣ
};

