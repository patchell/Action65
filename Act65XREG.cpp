#include "pch.h"

CAct65XREG::CAct65XREG() :CAstNode(m_pNodeTyypeName, NodeType::XREG)
{
	m_Reg = RegType::X;
}

CAct65XREG::~CAct65XREG()
{
}

bool CAct65XREG::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65XREG::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65XREG::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65XREG::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	if (pOut)
	{
		char* s = new char[256];
		int l = 0;
		int size = 0;

		l = Print(Indent, s, l, pbNextFlag);
		size = 256 - l;
		sprintf_s(&s[l], size, " - \'X\'");
		fprintf(pOut, "%s\n", s);
		delete[]s;
	}
}

CValue* CAct65XREG::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
