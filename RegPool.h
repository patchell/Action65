#pragma once


class CRegPool
{
	CReg m_aRegisters[6];;
public:
	CRegPool();
	virtual ~CRegPool() {}
	CReg* Lock(CReg::RegType Register);
	void UnLock(CReg::RegType Register);
};


