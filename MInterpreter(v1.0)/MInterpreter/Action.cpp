#include "stdafx.h"
#include "Action.h"


CAction::CAction()
{
	memset(this, 0, sizeof(*this));
}

CAction::CAction(BYTE type, BYTE value)
{
	m_type = type;
	m_value = value;
}


CAction::~CAction()
{
}

BYTE CAction::getType() const
{
	return m_type;
}

BYTE CAction::getValue() const
{
	return m_value;
}
