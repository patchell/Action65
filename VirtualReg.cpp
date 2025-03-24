#include "pch.h"

CVirtualReg::CVirtualReg()
{
	m_MaxRegs = 0;
	m_pLockTabel = 0;
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

CVirtualReg::VREG* CVirtualReg::Lock(RegSize size)
{
	VREG* pReg = 0;
	int i = 0;
	bool Loop = true;

	//--- Find a free slot(s)
	for (i = 0; (i < m_MaxRegs) && Loop; ++i)
	{
		if (m_pLockTabel[i] == 0)
		{
			switch (size) 
			{
			case RegSize::SIXTEEN_BITS:
				break;
			case RegSize::EIGHT_BITS:
				break;
			}
		}
	}
	return nullptr;
}

bool CVirtualReg::UnLock(VREG* pReg)
{
	return false;
}
