#include "pch.h"

CAct65Assignment::CAct65Assignment() :CAstNode(m_pNodeTyypeName, NodeType::BASE)
{
}

CAct65Assignment::~CAct65Assignment()
{
}

bool CAct65Assignment::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym);
	return rV;
}

CValue* CAct65Assignment::Process()
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValue = 0;

	pChild = GetChild();
	if (pChild)
	{
		pNext = pChild->GetNext();
	}
	if (pChild)
	{
		m_pChildValue = pChild->Process();
	}
	if (pNext)
	{
		m_pNextValue = pNext->Process();
	}
	return Emit(m_pChildValue, m_pChildValue);
}

int CAct65Assignment::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65Assignment::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65Assignment::Emit(CValue* pVc, CValue* pVn)
{
	//----------------------------------------------
	// Value to store (assign to) is in pVc
	// Value to store to is pVn
	//----------------------------------------------
	CInstruction* pOpCode = 0;
	Token OpToken = Token::NONE;
	AdrModeType AddressingMode = AdrModeType::NA;
	int ValueSizeC;
	int ValueSizeN;
	CValue* pTempValue = 0;
	int V = 0;

	ValueSizeC = pVc->SizeOf();
	ValueSizeN = pVn->SizeOf();
	
	for (int i = 0; i < ValueSizeN; ++i)
	{
		switch (pVc->GetValueType())	// from here
		{
		case CValue::ValueType::CONSTANT:
			if (i == 0)
			{
				pTempValue = new CValue;
				V = pVc->GetConstVal();
				pTempValue->Create(V & 0xff);
				pOpCode = new CInstruction;
				pOpCode->GenInstruction(
					Token::LDA,
					AdrModeType::IMMEDIATE_ADR,
					pTempValue,
					0,
					0
				);
				GetSection()->AddInstruction(pOpCode);
				pOpCode = 0;
			}
			else
			{
				// Second Byte if any
				pTempValue = new CValue;
				pTempValue->Create((V & 0xff00) >> 8);
				pOpCode->GenInstruction(
					Token::LDA,
					AdrModeType::IMMEDIATE_ADR,
					pTempValue,
					0,
					0
				);
				GetSection()->AddInstruction(pOpCode);
				pOpCode = 0;
				delete pTempValue;
			}
			break;
		case CValue::ValueType::REG:
			switch (pVc->GetRegister()->GetType())
			{
			case CReg::RegType::A:
				OpToken = Token::STA;
				break;
			case CReg::RegType::X:
				OpToken = Token::STX;
				break;
			case CReg::RegType::Y:
				OpToken = Token::STY;
				break;
			}
			break;
		case CValue::ValueType::SYMBOL:
			break;
		case CValue::ValueType::VIRTUAL_REGISTER:
			break;
		default:
			break;
		}

		switch (pVn->GetValueType())	//to here
		{
		case CValue::ValueType::SYMBOL:
			if (pVn->IsPageZero())
				AddressingMode = AdrModeType::ZERO_PAGE_ADR;
			else
				AddressingMode = AdrModeType::ABSOLUTE_ADR;
			pOpCode = new CInstruction;
			pOpCode->GenInstruction(
				OpToken,
				AddressingMode,
				pVn,
				0,
				0
			);
			GetSection()->AddInstruction(pOpCode);
			break;
		case CValue::ValueType::VIRTUAL_REGISTER:
			break;
		default:
			break;
		}
	}
    return nullptr;
}
