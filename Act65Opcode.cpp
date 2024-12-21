#include "pch.h"

CAct65Opcode::CAct65Opcode()
{
	m_OpcodeToken = Token(-1);
	m_pOpCodeName = 0;
	m_LineNumber = 0;
	m_ColumnNumber = 0;
	m_pSym = 0;
	m_pLabel = 0;
	m_AdressMode = AdrModeType::NA;
}

CAct65Opcode::~CAct65Opcode()
{
}

bool CAct65Opcode::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65Opcode::Process()
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValueChild = 0, * pValueNext = 0
		;
	pChild = GetChild();
	if (pChild)
	{
		pNext = pChild->GetNext();
	}
	if (pChild)
	{
		pValueChild = pChild->Process();
	}
	if (pNext)
	{
		pValueNext = pNext->Process();
	}
	//-----------------------
	// Code Generation
	//-----------------------
	return pValueChild;
}

void CAct65Opcode::Print(FILE* pOut, int Indent, char* s)
{
	CAstNode::Print(pOut, Indent, s);
}
