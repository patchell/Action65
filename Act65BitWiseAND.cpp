#include "pch.h"

CAct65BitWiseAND::CAct65BitWiseAND() :CAstNode(m_pNodeTyypeName, NodeType::BITWISE_AND)
{
}

CAct65BitWiseAND::~CAct65BitWiseAND()
{
}

bool CAct65BitWiseAND::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65BitWiseAND::Process(SAuxEmitInfo* pAuxInfo)
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
	if (pNext->GetNext())
	{
		m_pResultValue = pNext->GetNext()->Process(pAuxInfo);
	}
	return Emit(m_pChildValue, m_pNextValue, m_pResultValue);
}

int CAct65BitWiseAND::Print(char* s, int Strlen, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr, pbNextFlag);
	return l;
}

void CAct65BitWiseAND::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65BitWiseAND::Emit(CValue* pVc, CValue* pVn, CValue* pVr)
{
	if (!pVc)
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_VALUE_NULL);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Internal Error:ADD op Child Value is NULL Line:%d Col:%d\n",
			GetLine(), 
			GetColumn()
		);
		throw(ThrownException);
	}
	if (!pVn)
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_VALUE_NULL);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Internal Error:ADD op Child Value is NULL Line:%d Col:%d\n",
			GetLine(),
			GetColumn()
		);
		throw(ThrownException);
	}
	return GetCodeGen()->EmitBinaryOp(
		Token::AND, 
		pVc, 
		pVn, 
		pVr, 
		GetSection()
	);
}
