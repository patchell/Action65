#include "pch.h"

CAct65YREG::CAct65YREG() :CAstNode(m_pNodeTyypeName, NodeType::YREG)
{
	m_Reg = CReg::RegType::Y;
}

CAct65YREG::~CAct65YREG()
{
}

bool CAct65YREG::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65YREG::Process()
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

int CAct65YREG::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65YREG::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	if (pOut)
	{
		char* s = new char[256];
		int l = 0;
		int size = 0;

		l = Print(Indent, s, l, pbNextFlag);
		size = 256 - l;
		sprintf_s(&s[l], size, " - \'Y\'");
		fprintf(pOut, "%s\n", s);
		delete[]s;
	}
}

CValue* CAct65YREG::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
