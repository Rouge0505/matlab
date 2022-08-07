#pragma once
#include <list>
#include <string>
using namespace std;
//��Ϲ�ϵ��>=Ҫ��>��1��<,=,~ͬ��
//��Ϲ�ϵ��tokenList��while��ʼ�ı�����˳��Ҫ�ͽ�����������˳��һ��
//��Ϲ�ϵ��tokenListҪ��CToken��m_tokenStrList˳��һ�£��ҷ��ս����ǰ
typedef WORD QUADNUM;
typedef double MCONST;
enum tokenList
{
	_TOKEN_NONE_ = 0,		//��ʼ��ʱû�з���
	_TOKEN_CONST_,			//����
	_TOKEN_VAR_,			//����
	_TOKEN_WHILE_,			//while
	_TOKEN_BREAK_,			//break
	_TOKEN_CONTN_,			//continue
	_TOKEN_END_,			//end
	_TOKEN_IF_,				//if
	_TOKEN_ELSE_,			//else
	_TOKEN_CLEAR_,			//clear
	_TOKEN_CLC_,			//clc
	_TOKEN_ADD_,			//+
	_TOKEN_SUB_,			//-
	_TOKEN_MUL_,			//*
	_TOKEN_DIV_,			// /
	_TOKEN_POW_,			//^
	_TOKEN_SLP_,			//(
	_TOKEN_SRP_,			//)
	_TOKEN_GRT_,			//>
	_TOKEN_GRE_,			//>=
	_TOKEN_LES_,			//<
	_TOKEN_LEE_,			//<=
	_TOKEN_ASN_,			//=
	_TOKEN_EQL_,			//==
	_TOKEN_NOT_,			//~
	_TOKEN_NEQ_,			//~=
	_TOKEN_NEWL_,			//����
	_TOKEN_SEMCLN_,			//;
	_TOKEN_OR_,				//|
	_TOKEN_AND_,			//&
	//����Ϊ��Ԫʽר��token
	_TOKEN_OUTPUT_,			//��ӡ�������
	_TOKEN_USUB_,			//һԪ����
	_TOKEN_JZ_,				//���ʽΪ0��ת
	_TOKEN_JMP_,			//��������ת
	_TOKEN_FIN_,			//la fin,��Ԫʽ�������һ��
	//����Ϊ���ս��
	_TOKEN_E_CONST_,		//expression-constant
	_TOKEN_E_VAR_,			//expression-variable
	_TOKEN_E_TEMP_,			//expression-temporary
	_TOKEN_S_,				//sentence
	_TOKEN_P_,				//paragraph
	
	_TOKEN_TOTAL_,			//����
	//_TOKEN_END_,			//�������ţ���ʵ����Ϊ�ַ�����β��0
};
//+-��NONE���������(��Ϊ�����ţ��ڳ�����)��Ϊ�����

class CToken
{
public:
	CToken();
	CToken(DWORD type, char* name = NULL);	//��������Ϊtype������Ϊname�ĵ�token
	CToken(const CToken& another);
	~CToken();
	void setType(DWORD type);			//�����ֱ���
	void setNum(DWORD num);				//��ʱ��������������ֵ
	void setAssignOutput(BOOL bOutput = TRUE);				//���ø�ֵtoken�Ƿ��ӡ���
	void setJmpDestination(QUADNUM dest);//jmp��jz������ת��ַ
	void setName(char* name);			//�û��������ñ�����
	void setNextList(const list<QUADNUM>* pNextlist);	//����nextlist
	void copyNextList(const CToken& another);	//�����reset��ʹ��
	void mergetNextList(const CToken& another);
	void copyParamList(const CToken& another);
	DWORD getType() const;						//��ȡ�ֱ���
	DWORD getNum() const;						//��ȡ����m_num
	QUADNUM getJmpDestination() const;			//��ȡjmp��jz��ת��ַ
	BOOL assignNeedsOutput() const;				//�����ֵtoken��Ҫ��ӡ��������ط��㣬���򷵻�0
	const list<QUADNUM>* getNextList() const;	//��ȡnextlist
	const char* getName() const;				//��ȡ�û�������
	CToken& operator= (const CToken& another);
	void updateAdherence();	//����><=~��������=�����ʾ��һ�����壬�����ֱ���
	void toString(string& output) const;	//��tokenת��Ϊ�ַ���
	static int getMaxTemp(const CToken& token1, const CToken& token2);	//�������token��������ʱ����������-1�����򷵻���ʱ������Žϴ���
	void push_backQuadNum(QUADNUM num);	//��S,while,P token��list<QUADNUM>������Ӵ�������Ԫʽ��
	void reset();						//����Ϊ��
	void clearPointers();				//��������ָ��ָ��Ķ��ڴ沢��ָ����0
private:
	WORD m_type;			//�ֱ𣬼�tokenList
	WORD m_num;				//��ʱ�����ı��  ; else while P��¼������  ���result���û���������ֵΪ1��ʾ��ӡ���
	//void* m_ptr;			//����new double�׵�ַ���û�����new char[]�׵�ַ��SP��nextlistָ�룬NULL��ʾû�У�while��nextlist
	
	//MCONST* m_pValue;			//����ֵ
	char* m_pName;				//�������ַ���
	list<QUADNUM>* m_pNextList;	//S��P��nextlistָ�룬NULL��ʾû�У�while��nextlist
	list<CToken>* m_pParamList;	//iden��paramlist���ڡ����롱�׶��Գ�������������ʱ������list�洢������׶����μ�����ֵ����MCONST��list�����뺯��
public:
	static char* const m_tokenStrList[];
};

