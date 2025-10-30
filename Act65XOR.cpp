#include "pch.h"

CAct65XOR::CAct65XOR() :CAstNode(m_pNodeTyypeName, NodeType::XOR)
{
}

CAct65XOR::~CAct65XOR()
{
}

bool CAct65XOR::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65XOR::Process()
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValue = 0;

	fprintf(Act()->LogFile(), "Process %s Node:%d\n", GetNodeName(), GetID());
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
	if (pNext->GetNext())
	{
		m_pResultValue = pNext->GetNext()->Process();
	}
	return Emit(m_pChildValue, m_pNextValue, m_pResultValue);
}

int CAct65XOR::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65XOR::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65XOR::Emit(CValue* pVc, CValue* pVn, CValue* pVr)
{
	return GetCodeGen()->EmitBinaryOp(
		Token::EOR, 
		pVc, 
		pVn, 
		pVr, 
		GetSection()
	);
}
