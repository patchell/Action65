#include "pch.h"

CAct65OD::CAct65OD() :CAstNode(m_pNodeTyypeName, NodeType::OD)
{
}

CAct65OD::~CAct65OD()
{
}

bool CAct65OD::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65OD::Process(SAuxEmitInfo* pAuxInfo)
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
		m_pChildValue = pChild->Process(pAuxInfo);
	}
	if (pNext)
	{
		m_pNextValue = pNext->Process(pAuxInfo);
	}
	return Emit(m_pChildValue, m_pChildValue, pAuxInfo);
}

int CAct65OD::Print(char* s, int Strlen, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr, pbNextFlag);
	return l;
}

void CAct65OD::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65OD::Emit(CValue* pVc, CValue* pVn, SAuxEmitInfo* pAuxInfo)
{
	CStackDOODItem* pDOODItem = 0;
	CInstruction* pOpCode = 0;

	pDOODItem = (CStackDOODItem*)Act()->GetDOODStack()->Pop();
	if (pDOODItem)
	{
		pOpCode = new CInstruction;
		pOpCode->GenInstruction(
			Token::JMP,
			AdrModeType::ABSOLUTE_ADR,
			pDOODItem->GetLabelValue(),
			0
		);
		GetSection()->AddInstruction(pOpCode);
	}
	return nullptr;
}
