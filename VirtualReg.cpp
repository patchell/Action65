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

bool CVirtualReg::Create(int RHeapSize, CSection* pSection)
{
	int i = 0;

	m_pSection = pSection;
	m_MaxRegs = RHeapSize;
	m_pLockTabel = new int[m_MaxRegs];
	for (i = 0; i < m_MaxRegs; ++i)
		m_pLockTabel[i] = RegStatus::FREE;
	return true;
}

CValue* CVirtualReg::Lock(RegStatus size, CTypeChain* pTC)
{
	int i;
	int Reg = -1;
	VREG* pVReg = 0;
	CValue* pValue = 0;

	for (i = 0; (i < m_MaxRegs) && (0 > Reg); ++i)
	{
		if (m_pLockTabel[i] == RegStatus::FREE)
		{
			if (RegStatus::LOCKED_WORD == size)
			{
				if (m_pLockTabel[i + 1] == RegStatus::FREE)
				{
					Reg = i;
					m_pLockTabel[i] = RegStatus::LOCKED_WORD;
					m_pLockTabel[i+1] = RegStatus::LOCKED_WORD;
				}
			}
			else
			{
				// Byte sized register
				Reg = i;
				m_pLockTabel[i] = RegStatus::LOCKED_BYTE;
			}
		}
	}
	//------------------------------------------
	// Create register
	//------------------------------------------
	if (Reg >= 0)
	{
		pVReg = new CVirtualReg::VREG;
		pVReg->Create(size,m_BaseAddress + Reg, Reg);
		pValue = new CValue;
		pValue->Create(pVReg);
		
		pVReg->GetSymbol()->SetSection(m_pSection);
	}
	return pValue;
}

bool CVirtualReg::UnLock(CValue* pVRegValue)
{
	bool rV = false;
	VREG* pVReg = 0;
	RegStatus Size;
	int TableIndex = 0;

	pVReg = pVRegValue->GetVirtualReg();
	if (pVReg)	//Is this value a vertual register?
	{
		Size = pVReg->GetSize();
		TableIndex = pVReg->GetTableIndex();
		if (Size == m_pLockTabel[TableIndex])
		{
			if (Size == RegStatus::LOCKED_WORD)
			{
				if (TableIndex + 1 < m_MaxRegs)
				{
					if (Size == m_pLockTabel[TableIndex] + 1)
					{
						m_pLockTabel[TableIndex] = FREE;
						m_pLockTabel[TableIndex + 1] = FREE;
						rV = true;
						delete pVRegValue;
					}
					else
					{
						fprintf(Act()->LogFile(), "Internal Error:Virtual Register Lock Type Mismatch ID=%d\n", pVReg->GetTableIndex());
						Act()->Exit(2);
					}
				}
				else
				{
					fprintf(Act()->LogFile(), "Internal Error:Virtual Reg Tabel Index Exceeds Max Entries\n");
					Act()->Exit(2);
				}
			}
			else if (Size == RegStatus::LOCKED_BYTE)
			{
				if (Size == m_pLockTabel[TableIndex])
				{
					m_pLockTabel[TableIndex] = FREE;
					rV = true;
					delete pVRegValue;
				}
				else
				{
					fprintf(Act()->LogFile(), "Internal Error:Virtual Register Lock Type Mismatch ID=%d\n", pVReg->GetTableIndex());
					Act()->Exit(2);
				}
			}
			else
			{
				fprintf(Act()->LogFile(), "Internal Error:Unlock Virtual Register:Well I Never!\n");
				Act()->Exit(2);
			}
		}
		else
		{
			fprintf(Act()->LogFile(), "Internal Error:Unlock Virtual Register:Not Even Close\n");
			Act()->Exit(2);
		}
	}
	else
	{
		// Value is not a virtual register
		switch (pVRegValue->GetValueType())
		{
		case CValue::ValueType::AREG:
		case CValue::ValueType::XREG:
		case CValue::ValueType::YREG:
			fprintf(Act()->LogFile(), "Internal Error:Using Hardware Register as Virtual Register\n");
			break;
		case CValue::ValueType::ASTRING:
		case CValue::ValueType::CSTRING:
		case CValue::ValueType::STRING:
			fprintf(Act()->LogFile(), "Inte3rnal Error:Using String as Virtual Register:%s\n", pVRegValue->GetString());
			break;
		case CValue::ValueType::CONSTANT:
			fprintf(Act()->LogFile(), "Inte3rnal Error:Using Constant as Virtual Register:%d\n", pVRegValue->GetConstVal());
			break;
		case CValue::ValueType::NONE:
			fprintf(Act()->LogFile(), "Inte3rnal Error:Using Undefined Value as Virtual Register\n");
			break;
		case CValue::ValueType::SYMBOL:
			fprintf(Act()->LogFile(), "Inte3rnal Error:Using Symbol Value as Virtual Register:%s\n",pVRegValue->GetSymbol()->GetName());
			break;
		default:
			break;
		}
		Act()->Exit(2);
	}
	return rV;
}

bool CVirtualReg::VREG::Create(RegStatus Size, int Address, int TableIndex)
{
	CSymbol* pSym = 0;
	CObjTypeChain* pOTC = 0;

	m_nBytes = Size;
	m_TableIndex = TableIndex;
	m_pName = new char[32];
	sprintf_s(m_pName, 16, "VR%d_%d", ++m_ID, TableIndex);
	pSym = new CSymbol;
	pSym->SetName(m_pName);
	pSym->SetAddress(Address);
	pSym->CreateTypeChain();
	pOTC = new CObjTypeChain;
	pOTC->SetSpec(CObjTypeChain::Spec::VIRTUAL_REG);
	pSym->GetTypeChain()->AddToHead(pOTC);
	if (Size == RegStatus::LOCKED_BYTE)
	{
		pOTC = new CObjTypeChain;
		pOTC->SetSpec(CObjTypeChain::Spec::BYTE);
	}
	else
	{
		pOTC = new CObjTypeChain;
		pOTC->SetSpec(CObjTypeChain::Spec::CARD);
	}
	pSym->GetTypeChain()->AddToHead(pOTC);
	Act()->GetParser()->GetLexer()->GetSymTab()->AddSymbol(pSym);
	SetSymbol(pSym);
	return true;
}
