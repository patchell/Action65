#include "pch.h"

CAct65ADD::CAct65ADD() :CAstNode(m_pNodeTyypeName, NodeType::ADDEXPR)
{
}

CAct65ADD::~CAct65ADD()
{
}

bool CAct65ADD::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65ADD::Process()
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValue = 0;

//	fprintf(Act()->LogFile(), "Process %s Node:%d\n",GetNodeName(), GetID());
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
	if (m_pResultValue)
		pValue = AltEmit(m_pChildValue, m_pNextValue, m_pResultValue);
	else
		pValue = Emit(m_pChildValue, m_pNextValue);
	return pValue;
}

int CAct65ADD::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
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
CValue* CAct65ADD::Emit(CValue* pVc, CValue* pVn)
{
	if (!pVc)
	{
		fprintf(Act()->LogFile(), "Internal Error:ADD op Child Value is NULL Line:%d Col:%d\n", GetLine(), GetColumn());
		Act()->Exit(2);
	}
	if (!pVn)
	{
		fprintf(Act()->LogFile(), "Internal Error:ADD op Next Value is NULL  Line:%d Col:%d\n", GetLine(), GetColumn());
		Act()->Exit(2);
	}
	return Act()->GetParser()->GetCodeGenUtils()->EmitBinaryOp(Token::ADC, pVc, pVn, 0, GetSection());
}

CValue* CAct65ADD::AltEmit(CValue* pVc, CValue* pVn, CValue* pVr)
{
	if (!pVc)
	{
		fprintf(Act()->LogFile(), "Internal Error:ADD op Child Value is NULL Line:%d Col:%d\n", GetLine(), GetColumn());
		Act()->Exit(2);
	}
	if (!pVn)
	{
		fprintf(Act()->LogFile(), "Internal Error:ADD op Next Value is NULL  Line:%d Col:%d\n", GetLine(), GetColumn());
		Act()->Exit(2);
	}
	return Act()->GetParser()->GetCodeGenUtils()->EmitBinaryOp(Token::ADC, pVc, pVn, pVr, GetSection());
}
