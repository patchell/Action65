#include "pch.h"

CAct65ADD::CAct65ADD() :CAstNode(m_pNodeTyypeName, NodeType::ADDEXPR)
{
}

CAct65ADD::~CAct65ADD()
{
}

bool CAct65ADD::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65ADD::Process()
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

int CAct65ADD::Print(char* s, int Strlen, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr, pbNextFlag);
	if (GetValue())
	{
		if (GetValue()->GetSymbol())
		{
			if (GetValue()->GetSymbol()->GetName())
			{
				int size = Strlen - l;
				l += sprintf_s(&s[l], size, ": %s", GetValue()->GetSymbol()->GetName());
			}
		}
	}
	return l;
}
void CAct65ADD::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65ADD::Emit(CValue* pVc, CValue* pVn, CValue* pVr)
{
	if (!pVc)
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_VALUE_NULL);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Internal Error:ADD op Next Value is NULL  Line:%d Col:%d\n",
			GetLine(),
			GetColumn()
		);
		throw(ThrownException);
	}
	if (!pVn)
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::CODEGEN_UNKNOWN_BYTE_ORDER);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Internal Error:ADD op Next Value is NULL  Line:%d Col:%d\n", 
			GetLine(), 
			GetColumn()
		);
		throw(ThrownException);
	}
	return Act()->GetCodeGen()->EmitBinaryOp(Token::ADC, pVc, pVn, pVr, GetSection(),Token::CLC);
}
