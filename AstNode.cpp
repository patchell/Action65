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
	GetChild()->SetParent(this);
	GetNext()->SetPrev(this);
	SetStart(Act()->GetParser()->GetAstTree()->GetRootNode());
    return true;
}

bool CAstNode::CreateLeaf(CAstNode* pChild, CAstNode* pNext, CAstNode* pPrev, CAstNode* pStart, CAstNode* pParent)
{
    return false;
}

void CAstNode::Print(FILE* pOut, int Indent)
{
}

void CAstNode::NodeProc()
{
	TopDown();
	if (m_pChild)
		m_pChild->NodeProc();
	Sideways();
	if (m_pNext)m_pNext->NodeProc();
	BottomUp();
}

void CAstNode::TopDown()
{
}

void CAstNode::BottomUp()
{
}

void CAstNode::Sideways()
{
}
