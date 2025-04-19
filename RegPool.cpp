#include "pch.h"


CRegPool::CRegPool()
{
	m_aRegisters[0].Lock();
	m_aRegisters[0].SetType(CReg::RegType::NONE);
	// Accumulator
	m_aRegisters[int(CReg::RegType::A)].UnLock();
	m_aRegisters[int(CReg::RegType::A)].SetType(CReg::RegType::A);
	// X index register
	m_aRegisters[int(CReg::RegType::X)].UnLock();
	m_aRegisters[int(CReg::RegType::X)].SetType(CReg::RegType::X);
	// Y index register
	m_aRegisters[int(CReg::RegType::Y)].UnLock();
	m_aRegisters[int(CReg::RegType::Y)].SetType(CReg::RegType::Y);
	// Stack Pointer
	m_aRegisters[int(CReg::RegType::S)].UnLock();
	m_aRegisters[int(CReg::RegType::S)].SetType(CReg::RegType::S);
	// Status Register
	m_aRegisters[int(CReg::RegType::P)].UnLock();
	m_aRegisters[int(CReg::RegType::P)].SetType(CReg::RegType::P);
}

CReg* CRegPool::Lock(CReg::RegType Register)
{
	CReg* pReg = 0;

	if (m_aRegisters[int(Register)].IsUnLocked())
	{
		m_aRegisters[int(Register)].Lock();
		pReg = &m_aRegisters[int(Register)];
	}
	else
	{
		fprintf(Act()->LogFile(), "%s Register Already In Use\n",
			m_aRegisters[int(Register)].ToString()
		);
		fprintf(stderr, "Internal Error:%s Register Already In Use\n",
			m_aRegisters[int(Register)].ToString()
		);
	}
	return pReg;
}

void CRegPool::UnLock(CReg::RegType Register)
{
	m_aRegisters[int(Register)].UnLock();
}
