#include "pch.h"

CAstNode::CAstNode()
{
	m_NodeID = ++m_NodeCount;
	m_NodeType = 0;
	m_pStart = 0;
	m_pNext = 0;
	m_pPrev = 0;
	m_pChild = 0;
	m_pParent = 0;
	m_pNodeName = 0;
	m_pValue = 0;
	m_pHead = 0;
	m_pTail = 0;
}

CAstNode::CAstNode(int NodeType)
{
	m_NodeID = ++m_NodeCount;
	m_NodeType = NodeType;
	m_pStart = 0;
	m_pNext = 0;
	m_pPrev = 0;
	m_pChild = 0;
	m_pParent = 0;
	m_pNodeName = 0;
	m_pValue = 0;
	m_pHead = 0;
	m_pTail = 0;
}

CAstNode::~CAstNode()
{
	if (m_pValue)
		delete m_pValue;
}

bool CAstNode::Create(
	CAstNode* pChild,
	CAstNode* pNext
)
{
	FILE* pOut = Act()->GetParser()->LogFile();

	SetChild(pChild);
	if(pNext)
		AddThatToThisNext(pNext);
	SetStart(Act()->GetParser()->GetAstTree()->GetRootNode());
    return true;
}

void CAstNode::CreateValue(CSymbol* pSym)
{
	m_pValue = new CValue;
	m_pValue->Create(pSym);
}

void CAstNode::Print(FILE* pOut, int Indent, char* s, int Strlen)
{

	if (pOut)
	{
		fprintf(pOut, "%s\n",
			Print(Indent,s,Strlen)
		);
	}	
}

char* CAstNode::Print(int Indent, char* s, int Strlen)
{
	int i = 0, l = 0;
	int Id, Child, Next;
	int size;

	Id = GetID();
	if (GetChild())
		Child = GetChild()->GetID();
	else
		Child = -1;
	if (GetNext())
		Next = GetNext()->GetID();
	else
		Next = -1;
	size = Strlen - l;
	l += sprintf_s(&s[l], size, "%6d %6d %6d  ", Id, Child, Next);
	for (i = 0; i < Indent; ++i)
	{
		size = Strlen - l;
		l += sprintf_s(&s[l], size, "| ");
	}
	size = Strlen - l;
	l += sprintf_s(&s[l], size, "+-%s", GetNodeName());
	if (GetValue())
	{
		if (GetValue()->GetSymbol())
		{
			if (GetValue()->GetSymbol()->GetName())
			{
				size = Strlen - l;
				l += sprintf_s(&s[l], size, ": %s", GetValue()->GetSymbol()->GetName());
			}
		}
	}
	return s;
}

void CAstNode::AddToHeadNextChain(CAstNode* pN)
{
	if (GetHead())
	{
		GetHead()->SetPrev(pN);
		pN->SetNext(GetHead());
		SetHead(pN);
	}
	else
	{
		SetTail(pN);
		SetHead(pN);
	}
}


void CAstNode::AddToTailNextChain(CAstNode* pNode)
{
	if (GetHead())
	{
		GetTail()->SetNext(pNode);
		pNode->SetPrev(GetTail());
		SetTail(pNode);
	}
	else
	{
		SetTail(pNode);
		SetHead(pNode);
	}
}

void CAstNode::InsertThatIntoThisNext(CAstNode* pN)
{
	CAstNode* pTemp;

	pTemp = GetNext();
	SetNext(pN);
	pN->SetNext(pTemp);
}

void CAstNode::AddThatToThisNext(CAstNode* pN)
{
	CAstNode* pNode = this;

	if (pN)
	{
		while (pNode->GetNext())
		{
			pNode = pNode->GetNext();
		}
		pNode->SetNext(pN);
	}
}

void CAstNode::AddThisToThatNext(CAstNode* pN)
{
	CAstNode* pNode = pN;
	while (pNode->GetNext())
		pNode = pNode->GetNext();
	pNode->SetNext(this);
}

void CAstNode::AddThatToThisChild(CAstNode* pN)
{
	CAstNode* pNode = GetChild();
	if (pN)
	{
		if (!pNode)
			SetChild(pN);
		else
		{
			while (pNode->GetNext())
			{
				pNode = pNode->GetNext();
			}
			pNode->SetNext(pN);
		}

	}
}
