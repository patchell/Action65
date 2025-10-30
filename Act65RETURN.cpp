#include "pch.h"

CAct65RETURN::CAct65RETURN() :CAstNode(m_pNodeTyypeName, NodeType::RETURN)
{
}

CAct65RETURN::~CAct65RETURN()
{
}

bool CAct65RETURN::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65RETURN::Process()
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValue = 0;

//	fprintf(Act()->LogFile(), "Process %s Node:%d\n", GetNodeName(), GetID());
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

int CAct65RETURN::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65RETURN::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65RETURN::Emit(CValue* pVc, CValue* pVn)
{
	CInstruction* pOpCode = 0;
	ProcType PT = GetProcType();

	switch(PT)
	{
	case ProcType::IRQPROC:
		//------------------------------
		// Generate code to pop stack
		//------------------------------
		pOpCode = new CInstruction();
		pOpCode->GenInstruction(
			Token::PLA, 
			AdrModeType::IMPLIED, 
			0, 
			0, 
			0
		);
		GetSection()->AddInstruction(pOpCode);	

		pOpCode = new CInstruction();
		pOpCode->GenInstruction(
			Token::TAY,
			AdrModeType::IMPLIED,
			0,
			0,
			0
		);
		GetSection()->AddInstruction(pOpCode);

		pOpCode = new CInstruction();
		pOpCode->GenInstruction(
			Token::PLA,
			AdrModeType::IMPLIED,
			0,
			0,
			0
		);
		GetSection()->AddInstruction(pOpCode);

		pOpCode = new CInstruction();
		pOpCode->GenInstruction(
			Token::TAX,
			AdrModeType::IMPLIED,
			0,
			0,
			0
		);
		GetSection()->AddInstruction(pOpCode);

		pOpCode = new CInstruction();
		pOpCode->GenInstruction(
			Token::PLA,
			AdrModeType::IMPLIED,
			0,
			0,
			0
		);
		GetSection()->AddInstruction(pOpCode);

		pOpCode = new CInstruction();
		pOpCode->GenInstruction(
			Token::RTI,
			AdrModeType::IMPLIED,
			0,
			0,
			0
		);
		GetSection()->AddInstruction(pOpCode);
		break;
	case ProcType::FUNC:
		break;
	case ProcType::NONE:
		break;
	case ProcType::PROC:
		pOpCode = new CInstruction();
		pOpCode->GenInstruction(
			Token::RTS,
			AdrModeType::IMPLIED,
			0,
			0,
			0
		);
		GetSection()->AddInstruction(pOpCode);
		break;
	}
    return nullptr;
}
