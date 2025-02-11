#include "pch.h"

CAct65PROCasm::CAct65PROCasm()
{
}

CAct65PROCasm::~CAct65PROCasm()
{
}

bool CAct65PROCasm::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::OPCODE);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65PROCasm::Process()
{
	return nullptr;
}

int CAct65PROCasm::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65PROCasm::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	if (pOut)
	{
		char* pS = new char[256];
		int l = 0;
		int size = 0;

		l = Print(Indent, pS, 256, pbNextFlag);
		fprintf(pOut, "%s\n", pS);
		delete[]pS;
	}
}
