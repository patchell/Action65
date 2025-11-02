#include "pch.h"

CAct65DS::CAct65DS() :CAstNode(m_pNodeTyypeName, NodeType::DEFINE_STORAGE)
{
	m_pSection = 0;
	m_SizeInBytes = 0;
}

CAct65DS::~CAct65DS()
{
}

bool CAct65DS::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65DS::Process(SAuxEmitInfo* pAuxInfo)
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

int CAct65DS::Print(char* s, int Strlen, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr, pbNextFlag);
	return l;
}

void CAct65DS::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	if (pOut)
	{
		char* s = new char[256];
		int l = 0;
		int size = 0;

		l = Print(s, 256, Indent, NULL, pbNextFlag);
		size = 256 - l;
		sprintf_s(&s[l], size, " = $%04X Bytes", GetSize() & 0x0ffff);
		fprintf(pOut, "%s\n", s);
		delete[] s;
	}
}

CValue* CAct65DS::Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo)
{
    return nullptr;
}
