#include "pch.h"

CChainInstructionItem::CChainInstructionItem() :CChainItem(ChainItemType::INSTRUCTION)
{
	m_pInstruction = 0;
}

CChainInstructionItem::~CChainInstructionItem()
{
}

bool CChainInstructionItem::Create(CInstruction* pInstruction)
{
	m_pInstruction = pInstruction;
	CChainItem::Create(m_pInstruction->GetNumBytes());
	return true;
}

void CChainInstructionItem::Copy(CChainItem* pI)
{
	if (pI && (pI->Is(CChainItem::ChainItemType::INSTRUCTION)))
	{
		m_pInstruction = ((CChainInstructionItem*)pI)->m_pInstruction;
	}
}

bool CChainInstructionItem::Compare(const char* pName)
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

int CChainInstructionItem::Print(char* pSO, int l, int Indent, const char* s)
{
	int rV = 0;
	if (m_pInstruction)
	{
		rV = m_pInstruction->Print(pSO, l, Indent, s);
	}
	return rV;
}

void CChainInstructionItem::EmitListing(CSection* pSec)
{
	if (m_pInstruction)
	{
		m_pInstruction->EmitListing();
	}
}

void CChainInstructionItem::Emit(CSection* pSec)
{
	if (m_pInstruction)
	{
		char* pData = m_pInstruction->GetData();
		int numBytes = m_pInstruction->GetNumBytes();
		int addr = pSec->GetLocationCounter();
		pSec->AddData(numBytes, pData, m_pInstruction->GetLabel());
		m_pInstruction->SetAddress(addr);
	}
}
