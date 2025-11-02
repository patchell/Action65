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

CValue* CAct65XOR::Process(SAuxEmitInfo* pAuxInfo)
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
		m_pChildValue = pChild->Process(pAuxInfo);
	}
	if (pNext)
	{
		m_pNextValue = pNext->Process(pAuxInfo);
	}
	if (pNext->GetNext())
	{
		m_pResultValue = pNext->GetNext()->Process(pAuxInfo);
	}
	return Emit(m_pChildValue, m_pNextValue, m_pResultValue);
}

int CAct65XOR::Print(char* s, int Strlen, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr, pbNextFlag);
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
