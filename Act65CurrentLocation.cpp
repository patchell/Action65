#include "pch.h"

CAct65CurrentLocation::CAct65CurrentLocation() :CAstNode(m_pNodeTyypeName, NodeType::CURRENT_LOCATION)
{
}

CAct65CurrentLocation::~CAct65CurrentLocation()
{
}

bool CAct65CurrentLocation::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65CurrentLocation::Process()
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

int CAct65CurrentLocation::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65CurrentLocation::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	if (pOut)
	{
		char* s = new char[256];
		int l = 0;
		int size = 0;

		l = Print(Indent, s, 256, pbNextFlag);
		size = 256 - l;
		sprintf_s(&s[l], size, " =$%04X", GetValue() & 0x0ffff);
		fprintf(pOut, "%s\n", s);
		delete[] s;
	}
}

CValue* CAct65CurrentLocation::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
