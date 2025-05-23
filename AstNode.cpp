#include "pch.h"

CAstNode::CAstNode()
{
	m_NodeID = ++m_NodeCount;
	m_NodeType = NodeType::BASE;
	m_pStart = 0;
	m_pNext = 0;
	m_pPrev = 0;
	m_pChild = 0;
	m_pParent = 0;
	m_pNodeName = 0;
	m_pValue = 0;
	m_pHead = 0;
	m_pTail = 0;
	m_pSection = GetParser()->GetCurrentSection();
	m_Line = Act()->GetParser()->GetLexer()->GetLineNumber();
	m_Column = Act()->GetParser()->GetLexer()->GetColunm();
	m_pChildValue = 0;
	m_pNextValue = 0;
	m_pResultValue = 0;
}

CAstNode::CAstNode(const char* pName, NodeType NT)
{
	m_NodeID = ++m_NodeCount;
	m_NodeType = NT;
	m_pStart = 0;
	m_pNext = 0;
	m_pPrev = 0;
	m_pChild = 0;
	m_pParent = 0;
	m_pNodeName = pName;
	m_pValue = 0;
	m_pHead = 0;
	m_pTail = 0;
	m_pSection = GetParser()->GetCurrentSection();
	m_Line = GetParser()->GetLexer()->GetLineNumber();
	m_Column = GetParser()->GetLexer()->GetColunm();
}

CAstNode::~CAstNode()
{
	if (m_pValue)
		delete m_pValue;
}

CAstNode* CAstNode::MakeNode(
	CAstNode* pChild,
	CAstNode* pNext
)
{
	//------------------------------------------------
	//
	// MakeNode
	//
	// Paramerters:
	//	pChild.......Child node
	//	pNext........Next Node
	//	pSym.........Symbol associated with this node
	// Returns:
	//	"this"
	//
	// When done, you should have this:
	// 
	//		+THIS_NODE-pSym
	//		|	+ CHILD
	//		|	+ NEXT
	//------------------------------------------------

	CAstNode* pNode = 0;

	if (pChild)
	{
		SetChild(pChild);
		pChild->SetParent(this);
		if(pNext)
			pNext->SetParent(this);
		pNode = pChild;
		while (pNode->GetNext())
			pNode = pNode->GetNext();
		pNode->SetNext(pNext);
	}
	return this;
}

void CAstNode::CreateValue(CSymbol* pSym)
{
	m_pValue = new CValue;
	m_pValue->Create(pSym);
}

void CAstNode::CreateValue(const char* s)
{
	m_pValue = new CValue;
	m_pValue->Create(s);

}

CAstNode* CAstNode::CreateValue(int V)
{
	m_pValue = new CValue;
	m_pValue->Create(V);
	return this;
}

void CAstNode::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	if (pOut)
	{
		char* s = new char[256];
		Print(Indent, s, 256, pbNextFlag);
		fprintf(pOut, "%s\n",s);
		delete[] s;
	}	 
}

int CAstNode::Print(int Indent, char* s, int strLen, bool* pbNextFlag)
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
	size = strLen - l;
	l += sprintf_s(&s[l], size, "%6d %6d %6d  ", Id, Child, Next);
	size = strLen - l;
	l += MakeIndentString(&s[l], size, Indent, pbNextFlag);
	size = strLen - l;
	l += sprintf_s(&s[l], size, "+- \'%s\'", GetNodeName());
	return l;
}

bool CAstNode::IsBinOpNode()
{
	bool rV = false;

	switch (GetNodeType())
	{
	case NodeType::SUBTRACT:
	case NodeType::ADDEXPR:
	case NodeType::BITWISE_AND:
	case NodeType::BITWISE_OR:
	case NodeType::XOR:
	case NodeType::LSH:
	case NodeType::RSH:
	case NodeType::MULTIPLY:
	case NodeType::DIVIDE:
	case NodeType::MOD:
		rV = true;
		break;
	}
    return rV;
}

CAstNode* CAstNode::SetChild(CAstNode* pAN)
{
	m_pChild = pAN;
	if(pAN)
		pAN->SetParent(this);
	return this;
}


CAstNode* CAstNode::AddToNext(CAstNode* pNode)
{
	CAstNode* pN = this;

	while (pN->GetNext())
		pN = pN->GetNext();
	pN->SetNext(pNode);
	return this;
}

CAstNode* CAstNode::AddToChildChain(CAstNode* pHead, CAstNode* pNode)
{
	CAstNode* pN = 0;

	if (pHead)
	{
		pN = pHead;
		while (pN->GetChild())
			pN = pN->GetChild();
		pN->SetChild(pNode);
	}
	else
	{
		pHead = pNode;
	}
	return pHead;
}

CAstNode* CAstNode::MakeNextList(CAstNode* pList, CAstNode* pListMember)
{
	CAstNode* pNode;

	if (pList)
	{
		if (pListMember)
		{
			pListMember->SetParent(pList->GetParent());
			pNode = pList->GetNext();
			if (pNode)
			{
				while (pNode->GetNext())
				{
					pNode = pNode->GetNext();
				}
				pNode->SetNext(pListMember);
			}
			else
			{
				pList->SetNext(pListMember);
			}
		}
	}
	else
		pList = pListMember;
	return pList;
}

CAstNode* CAstNode::MakeChildList(CAstNode* pList, CAstNode* pChild)
{
	CAstNode* pNode = 0;

	if (pList)
	{
		pChild->SetParent(pList);
		pNode = pList->GetChild();
		if (pNode)
		{
			while (pNode->GetNext())
			{
				pNode = pNode->GetNext();
			}
			pNode->SetNext(pChild);
		}
		else
		{
			pList->SetChild(pChild);
		}
	}
	else
	{
		fprintf(stderr, "Internal Error 101\n");
		Act()->Exit(101);
	}
    return pList;
}

CAstNode* CAstNode::GetLastNext()
{
	CAstNode* pN = this;

	while (pN->GetNext())
		pN = pN->GetNext();
	return pN;
}

int CAstNode::MakeIndentString(char* s, int size, int Indent, bool* pbNextFlag)
{
	int j;
	int l = 0;
	int sz = size;

	for (j = 0; j < Indent; ++j)
	{
		if (pbNextFlag[j])
			l += sprintf_s(&s[l], sz, "|  ");
		else
			l += sprintf_s(&s[l], sz, "   ");
		sz = size - l;
	}
	return l;
}

CParser* CAstNode::GetParser()
{
	return Act()->GetParser();
}

FILE* CAstNode::LogFile()
{
	return GetParser()->LogFile();
}
