#include "pch.h"

CAct65VarGlobal::CAct65VarGlobal()
{
}

CAct65VarGlobal::~CAct65VarGlobal()
{
}

bool CAct65VarGlobal::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65VarGlobal::Process()
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

int CAct65VarGlobal::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;
	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65VarGlobal::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65VarGlobal::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
