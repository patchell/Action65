#include "pch.h"

CAct65NUMBER::CAct65NUMBER() :CAstNode(m_pNodeTyypeName, NodeType::NUMBER)
{
}

CAct65NUMBER::~CAct65NUMBER()
{
}

bool CAct65NUMBER::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65NUMBER::Process(SAuxEmitInfo* pAuxInfo)
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
	return Emit(m_pChildValue, m_pChildValue, pAuxInfo);
}

void CAct65NUMBER::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	int l = 0;
	int size = 0;

	if (pOut)
	{
		char* s = new char[256];
		l = Print(s, 256, Indent, NULL, pbNextFlag);
		size = 256 - l;
		sprintf_s(&s[l], size, "%04X", GetValue()->GetConstVal());
		fprintf(pOut, "%s\n", s);
		delete[] s;
	}
}

int CAct65NUMBER::Print(char* s, int Strlen, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr, pbNextFlag);
	int size = Strlen - l;
	l += sprintf_s(&s[l], size, "$%04X", GetValue()->GetConstVal());
	return l;
}

CValue* CAct65NUMBER::Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo)
{
    return nullptr;
}
