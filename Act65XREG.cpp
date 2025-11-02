#include "pch.h"

CAct65XREG::CAct65XREG() :CAstNode(m_pNodeTyypeName, NodeType::XREG)
{
	m_Reg = CReg::RegType::X;
}

CAct65XREG::~CAct65XREG()
{
}

bool CAct65XREG::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	{
		bool rV = true;
		rV = CAstNode::Create(pChild, pNext, pSym, pSec);
		return rV;
	}
}

CValue* CAct65XREG::Process(SAuxEmitInfo* pAuxInfo)
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

int CAct65XREG::Print(char* s, int Strlen, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr, pbNextFlag);
	return l;
}

void CAct65XREG::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	if (pOut)
	{
		char* s = new char[256];
		int l = 0;
		int size = 0;

		l = Print(s, 256, Indent, 0, pbNextFlag);
		size = 256 - l;
		sprintf_s(&s[l], size, " - \'X\'");
		fprintf(pOut, "%s\n", s);
		delete[]s;
	}
}

CValue* CAct65XREG::Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo)
{
    return nullptr;
}
