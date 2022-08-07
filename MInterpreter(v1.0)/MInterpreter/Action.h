#pragma once
enum actionList
{
	_ACTION_ERR_ = 0,		//����
	_ACTION_ACC_,			//���ܣ��﷨�����ɹ�
	_ACTION_SFT_,			//�ƽ�
	_ACTION_RDC_,			//��Լ

};

#define _ERR_	CAction()
#define _ACC_	CAction(_ACTION_ACC_)
#define _S(i)	CAction(_ACTION_SFT_, i)
#define _R(i)	CAction(_ACTION_RDC_, i)

class CAction				//LR�﷨�������ÿһ������
{
public:
	CAction();
	CAction(BYTE type, BYTE value = 0);
	~CAction();
	BYTE getType() const;			//��ȡ����
	BYTE getValue() const;		//��ȡ����ֵ
private:
	BYTE m_type;			//�������࣬��actionList
	BYTE m_value;			//����ֵ���ƽ�ʱ��ʾ��ת���ĸ�״̬����Լʱ��ʾ���ĸ�����ʽ��Լ
};

