#include "pch.h"

CVirtualReg::CVirtualReg()
{
	m_MaxRegs = 0;
	m_pLockTabel = 0;
	m_pSection = 0;
	m_BaseAddress = 0;
}

CVirtualReg::~CVirtualReg()
{
	if (m_pLockTabel)
		delete[] m_pLockTabel;
}

bool CVirtualReg::Create(int RHeapSize)
{
	int i = 0;

	m_MaxRegs = RHeapSize;
	m_pLockTabel = new int[m_MaxRegs];
	for (i = 0; i < m_MaxRegs; ++i)
		m_pLockTabel[i] = 0;
	return true;
}

CValue* CVirtualReg::Lock(RegSize size)
{
	return nullptr;
}

bool CVirtualReg::UnLock(VREG* pReg)
{
	return false;
}
