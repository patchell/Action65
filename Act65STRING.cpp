#include "pch.h"

CAct65STRING::CAct65STRING() :CAstNode(m_pNodeTyypeName, NodeType::STRING)
{
}

CAct65STRING::~CAct65STRING()
{
}

void CAct65STRING::SetString(const char* s)
{
	CreateValue(s);
	GetValue()->SetValueType(CValue::ValueType::CSTRING);
}

bool CAct65STRING::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	{
		bool rV = true;
		rV = CAstNode::Create(pChild, pNext, pSym, pSec);
		return rV;
	}
}

CValue* CAct65STRING::Process(SAuxEmitInfo* pAuxInfo)
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

int CAct65STRING::Print(char* s, int Strlen, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	int l = 0;
	int size = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr, pbNextFlag);
	size = Strlen - l;
	l += sprintf_s(&s[l], size, ":\'%s\'", GetValue()->GetString());
	return l;
}

void CAct65STRING::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	if (pOut)
	{
		char* s = new char[512];
		int l = 0;
		int size = 0;

		l = Print(s, 512, Indent, 0, pbNextFlag);
		size = 256 - l;
		sprintf_s(&s[l], size, " = \"%s\"", GetString());
		fprintf(pOut, "%s\n", s);
		delete[] s;
	}
}

CValue* CAct65STRING::Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo)
{
	CValue* pValue;

	pValue = GetValue();
    return pValue;
}

char* CAct65STRING::GetString()
{
	return GetValue()->GetString();
}

int CAct65STRING::GetStrLen()
{
	return GetValue()->GetConstVal();
}
