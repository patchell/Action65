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
		CValue* pLabel = m_pInstruction->GetLabel();
		CValue* pOperand = m_pInstruction->GetOperand();

		if (pLabel)
		{
			if (pLabel->GetSymbol())
			{
				if (pLabel->GetName())
				{
					if(strcmp("DOlabel__19", pLabel->GetName()) == 0)
						printf("DO\n");
				}
			}
		}
		if (m_pInstruction->GetOperand())
		{
			if(m_pInstruction->GetOperand()->GetSymbol())
			{
				if (m_pInstruction->GetOperand()->GetName())
				{
					if (strcmp("DOlabel__19", m_pInstruction->GetOperand()->GetName()) == 0)
						printf("OD\n");
				}
			}
		}
		if(pLabel && pLabel->GetSymbol()->IsUnResolved())
		{
			//mark instruction label as unresolved
			pLabel->SetAddress(addr);
		}
		if (pOperand && pOperand->GetSymbol())
		{
			if (pOperand->GetSymbol()->IsUnResolved())
				printf("Error: Operand symbol not resolved before Emit\n");
			else
			{
				if (m_pInstruction->GetNumBytes() == 3)
					m_pInstruction->SaveWordToOperandField((short)pOperand->GetAddress());
				else if(m_pInstruction->GetNumBytes() == 2)
					m_pInstruction->SetData(1, (char)pOperand->GetAddress());
			}
		}
		pSec->AddData(numBytes, pData, pLabel);
		m_pInstruction->SetAddress(addr);
	}
}
