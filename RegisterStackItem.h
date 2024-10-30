#pragma once

class CRegisterStackItem :  public CStackItem
{
public:
	enum RegType {
		NONE,
		X,
		Y
	};
private:
	RegType m_RegType;
public:
	CRegisterStackItem();
	virtual ~CRegisterStackItem();
	bool Create();
	RegType GetRegType() { return m_RegType; }
	void SetRegType(RegType t) { m_RegType = t; }
};

