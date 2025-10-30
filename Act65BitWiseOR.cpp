#include "pch.h"

CAct65BitWiseOR::CAct65BitWiseOR() :CAstNode(m_pNodeTyypeName, NodeType::BITWISE_OR)
{
}

CAct65BitWiseOR::~CAct65BitWiseOR()
{
}

bool CAct65BitWiseOR::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65BitWiseOR::Process()
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
	if (pNext->GetNext())
	{
		m_pResultValue = pNext->GetNext()->Process();
	}
	return Emit(m_pChildValue, m_pNextValue, m_pResultValue);
}

int CAct65BitWiseOR::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65BitWiseOR::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65BitWiseOR::Emit(CValue* pVc, CValue* pVn, CValue* pVr)
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
		Token::ORA,
		pVc,
		pVn,
		pVr,
		GetSection()
	);
}
