#pragma once
#include <list>
#include <string>
using namespace std;
//耦合关系，>=要比>大1，<,=,~同理
//耦合关系，tokenList，while开始的保留字顺序要和解释器保留字顺序一致
//耦合关系，tokenList要与CToken的m_tokenStrList顺序一致，且非终结符在前
typedef WORD QUADNUM;
typedef double MCONST;
enum tokenList
{
	_TOKEN_NONE_ = 0,		//初始化时没有符号
	_TOKEN_CONST_,			//常量
	_TOKEN_VAR_,			//变量
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
	_TOKEN_NEWL_,			//换行
	_TOKEN_SEMCLN_,			//;
	_TOKEN_OR_,				//|
	_TOKEN_AND_,			//&
	//以下为四元式专用token
	_TOKEN_OUTPUT_,			//打印输出变量
	_TOKEN_USUB_,			//一元减法
	_TOKEN_JZ_,				//表达式为0跳转
	_TOKEN_JMP_,			//无条件跳转
	_TOKEN_FIN_,			//la fin,四元式结束最后一句
	//以下为非终结符
	_TOKEN_E_CONST_,		//expression-constant
	_TOKEN_E_VAR_,			//expression-variable
	_TOKEN_E_TEMP_,			//expression-temporary
	_TOKEN_S_,				//sentence
	_TOKEN_P_,				//paragraph
	
	_TOKEN_TOTAL_,			//总数
	//_TOKEN_END_,			//结束符号，本实例中为字符串结尾的0
};
//+-在NONE，运算符，(后为正负号，在常量，)后为运算符

class CToken
{
public:
	CToken();
	CToken(DWORD type, char* name = NULL);	//构造类型为type、名称为name的的token
	CToken(const CToken& another);
	~CToken();
	void setType(DWORD type);			//设置种别码
	void setNum(DWORD num);				//临时变量等设置整型值
	void setAssignOutput(BOOL bOutput = TRUE);				//设置赋值token是否打印输出
	void setJmpDestination(QUADNUM dest);//jmp和jz设置跳转地址
	void setName(char* name);			//用户变量设置变量名
	void setNextList(const list<QUADNUM>* pNextlist);	//设置nextlist
	void copyNextList(const CToken& another);	//最好在reset后使用
	void mergetNextList(const CToken& another);
	void copyParamList(const CToken& another);
	DWORD getType() const;						//获取种别码
	DWORD getNum() const;						//获取整型m_num
	QUADNUM getJmpDestination() const;			//获取jmp或jz跳转地址
	BOOL assignNeedsOutput() const;				//如果赋值token需要打印输出，返回非零，否则返回0
	const list<QUADNUM>* getNextList() const;	//获取nextlist
	const char* getName() const;				//获取用户变量名
	CToken& operator= (const CToken& another);
	void updateAdherence();	//对于><=~，如果后接=，则表示另一个含义，更新种别码
	void toString(string& output) const;	//将token转化为字符串
	static int getMaxTemp(const CToken& token1, const CToken& token2);	//如果两个token都不是临时变量，返回-1，否则返回临时变量标号较大者
	void push_backQuadNum(QUADNUM num);	//在S,while,P token的list<QUADNUM>后面添加待回填四元式号
	void reset();						//重置为空
	void clearPointers();				//清理所有指针指向的堆内存并将指针置0
private:
	WORD m_type;			//种别，见tokenList
	WORD m_num;				//临时变量的编号  ; else while P记录的整型  如果result是用户变量，此值为1表示打印输出
	//void* m_ptr;			//常量new double首地址，用户变量new char[]首地址，SP的nextlist指针，NULL表示没有；while的nextlist
	
	//MCONST* m_pValue;			//常量值
	char* m_pName;				//变量名字符串
	list<QUADNUM>* m_pNextList;	//S、P的nextlist指针，NULL表示没有；while的nextlist
	list<CToken>* m_pParamList;	//iden的paramlist，在“编译”阶段以常量、变量、临时变量的list存储，计算阶段依次计算数值产生MCONST的list，传入函数
public:
	static char* const m_tokenStrList[];
};

