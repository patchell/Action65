#include "pch.h"

CChainInstruction::CChainInstruction() :CChainItem(ChainItemType::INSTRUCTION)
{
	m_pInstruction = 0;
}

CChainInstruction::~CChainInstruction()
{
}

bool CChainInstruction::Create(CInstruction* pInstruction)
{
	m_pInstruction = pInstruction;
	return true;
}

void CChainInstruction::Copy(CChainItem* pI)
{
	if (pI && (pI->Is(CChainItem::ChainItemType::INSTRUCTION)))
	{
		m_pInstruction = ((CChainInstruction*)pI)->m_pInstruction;
	}
}

bool CChainInstruction::Compare(const char* pName)
{
	bool rV = false;
	if (m_pInstruction)
	{
		if (m_pInstruction->GetLabel())
		{
			if (strcmp(m_pInstruction->GetLabel()->GetSymbol()->GetName(), pName) == 0)
			{
				rV = true;
			}
		}
	}
	return rV;
}
