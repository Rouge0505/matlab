#pragma once
#include "Token.h"

class CQuadruple
{
public:
	CQuadruple();
	CQuadruple(DWORD operation, const CToken& data, const CToken& result);	//һԪ����
	CQuadruple(DWORD operation, const CToken& data1, const CToken& data2, const CToken& result);	//��Ԫ����
	CQuadruple(const CToken& output);	//�������
	CQuadruple(const CQuadruple& another);
	~CQuadruple();
	CQuadruple& operator= (const CQuadruple& another);


	void setAssignOutput();					//����ֵ�������Ϊ��������ڸ�ֵ����Լ�Ҳ��ԷֺŽ�βʱʹ��
	void setJumpZero(const CToken& E);		//jz(EΪ��)��ת��δ������ת��ַ����setJmpDestination���õ�ַ
	void setJmpDestination(QUADNUM dest);	//������ת��ַ�����ڲ�����Ϊjz��jmpʱʹ��
	
	void toString(string& output) const;
	DWORD getAll(const CToken** pData1, const CToken** pData2, const CToken** pResult) const;

	void set(DWORD operation, const CToken& data1, const CToken& data2, const CToken& result);
	void setOperation(DWORD operation);
	void setData1(const CToken& data);
	void setData2(const CToken& data);
	void setResult(const CToken& data);
private:
	DWORD m_operation;		//������
	CToken m_data1;			//������1
	CToken m_data2;			//������2
	CToken m_result;		//���
};

