#include "pch.h"

CSection::CSection():CBin(BinType::SECTION)
{
	m_StartAddress = 0;
	m_Size = 0;
	m_LocationCounter = 0;
	m_pSectionData = 0;
	m_AccessMode = Mode::MODE_READ_ONLY;
	m_ZeroPageAddressSize = AddressSize::ADDRESSSIZE_WORD;
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
			"Section:%s  Start Address:$%04x  Size:$%04x Over By:$%04x\n",
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
			"Section:%s  Start Address:$%04x  Size:$%04x Over By:$%04x\n",
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
			"Section:%s  Start Address:$%04x  Size:$%04x Over By:$%04x\n",
			GetName(),
			0,
			m_Size,
			NewAddress
		);
		throw(ThrownException);
	}
}

int CSection::AddInstruction(CAct65Opcode* pINS)
{
	switch (pINS->GetAdrModeType())
	{
	case AdrModeType::IMPLIED:
		AddData(1, pINS->GetOpCode());
		break;
	case AdrModeType::IMMEDIATE_ADR:
	case AdrModeType::ZERO_PAGE_ADR:
	case AdrModeType::ZERO_PAGE_X_ADR:
	case AdrModeType::ZERO_PAGE_Y_ADR:
	case AdrModeType::INDEXED_INDIRECT_X_ADR:
	case AdrModeType::INDIRECT_INDEXED_Y_ADR:
		AddData(1, pINS->GetOpCode());
		AddData(1, pINS->GetOperand()->GetTotalValue());
		break;
	case AdrModeType::ABSOLUTE_ADR:
	case AdrModeType::ABSOLUTE_X_ADR:
	case AdrModeType::ABSOLUTE_Y_ADR:
	case AdrModeType::INDIRECT_ADR:
		AddData(1, pINS->GetOpCode());
		AddData(2, pINS->GetOperand()->GetTotalValue());
		break;
	case AdrModeType::RELATIVE:
		AddData(1, pINS->GetOpCode());
		AddData(1, pINS->GetOperand()->GetTotalValue() - 1);
		break;
	}
    return m_LocationCounter;
}

int CSection::AddData(int ObjectSize, int Value)
{
	char* b;
	short* s;
	long* l;

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

int CSection::AddData(int ObjSize, const char* pData)
{
	int i = 0;

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

int CSection::AllocateDataBlock(int size)
{
	m_LocationCounter += size;
	return m_LocationCounter;
}

void CSection::Print(FILE* pOut, const char* s)
{
	fprintf(pOut, "Section:%s:Size:$%04X MODE:%s\n",
		GetName(),
		m_Size,
		int(m_AccessMode) ? "Read/Write" : "Read Only"
	);
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

bool CSection::Compare(const char* name, int scope)
{
	bool rV = false;

	if (strcmp(GetName(), name) == 0)
	{
		rV = true;
		//if (scope > 0)
		//{
		//	if (scope == GetScope())
		//		rV = true;
		//}
		//else
		//	rV = true;
	}
	return rV;
}

