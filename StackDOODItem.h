#pragma once

class CStackDOODItem : public CStackItem
{
	CValue* m_pLabelValue;
public:
	CStackDOODItem();
	virtual ~CStackDOODItem();
	bool Create(CValue* pLabelVal);
	CValue* GetLabelValue() { return m_pLabelValue; }
};

