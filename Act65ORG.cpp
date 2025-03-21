#include "pch.h"

CAct65ORG::CAct65ORG() :CAstNode(m_pNodeTyypeName, NodeType::ORG)
{
	m_Address = 0;}

CAct65ORG::~CAct65ORG()
{
}

bool CAct65ORG::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65ORG::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65ORG::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65ORG::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	if (pOut)
	{
		char* s = new char[256];
		int l = 0;
		int size;

		l = Print(Indent, s, 256, pbNextFlag);
		size = 256 - l;
		sprintf_s(&s[l], size, "- Address:$%04X", GetAddress() & 0x0ffff);
		fprintf(pOut, "%s\n", s);
		delete[] s;
	}
}

CValue* CAct65ORG::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
