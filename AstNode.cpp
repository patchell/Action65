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
	m_pSym = 0;
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
	m_pSym = 0;
}

CAstNode::~CAstNode()
{
}

bool CAstNode::CreateNode(
	CAstNode* pChild,
	CAstNode* pNext
)
{
	SetChild(pChild);
	SetNext(pNext);
	if(pChild)GetChild()->SetParent(this);
	if(pNext)GetNext()->SetPrev(this);
	SetStart(Act()->GetParser()->GetAstTree()->GetRootNode());
    return true;
}

void CAstNode::Print(FILE* pOut, int Indent, char* s)
{
	int ChildID, NextID;
//	char* pIndentString;

//	pIndentString = new char[256];
//	Act()->IndentString(pIndentString, Indent, ' ');
	if (GetChild())
		ChildID = GetChild()->GetID();
	else
		ChildID = -1;
	if (GetNext())
		NextID = GetNext()->GetID();
	else
		NextID = -1;
	fprintf(
		pOut,
		";\t %4d %4d %4d %s%s %s\n",
		GetID(),
		ChildID,
		NextID,
		s,
		this->GetNodeName(),
		GetSymbol()?GetSymbol()->GetName():"..."
	);
//	delete[] pIndentString;
}
