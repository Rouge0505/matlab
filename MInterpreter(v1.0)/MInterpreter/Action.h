#pragma once
enum actionList
{
	_ACTION_ERR_ = 0,		//报错
	_ACTION_ACC_,			//接受，语法分析成功
	_ACTION_SFT_,			//移进
	_ACTION_RDC_,			//归约

};

#define _ERR_	CAction()
#define _ACC_	CAction(_ACTION_ACC_)
#define _S(i)	CAction(_ACTION_SFT_, i)
#define _R(i)	CAction(_ACTION_RDC_, i)

class CAction				//LR语法分析表的每一个动作
{
public:
	CAction();
	CAction(BYTE type, BYTE value = 0);
	~CAction();
	BYTE getType() const;			//获取动作
	BYTE getValue() const;		//获取动作值
private:
	BYTE m_type;			//动作种类，见actionList
	BYTE m_value;			//动作值，移进时表示跳转到哪个状态，归约时表示用哪个产生式归约
};

