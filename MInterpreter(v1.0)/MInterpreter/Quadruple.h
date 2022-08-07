#pragma once
#include "Token.h"

class CQuadruple
{
public:
	CQuadruple();
	CQuadruple(DWORD operation, const CToken& data, const CToken& result);	//一元操作
	CQuadruple(DWORD operation, const CToken& data1, const CToken& data2, const CToken& result);	//二元操作
	CQuadruple(const CToken& output);	//输出操作
	CQuadruple(const CQuadruple& another);
	~CQuadruple();
	CQuadruple& operator= (const CQuadruple& another);


	void setAssignOutput();					//将赋值语句设置为输出，仅在赋值语句归约且不以分号结尾时使用
	void setJumpZero(const CToken& E);		//jz(E为假)跳转，未设置跳转地址，调setJmpDestination设置地址
	void setJmpDestination(QUADNUM dest);	//设置跳转地址，仅在操作码为jz和jmp时使用
	
	void toString(string& output) const;
	DWORD getAll(const CToken** pData1, const CToken** pData2, const CToken** pResult) const;

	void set(DWORD operation, const CToken& data1, const CToken& data2, const CToken& result);
	void setOperation(DWORD operation);
	void setData1(const CToken& data);
	void setData2(const CToken& data);
	void setResult(const CToken& data);
private:
	DWORD m_operation;		//操作码
	CToken m_data1;			//操作数1
	CToken m_data2;			//操作数2
	CToken m_result;		//结果
};

