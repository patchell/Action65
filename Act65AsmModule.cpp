#include "pch.h"

CAct65AsmModule::CAct65AsmModule()
{
}

CAct65AsmModule::~CAct65AsmModule()
{
}

bool CAct65AsmModule::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65AsmModule::Process()
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

void CAct65AsmModule::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	if (pOut)
	{
		char* s = new char[256];
		int l = 0;
		l = Print(s, 256, Indent, NULL,pbNextFlag);
		fprintf(pOut, "%s\n", s);
		delete[]s;
	}
}

int CAct65AsmModule::Print(char* s, int l, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	l = CAstNode::Print(s, l, Indent, NULL, pbNextFlag);
	int size = 256 - l;
	l += sprintf_s(&s[l], size, " %s", m_pNodeTyypeName);
	size = 256 - l;
	if (GetSection())
	{
		l += sprintf_s(&s[l], size, " SECTION:%s", GetSection()->GetName());
	}
	else
	{
		l += sprintf_s(&s[l], size, "ERROR:No Section");
	}
	if (GetParent())
	{
		size = 256 - l;
		l += sprintf_s(&s[l], size, " (Parent:%s", GetParent()->GetNodeName());
	}
	return l;
}

CValue* CAct65AsmModule::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
