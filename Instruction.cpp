#include "pch.h"

CInstruction::CInstruction()
{
	m_OpCode = 0;
	m_LowByte = 0;
	m_HighByte = 0;
	m_unused = 0;
	m_ByteCount = 0;
	m_OpCodeToken = Token::BRK;
	m_UnResolved = false;
}

CInstruction::~CInstruction()
{
	m_OpCodeToken = Token(0);
	m_OpCode = 0;
	m_LowByte = 0;
	m_HighByte = 0;
	m_unused = 0;
	m_ByteCount = 1;
}

bool CInstruction::Create()
{
	CStackItem::Create(CStackItem::ItemType::INSTRUCTION);
	return 0;
}

void CInstruction::AddToCurrentSection()
{
	CSection* pSec;
	unsigned short Address;

	pSec = Act()->GetParser()->GetCurrentSection();
	Address = (unsigned short)pSec->GetLocationCounter();
	pSec->AddData(1, GetOpCode());
	if (GetByteCount() == 1)
	{
//		fprintf(
//			Act()->LogFile(),
//			"Add Instruction %04x: %02x\n",
//			Address & 0x0ffff,
//			GetOpCode() & 0x0ff
//		);
	}
	else if (GetByteCount() == 2)
	{
		pSec->AddData(1, GetLowByte());
//		fprintf(
//			Act()->LogFile(), 
//			"Add Instruction %04hx: %02x,%02x\n", 
//			Address & 0x0ffff,
//			GetOpCode() & 0x0ff,
//			GetLowByte() & 0x0ff);
	}
	else 
	{
//		fprintf(
//			Act()->LogFile(), 
//			"Add Instruction %04hX: %02X, %04X\n", 
//			Address &0x0ffff, 
//			GetOpCode() & 0x0ff,
//			GetOperandW() & 0x0ffff
//		);
		pSec->AddData(2, GetOperandW());
	}
}

bool CInstruction::IsRelative()
{
	bool rV = false;

	switch (m_OpCodeToken)
	{
	case Token::BCC:
	case Token::BCS:
	case Token::BEQ:
	case Token::BMI:
	case Token::BNE:
	case Token::BPL:
	case Token::BVC:
	case Token::BVS:
		rV = true;
		break;
	}
    return rV;
}
