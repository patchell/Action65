#include "pch.h"

CSection::CSection():CBin(BinType::SECTION)
{
	m_StartAddress = 0;
	m_Size = 0;
	m_LocationCounter = 0;
	m_pSectionData = 0;
	m_Type = SectionType::TYPE_ABSOLUTE;
	m_AccessMode = Mode::MODE_READ_ONLY;
	m_ZeroPageAddressSize = AddressSize::ADDRESSSIZE_WORD;
	m_pNextSection = 0;
	m_pPrevSection = 0;
}

CSection::~CSection()
{
	if (m_pSectionData) delete[] m_pSectionData;
}

bool CSection::Create()
{
	bool rV = true;
	CBin::Create();
	return rV;
}

bool CSection::Compare(const char* name, BinType Type, int aux)
{
	bool rV = false;

	if (strcmp(name, GetName()) == 0)
	{
		if (Type == BinType::ANY || Type == BinType::SECTION)
		{
			if(aux == 0 || aux == GetScope())
				rV = true;
		}
	}
	return rV;
}

void CSection::SetStartAddress(int Start)
{
	if ((Start + m_Size) <= 0x010000u)
	{
		m_LocationCounter = Start;
		m_StartAddress = Start;
		if (m_Size > 0)
		{
			int i;

			m_pSectionData = new char[m_Size];
			for (i = 0; i < m_Size; ++i)
				m_pSectionData[i] = 0;
		}
	}
	else
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::SECTION_ADDRES_RANGE_EXCEEDED);
		ThrownException.SetSymbol(this);
		ThrownException.SetGotToken(Token::NUMBER);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CSection::SetStartAddress: Section:%s  Start Address:$%04x  Size:$%04x Over By:$%04x\n",
			GetName(),
			Start,
			m_Size,
			Start + m_Size - 0x10000
		);
		throw(ThrownException);
	}
}

void CSection::SetSectionSize(int Size)
{
	if (m_StartAddress + Size <= 0x10000)
	{
		m_Size = Size;
		if (m_Size > 0)
		{
			int i;

			m_pSectionData = new char[m_Size];
			for (i = 0; i < m_Size; ++i)
				m_pSectionData[i] = 0;
		}
	}
	else
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::SECTION_ADDRES_RANGE_EXCEEDED);
		ThrownException.SetSymbol(this);
		ThrownException.SetGotToken(Token::NUMBER);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CSection::SetSectionSize: Section:%s  Start Address:$%04x  Size:$%04x Over By:$%04x\n",
			GetName(),
			m_StartAddress,
			Size,
			m_StartAddress + Size - 0x10000
		);
		throw(ThrownException);
	}
}

void CSection::SetLocationCounter(int NewAddress)
{
	if ((NewAddress < (m_StartAddress + m_Size)) &&
		(NewAddress >= m_StartAddress)
		)
	{
		m_LocationCounter = NewAddress;
	}
	else
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::SECTION_ADDRES_RANGE_EXCEEDED);
		ThrownException.SetSymbol(this);
		ThrownException.SetGotToken(Token::NUMBER);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CSection::SetLocationCounter: Section:%s  Start Address:$%04x  Size:$%04x Over By:$%04x\n",
			GetName(),
			0,
			m_Size,
			NewAddress
		);
		throw(ThrownException);
	}
}

int CSection::Relocate(int NewBaseAddress)
{
	CChainValueItem* pValChainItem = 0;

	pValChainItem = (CChainValueItem*)m_Values.GetHead();
	while (pValChainItem)
	{
		pValChainItem->GetValue()->GetSymbol()->SetAddress(NewBaseAddress + pValChainItem->GetLocation());
		pValChainItem->GetValue()->GetSymbol()->BackFillUnresolved();
		pValChainItem = (CChainValueItem*)pValChainItem->GetNext();
	}
    return 0;
}

//int CSection::AddInstruction(CInstruction* pInstruction)
//{
//	int OperandAddress = 0;
//	CWhereSymbolIsUsed* pWSIU = 0;
//	CSymbol* pSym = 0;
//
//	if (pInstruction->GetLabel())
//	{
//		AddLabelValue(pInstruction->GetLabel());
//		pInstruction->GetLabel()->GetSymbol()->SetAddress(m_LocationCounter);
//	}
//	switch (pInstruction->GetAdrModeType())
//	{
//	case AdrModeType::IMPLIED:
//		AddData(1, pInstruction->GetOpCode());
//		break;
//	case AdrModeType::IMMEDIATE_ADR:
//	case AdrModeType::ZERO_PAGE_ADR:
//	case AdrModeType::ZERO_PAGE_X_ADR:
//	case AdrModeType::ZERO_PAGE_Y_ADR:
//	case AdrModeType::INDEXED_INDIRECT_X_ADR:
//	case AdrModeType::INDIRECT_INDEXED_Y_ADR:
//		OperandAddress = AddData(1, pInstruction->GetOpCode());
//		if (pInstruction->GetOperand()->GetValueType() == CValue::ValueType::SYMBOL)
//		{
//			pWSIU = new CWhereSymbolIsUsed;
//			pWSIU->Create();
//			pWSIU->SetSection(this);
//			pWSIU->SetAddress(OperandAddress);
//			pWSIU->SetUnResType(CWhereSymbolIsUsed::UnResolvedType::ABSOLUTE_REFERENCE);
//			pInstruction->GetOperand()->GetSymbol()->Add(pWSIU);	//add where used entry
//			pSym = pInstruction->GetOperand()->GetSymbol();
//			if (pSym->IsResolved())
//				pWSIU->SetResolveProcessed();
//		}
//		AddData(1, pInstruction->GetOperand()->GetTotalValue());
//		break;
//	case AdrModeType::ABSOLUTE_ADR:
//	case AdrModeType::ABSOLUTE_X_ADR:
//	case AdrModeType::ABSOLUTE_Y_ADR:
//	case AdrModeType::INDIRECT_ADR:
//		OperandAddress = AddData(1, pInstruction->GetOpCode());
//		if (pInstruction->GetOperand()->GetValueType() == CValue::ValueType::SYMBOL)
//		{
//			pWSIU = new CWhereSymbolIsUsed;
//			pWSIU->Create();
//			pWSIU->SetSection(this);
//			pWSIU->SetAddress(OperandAddress);
//			pWSIU->SetUnResType(CWhereSymbolIsUsed::UnResolvedType::ABSOLUTE_REFERENCE);
//			pInstruction->GetOperand()->GetSymbol()->Add(pWSIU);	//add where used entry
//			pSym = pInstruction->GetOperand()->GetSymbol();
//			if (pSym->IsResolved())
//				pWSIU->SetResolveProcessed();
//		}
//		AddData(2, pInstruction->GetOperand()->GetTotalValue());
//		break;
//	case AdrModeType::RELATIVE:
//		//fprintf(Act()->LogFile(), "RELATIVE: Sym:%s  %04X  %d REL:%02X\n", 
//		//	pInstruction->GetOperand()->GetName(),
//		//	pInstruction->GetOperand()->GetSymbol()->GetAddress(),
//		//	pInstruction->GetOperand()->GetSymbol()->GetAddress(),
//		//	(pInstruction->GetOperand()->GetTotalValue() - (m_LocationCounter + 2)) & 0x0ff
//		//);
//		OperandAddress = AddData(1, pInstruction->GetOpCode());
//		if (pInstruction->GetOperand()->GetValueType() == CValue::ValueType::SYMBOL)
//		{
//			pWSIU = new CWhereSymbolIsUsed;
//			pWSIU->Create();
//			pWSIU->SetSection(this);
//			pWSIU->SetAddress(OperandAddress);
//			pWSIU->SetUnResType(CWhereSymbolIsUsed::UnResolvedType::RELATIVE_REFERENCE);
//			pInstruction->GetOperand()->GetSymbol()->Add(pWSIU);	//add where used entry
//			pSym = (CSymbol*)pInstruction->GetOperand()->GetSymbol();
//			if (pSym->IsResolved())
//				pWSIU->SetResolveProcessed();
//		}
//		//---------------------------------------------
//		// m_LocationCounter is one away from the
//		// next instruction, so by adding 1 to 
//		// m_LocationCounter that gets us up to the
//		// next instruction
//		//---------------------------------------------
//		AddData(1, pInstruction->GetOperand()->GetTotalValue() - (m_LocationCounter + 1));
//		break;
//	}
//    return m_LocationCounter;
//}

int CSection::AddData(int ObjectSize, int Value, CValue* pLabelValue)
{
	char* b;
	short* s;
	long* l;

	if (pLabelValue)
		AddLabelValue(pLabelValue);
	Info();
	switch (ObjectSize)
	{
	case 1:
		b = (char*)&m_pSectionData[m_LocationCounter - m_StartAddress];
		m_LocationCounter += 1;
		*b = char(Value);
		break;
	case 2:
		s = (short*)&m_pSectionData[m_LocationCounter - m_StartAddress];
		m_LocationCounter += 2;
		*s = short(Value);
		break;
	case 4:
		l = (long*)&m_pSectionData[m_LocationCounter - m_StartAddress];
		m_LocationCounter += 4;
		*l = long(Value);
		break;
	}
    return m_LocationCounter;
}

int CSection::AddData(int ObjSize, const char* pData, CValue* pLabelValue)
{
	int i = 0;

	if (pLabelValue)
		AddLabelValue(pLabelValue);
	for (i = 0; i < ObjSize; ++i)
	{
		AddData(1, pData[i]);
	}
	return m_LocationCounter;
}

void CSection::AddDataAt(
	unsigned Address, 
	unsigned ObjectSize, 
	int Value
)
{
	char* b;
	short* s;
	long* l;

	Info();
	switch (ObjectSize)
	{
	case 1:
		b = (char*)&m_pSectionData[Address - m_StartAddress];
		*b = char(Value) + *b;
		break;
	case 2:
		s = (short*)&m_pSectionData[Address - m_StartAddress];
		*s = short(Value) + *s;
		break;
	case 4:
		l = (long*)&m_pSectionData[Address - m_StartAddress];
		*l = long(Value) + *l;
		break;
	}
}

int CSection::AddInstruction(CInstruction* pInstruction)
{
	int NextAddress = 0;
	CChainInstruction* pInsChainItem = new CChainInstruction;

	pInsChainItem->Create(pInstruction);
	pInstruction->SetAddress(m_LocationCounter);
	if(pInstruction->GetAdrMode() == AdrModeType::NA)
	{
		printf("Oops\n");
	}
	switch(pInstruction->GetNumBytes())
	{
	case 1:
		IncrementLocationCounterBy(1);
		break;
	case 2:
		IncrementLocationCounterBy(2);
		break;
	case 3:
		IncrementLocationCounterBy(3);
		break;
	}
	GetInstructionsChain()->AddToTail(pInsChainItem);
	return NextAddress;
}

int CSection::AllocateDataBlock(int size, CValue* pLabelValue)
{
	int Temp = m_LocationCounter;

	if (pLabelValue)
	{
		AddLabelValue(pLabelValue);
		if (pLabelValue->GetSymbol())
		{
			pLabelValue->GetSymbol()->SetAddress(Temp);
		}
	}
	m_LocationCounter += size;
	return Temp;
}

void CSection::AddLabelValue(CValue* pLabelValue)
{
	CChainValueItem* pValueItem = new CChainValueItem;
	pValueItem->SetValue(pLabelValue);
	pValueItem->SetLocation(m_LocationCounter);
	m_Values.AddToHead(pValueItem);
}

bool CSection::EmitToSection(CAstNode* pNode, int ObjectSize, CSymbol* pLabel)
{
	bool rV = true;
	CValue* pValue;

	if (pLabel)
		pLabel->SetAddress(GetLocationCounter());
	while (pNode)
	{
		pValue = pNode->GetValue();
		switch (pValue->GetValueType())
		{
		case CValue::ValueType::SYMBOL:
			break;
		case CValue::ValueType::CONSTANT:
			AddData(ObjectSize, pValue->GetConstVal());
			break;
		case CValue::ValueType::STRING:
			break;
		}
		pNode = pNode->GetNext();
	}
    return rV;
}

void CSection::Print(FILE* pOut, const char* s)
{
	fprintf(pOut, "%sSection:%s:Start:$%04X Size:$%04X MODE:%s:%s Address Size:%s\n",
		s?s:"",
		GetName(),
		m_StartAddress,
		m_Size,
		int(m_AccessMode) ? "R/W" : "RO",
		int(m_Type) ? "RELOCATALE" : "ABSOLUTE",
		int(m_ZeroPageAddressSize)?"BYTE":"WORD"
	);
}

void CSection::Dump(FILE* pOut, const char* s)
{
	if (s)
		fprintf(pOut, "--------- %s ---------\n", s);
	else
		fprintf(pOut, "--------- %s ---------\n", GetName());
	CActionApp::Dump(
		pOut,
		m_pSectionData,
		GetStartAddress(),
		GetSectionSize()
	);
}

void CSection::Info()
{
	//fprintf(
	//	Act()->LogFile(),
	//	"%s Loc Cntr:%04x\n",
	//	GetName(),
	//	GetLocationCounter()
	//);
}

void CSection::EmitListing()
{
	CChainInstruction* pInsChainItem = (CChainInstruction*)m_Instructions.GetHead();
	while (pInsChainItem)
	{
		pInsChainItem->GetInstruction()->EmitListing();
		pInsChainItem = (CChainInstruction*)pInsChainItem->GetNext();
	}
}

void CSection::EmitBinary()
{
	CInstruction* pIns = 0;
	CChainItem* pItem = m_Instructions.GetHead();

	while (pItem)
	{
		switch (pItem->GetItemType())
		{
		case CChainItem::ChainItemType::INSTRUCTION:
			pIns = ((CChainInstruction*)pItem)->GetInstruction();
			if (pIns)
			{
//				Add()
			}
			break;
		case CChainItem::ChainItemType::VALUE:
			break;
		case CChainItem::ChainItemType::BIN:
			break;
		case CChainItem::ChainItemType::PARAMETER:
			break;
		case CChainItem::ChainItemType::TYPE:
			break;
		case CChainItem::ChainItemType::SYMBOL_USED:
			break;
		default:
			break;
		}
		pItem = pItem->GetNext();
	}
}
