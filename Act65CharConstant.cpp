#include "pch.h"

CAct65CharConstant::CAct65CharConstant() :CAstNode(m_pNodeTyypeName, NodeType::CHAR_CONSTANT)
{
	m_Value = ' ';
}

CAct65CharConstant::~CAct65CharConstant()
{
}

bool CAct65CharConstant::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65CharConstant::Process()
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
	return Emit(m_pChildValue, m_pChildValue);
}

void CAct65CharConstant::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	if (pOut)
	{
		int l = 0;
		char* s = new char[256];
		
		l = Print(Indent, s, 256, pbNextFlag);
		sprintf_s(&s[l], 256, " \'%c'\'", GetValue());
		fprintf(pOut, "%s\n", s);
		delete[] s;
	}
}

int CAct65CharConstant::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

CValue* CAct65CharConstant::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
