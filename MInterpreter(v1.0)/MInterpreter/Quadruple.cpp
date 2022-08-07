#include "stdafx.h"
#include "Quadruple.h"


CQuadruple::CQuadruple()
{
	m_operation = 0;	//CToken有默认构造函数，这里就不用再初始化了
}

CQuadruple::CQuadruple(DWORD operation, const CToken & data, const CToken & result)
{
	m_operation = operation;
	m_data1 = data;
	m_result = result;
}

CQuadruple::CQuadruple(DWORD operation, const CToken & data1, const CToken & data2, const CToken & result)
{
	m_operation = operation;
	m_data1 = data1;
	m_data2 = data2;
	m_result = result;
}

CQuadruple::CQuadruple(const CToken & output)
{
	m_operation = _TOKEN_OUTPUT_;
	m_data1 = output;
}

CQuadruple::CQuadruple(const CQuadruple & another)
{
	m_operation = another.m_operation;
	m_data1 = another.m_data1;
	m_data2 = another.m_data2;
	m_result = another.m_result;
}


CQuadruple::~CQuadruple()
{
}

CQuadruple & CQuadruple::operator=(const CQuadruple & another)
{
	m_operation = another.m_operation;
	m_data1 = another.m_data1;
	m_data2 = another.m_data2;
	m_result = another.m_result;
	return *this;
}

void CQuadruple::set(DWORD operation, const CToken & data1, const CToken & data2, const CToken & result)
{
	m_operation = operation;
	m_data1 = data1;
	m_data2 = data2;
	m_result = result;
}

void CQuadruple::setOperation(DWORD operation)
{
	m_operation = operation;
}

void CQuadruple::setData1(const CToken & data)
{
	m_data1 = data;
}

void CQuadruple::setData2(const CToken & data)
{
	m_data2 = data;
}

void CQuadruple::setResult(const CToken & data)
{
	m_result = data;
}

void CQuadruple::toString(string & output) const
{
	//(操作符,\t操作数1,\t操作数2,\t结果)
	string temp;
	output = "(";
	output += CToken::m_tokenStrList[m_operation];	//操作符
	output += ",\t";
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
		m_data1.toString(temp);
		output += temp;
		output += ",\t";
		m_data2.toString(temp);
		output += temp;
		output += ",\t";
		m_result.toString(temp);
		output += temp;
		break;
		
	//一元运算
	case _TOKEN_ASN_:	//=
	case _TOKEN_NOT_:	//~
	case _TOKEN_JZ_:	//表达式为0跳转
	case _TOKEN_USUB_:	//一元减法
		m_data1.toString(temp);
		output += temp;
		output += ",\t,\t";
		m_result.toString(temp);
		output += temp;
		break;
	case _TOKEN_JMP_:			//无条件跳转
		output += ",\t,\t";
		m_result.toString(temp);
		output += temp;
		break;
	//结尾
	case _TOKEN_FIN_:
	case _TOKEN_CLEAR_:			//clear
	case _TOKEN_CLC_:			//clc
		output += ",\t,\t";
		break;
	case _TOKEN_OUTPUT_:
		m_data1.toString(temp);
		output += temp;
		output += ",\t,\t";
		break;
	}
	output += ")";
	if (m_operation == _TOKEN_ASN_)	//如果是赋值，判断是否输出
	{
		if (m_result.getNum())
			output += "\t output";
	}
}

void CQuadruple::setAssignOutput()
{
	ASSERT(m_operation == _TOKEN_ASN_);	//仅赋值语句会用到设置输出
	ASSERT(m_result.getType() == _TOKEN_E_VAR_ || m_result.getType() == _TOKEN_VAR_);	//仅用户变量会输出，包含ans
	m_result.setAssignOutput();
}

void CQuadruple::setJmpDestination(QUADNUM dest)
{
	ASSERT(m_operation == _TOKEN_JMP_ || m_operation == _TOKEN_JZ_);//仅跳转语句使用
	m_result.setType(m_operation);
	m_result.setJmpDestination(dest);
}

void CQuadruple::setJumpZero(const CToken & E)
{
	m_operation = _TOKEN_JZ_;
	m_data1 = E;
}

DWORD CQuadruple::getAll(const CToken ** pData1, const CToken ** pData2, const CToken ** pResult) const
{
	*pData1 = &m_data1;
	*pData2 = &m_data2;
	*pResult = &m_result;
	return m_operation;
}
